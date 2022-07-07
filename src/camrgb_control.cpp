/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 * author: Zhihua Wang, hogan.wang@rock-chips.com
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <dirent.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <string>

#include "rockface_control.h"
#include "video_common.h"

#ifdef CAMERA_ENGINE_RKISP
#include <camera_engine_rkisp/interface/rkisp_api.h>
#endif
#ifdef CAMERA_ENGINE_RKAIQ
#include <rkaiq/rkisp_api.h>
#endif
#include "rga_control.h"
// #include "rkfacial.h"

#include "rockface_ctl.h"
#include "SocketClient.h"

#include <iostream>
using namespace std;

#define IMG_WIDTH 1280
#define IMG_HEIGHT 720
#define IMG_DIR_PATH "/data/raw/"
#define IMG_SUFFIX ".raw"


static bool g_def_expo_weights = false;
bool g_expo_weights_en = false;
static unsigned char weights[81];

static const struct rkisp_api_ctx *ctx;
static const struct rkisp_api_buf *buf;
static bool g_run;
static pthread_t g_tid;

bool g_rgb_en;
int g_rgb_width;
int g_rgb_height;
// static display_callback g_display_cb = NULL;
static pthread_mutex_t g_display_lock = PTHREAD_MUTEX_INITIALIZER;
static int g_rotation = HAL_TRANSFORM_FLIP_H;

string last_img_path = ""; // 记录上一次识别的图片索引
SocketClient socket_client;

void set_rgb_rotation(int angle)
{
    if (angle == 90)
        g_rotation = HAL_TRANSFORM_ROT_90;
    else if (angle == 270)
        g_rotation = HAL_TRANSFORM_ROT_270;
}

// void set_rgb_display(display_callback cb)
// {
//     pthread_mutex_lock(&g_display_lock);
//     g_display_cb = cb;
//     pthread_mutex_unlock(&g_display_lock);
// }

// void set_rgb_param(int width, int height, display_callback cb, bool expo)
// {
//     g_rgb_en = true;
//     g_rgb_width = width;
//     g_rgb_height = height;
//     set_rgb_display(cb);
//     g_expo_weights_en = expo;
// }

static inline void camrgb_inc_fps(void)
{
    static int fps = 0;
    static struct timeval t0;
    struct timeval t1;

    fps++;
    gettimeofday(&t1, NULL);
    if (t1.tv_sec - t0.tv_sec > 1) {
        fps = 0;
        gettimeofday(&t0, NULL);
    } else if ((t1.tv_sec - t0.tv_sec) * 1000000 + (t1.tv_usec - t0.tv_usec) > 1000000) {
        printf("ISP fps: %d\n", fps);
        fps = 0;
        gettimeofday(&t0, NULL);
    }
}


int GetImgNum() {
    int file_num = 0;
    DIR *p_dir;
    struct dirent *ptr;
    if (!(p_dir = opendir("/data/raw/"))) {
        printf("Can not open img dir path.\n");
        return 0;
    }
    while ((ptr = readdir(p_dir)) != 0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            file_num++;
        }
    }
    closedir(p_dir);
    return file_num;
}


string GetNewestImgPath() {
    int img_num = GetImgNum();
    string num_str = to_string(img_num - 2);
    int padding_zero_num = 4 - num_str.length();
    for (int i = 0; i < padding_zero_num; i++) {
        num_str = "0" + num_str;
    }
    string full_path = IMG_DIR_PATH + num_str + IMG_SUFFIX;
    return full_path;
}

static void *process(void *arg)
{
    printf("process\n");
    int id = 0;

    do {
        id++;

        string img_path = GetNewestImgPath();
        FILE * fp;
        fp = fopen(img_path.c_str(), "rb+");
        if (fp == NULL) {
            printf("Read yuv error.\n");
        }
        int iImageSize = IMG_WIDTH * IMG_HEIGHT * 3 / 2 ;
        unsigned char *pYUVbuf = (uint8_t*)malloc(iImageSize * sizeof(uint8_t));
	    fread(pYUVbuf, iImageSize * sizeof(uint8_t), 1, fp);
        fclose(fp);

        rockface_image_t img;
        memset(&img, 0, sizeof(rockface_image_t));
        rockface_control_convert_detect(pYUVbuf, IMG_WIDTH, IMG_HEIGHT, RK_FORMAT_YUYV_420, g_rotation, id, &img);
        // if (!rockface_control_convert_detect(pYUVbuf, IMG_WIDTH, IMG_HEIGHT, RK_FORMAT_YCbCr_420_SP, g_rotation, id))
        //     rockface_control_convert_feature(pYUVbuf, IMG_WIDTH, IMG_HEIGHT, RK_FORMAT_YCbCr_420_SP, g_rotation, id);

        if (strcmp(last_img_path.c_str(), img_path.c_str()) != 0) {
            printf("Recog************%s\n", img_path.c_str());
            // 进行人脸识别
            std::string cmp_res = CompareImageWithFaceLib(&img);
            if (cmp_res.length() > 0) {
                // 发送人脸识别结果
                printf("Send*****************: %s\n", cmp_res.data());
                socket_client.SendMsg(cmp_res.data());
            } 
            last_img_path = img_path;
        }
        free(img.data);
        memset(&img, 0, sizeof(rockface_image_t));
        free(pYUVbuf);
    } while (g_run);

    pthread_exit(NULL);
}

int camrgb_control_init(SocketClient socket_cli)
{
    printf("cam init\n");
    int id = -1;
    char name[32];

    // if (!g_rgb_en)
    //     return 0;

    socket_client = socket_cli;
    // if (ctx == NULL) {
    //     printf("%s: ctx is NULL\n", __func__);
    //     return -1;
    // }

    // rkisp_set_buf(ctx, 3, NULL, 0);

    // rkisp_set_fmt(ctx, g_rgb_width, g_rgb_height, V4L2_PIX_FMT_NV12);

    // if (rkisp_start_capture(ctx))
    //     return -1;

    g_run = true;
    int res;
    if (res = pthread_create(&g_tid, NULL, process, NULL)) {
        printf("pthread_create fail.\n");
        return -1;
    }

    return 0;
}

void camrgb_control_exit(void)
{
    if (!g_rgb_en)
        return;

    g_run = false;
    if (g_tid) {
        pthread_join(g_tid, NULL);
        g_tid = 0;
    }

    // rkisp_stop_capture(ctx);
    // rkisp_close_device(ctx);
}

static void camrgb_control_expo_weights_270(int left, int top, int right, int bottom)
{
#ifdef CAMERA_ENGINE_RKISP
    if (!g_rgb_en)
        return;

    if (g_expo_weights_en) {
        unsigned char weights[81];
        int x = ctx->width - bottom;
        int y = left;
        int w = bottom - top;
        int h = right - left;
        x = x * 9 / ctx->width;
        y = y * 9 / ctx->height;
        w = w * 9 / ctx->width;
        h = h * 9 / ctx->height;
        w = w ? : 1;
        h = h ? : 1;
        memset(weights, 2, sizeof(weights));
        for (int j = 0; j < 9; j++)
            for (int i = 0; i < 9; i++)
                if (i > x && i <= x + w && j > y && j <= y + h)
                    weights[j * 9 + i] = 31;
        rkisp_set_expo_weights(ctx, weights, sizeof(weights));
        g_def_expo_weights = false;
    }
#endif
}

static void camrgb_control_expo_weights_90(int left, int top, int right, int bottom)
{
#ifdef CAMERA_ENGINE_RKISP
    if (!g_rgb_en)
        return;

    if (g_expo_weights_en) {
        unsigned char weights[81];
        int x = ctx->width - top;
        int y = ctx->height - right;
        int w = bottom - top;
        int h = right - left;
        x = x * 9 / ctx->width;
        y = y * 9 / ctx->height;
        w = w * 9 / ctx->width;
        h = h * 9 / ctx->height;
        w = w ? : 1;
        h = h ? : 1;
        memset(weights, 2, sizeof(weights));
        for (int j = 0; j < 9; j++)
            for (int i = 0; i < 9; i++)
                if (i > x && i <= x + w && j > y && j <= y + h)
                    weights[j * 9 + i] = 31;
        rkisp_set_expo_weights(ctx, weights, sizeof(weights));
        g_def_expo_weights = false;
    }
#endif
}

void camrgb_control_expo_weights(int left, int top, int right, int bottom)
{
    if (g_rotation == HAL_TRANSFORM_ROT_90)
        camrgb_control_expo_weights_90(left, top, right, bottom);
    else if (g_rotation == HAL_TRANSFORM_ROT_270)
        camrgb_control_expo_weights_270(left, top, right, bottom);
}

void camrgb_control_expo_weights_default(void)
{
#ifdef CAMERA_ENGINE_RKISP
    if (!g_rgb_en)
        return;

    if (g_expo_weights_en) {
        if (!g_def_expo_weights) {
            g_def_expo_weights = true;
            rkisp_set_expo_weights(ctx, weights, sizeof(weights));
        }
    }
#endif
}
