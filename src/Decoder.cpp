#include "Decoder.h"


int frameCount = 0;


MyDecoder::MyDecoder () {
    std::string param;
    std::string input_format = VIDEO_H264;
    int split_mode = 1;
    int timeout = 0;
    PARAM_STRING_APPEND(param, KEY_INPUTDATATYPE, input_format);
    // PARAM_STRING_APPEND(param, KEY_OUTPUTDATATYPE, IMAGE_RGB888);
    PARAM_STRING_APPEND_TO(param, KEY_MPP_SPLIT_MODE, split_mode);
    PARAM_STRING_APPEND_TO(param, KEY_OUTPUT_TIMEOUT, timeout);

    easymedia::REFLECTOR(Decoder)::DumpFactories();
    std::string mpp_codec = "rkmpp";
    mpp_dec = easymedia::REFLECTOR(Decoder)::Create<easymedia::VideoDecoder>(
        mpp_codec.c_str(), param.c_str());
    file_eos = false;
    eos = false;
    output_file_fd = open("/data/re.nv12", O_RDWR | O_CREAT);
    return;
}


// 将nv12文件转为jpg图片
void ReadYUV(int width, int height, void *ptr) {
	char buffer[200] = {0};
	int iImageSize = iImageSize = width * height * 3 / 2;
	cv::Mat yuvImg;
	cv::Mat rgbImg(height, width, CV_8UC3);
	yuvImg.create(height * 3 / 2, width, CV_8UC1);
	memcpy(yuvImg.data, ptr, iImageSize * sizeof(unsigned char));
	cv::cvtColor(yuvImg, rgbImg, CV_YUV2RGBA_NV12);
	snprintf(buffer, 200, "/data/raw/%d.jpg", ++frameCount);
	cv::imwrite(buffer, rgbImg);
	printf("write /data/raw/%d.jpg\n", frameCount);
}


MyDecoder ::~MyDecoder() {

}
 

// 输出nv12、jpg文件
void MyDecoder::dump_output(const std::shared_ptr<easymedia::MediaBuffer> &out) {
	auto out_image = std::static_pointer_cast<easymedia::ImageBuffer>(out);
	// hardware always need aligh width/height, we write the whole buffer with
	// virtual region which may contains invalid data

	if (output_file_fd >= 0 && out_image->GetValidSize() > 0) {
		const ImageInfo &info = out_image->GetImageInfo();
		printf("got one frame, format: %s <%dx%d>in<%dx%d>\n", PixFmtToString(info.pix_fmt), info.width, info.height,
			info.vir_width, info.vir_height);
		ReadYUV(out_image->GetVirWidth(), out_image->GetVirHeight(), out_image->GetPtr());
		ssize_t count = write(output_file_fd, out_image->GetPtr(), CalPixFmtSize(out_image->GetPixelFormat(),
			out_image->GetVirWidth(), out_image->GetVirHeight()));
		if (count < 0) {
		// RKMEDIA_LOGI("dump_output: write output_file_fd failed\n");
		}
	}
}


bool MyDecoder::get_output_and_process(easymedia::VideoDecoder *mpp_dec) {

	auto out_buffer = mpp_dec->FetchOutput();

	if (!out_buffer && errno != 0) {
		fprintf(stderr, "fatal error %m\n");
		eos = true; // finish
		return false;
	}
	if (out_buffer && !out_buffer->IsValid() && !out_buffer->IsEOF()) {
		// got a image info buffer
		// fprintf(stderr, "got info frame\n");
		// fetch the real frame
		out_buffer = mpp_dec->FetchOutput();
	}
	if (out_buffer) {

		dump_output(std::static_pointer_cast<easymedia::ImageBuffer>(out_buffer));
		if (out_buffer->IsEOF()) {
			eos = true;
			return false; // finish
		}
	}
	return true;
}


void MyDecoder::Decode_h264(void *readbuf, size_t read_size) {
	std::shared_ptr<easymedia::MediaBuffer> last_buffer;
	bool sent_eos = false;
	// fprintf(stderr, "Decode_h264 %d %d\n", sent_eos ,file_eos);
	// while (!eos)
	{
		if (last_buffer) {
			int ret = mpp_dec->SendInput(last_buffer);
			// printf("----------- last_buffer%d\n",ret );
			if (ret != -EAGAIN) {
				if (last_buffer->IsEOF()) {
					last_buffer = nullptr;
					get_output_and_process(mpp_dec.get());
					// break;
				}
				last_buffer = nullptr;
			}
		}
		// else
		{
			std::shared_ptr<easymedia::MediaBuffer> buffer;

			buffer = easymedia::MediaBuffer::Alloc(read_size);
			assert(buffer);
			memcpy(buffer->GetPtr(), readbuf, read_size);
			// fprintf(stderr, "fread size = %d\n", (int)read_size);
			buffer->SetValidSize(read_size);

			buffer->SetUSTimeStamp(easymedia::gettimeofday());
			int ret = mpp_dec->SendInput(buffer);
			// fprintf(stderr, "Decode_h264-- SendInput - %d %d\n", ret ,EAGAIN);
			if (ret == -EAGAIN) {
				last_buffer = buffer;
			}
		}
		get_output_and_process(mpp_dec.get());
	}
}
