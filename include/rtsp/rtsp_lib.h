#ifndef _RTSPAPI_H_
#define _RTSPAPI_H_

#define API
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct _videoitem{
	void*buf;
	__uint32_t size;
	bool flag;
}video_item;

#define H264_FILE "rec.h264"

typedef struct {
	void (*on_audio_data)(const void *data_ptr, size_t data_len);
	void (*on_video_data)(const void *data_ptr, size_t data_len);
	void (*on_user_joined)(void);
	void (*on_user_offline)(void);
} rtsp_event_handler_t;

typedef void (*video_data_callback)(char *buf, int len);
typedef void (*audio_data_callback)(char *buf, int len);


void rtsp_video_packet_cb(void *buf, int size, bool flag);
void rtsp_audio_packet_cb(void *buf, int size );
void stop_rtspserver();
void start_rtspserver(const char * hostip, rtsp_event_handler_t event_handler);

void stop_rtspclient();
void  start_rtspclient(const char * serverip,const char* path ,rtsp_event_handler_t evt_handler  , const char* port, const char* suffix,const char* name ,const char * pwd);
//void start_rtspclient(const char * serverip ,rtsp_event_handler_t evt_handler  ,bool flag, const char* port, const char* suffix);
// void register_video_data_callback(video_data_callback cb);
// void register_audio_data_callback(audio_data_callback cb); 
#ifdef __cplusplus
}
#endif

#endif
