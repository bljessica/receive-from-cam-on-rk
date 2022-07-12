#include <stdio.h>
#include <fstream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "buffer.h"
#include "key_string.h"
#include "media_config.h"
#include "utils.h"
#include "decoder.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#define MPI_DEC_STREAM_SIZE 4096

#pragma once
class MyDecoder
{
  public:
    MyDecoder();
    ~MyDecoder();
    void Decode_h264(void *data, size_t len);
    void dump_output(const std::shared_ptr<easymedia::MediaBuffer> &out);
    bool get_output_and_process(easymedia::VideoDecoder *mpp_dec);

  private:
    std::shared_ptr<easymedia::VideoDecoder> mpp_dec;
    std::shared_ptr<easymedia::MediaBuffer> last_buffer;
    bool file_eos;
    bool eos;
    int output_file_fd;
};