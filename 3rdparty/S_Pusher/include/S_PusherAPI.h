//
// Created by kqbi on 2020/2/19.
//

#ifndef S_PUSHER_S_PUSHERAPI_H
#define S_PUSHER_S_PUSHERAPI_H

#include <stdint.h>
#include <functional>

#if defined( _WIN32 ) || defined( __MINGW32__ )
#   if defined( S_PUSHER_EXPORTS )
#       define S_PUSHER_EXPORT __declspec(dllexport)
#       define S_PUSHER_CALL __stdcall
#   elif defined( S_PUSHER_USE_DLL_IMPORT ) || !defined( S_PUSHER_USE_STATIC_LIB )
#       define S_PUSHER_EXPORT __declspec(dllimport)
#       define S_PUSHER_CALL __stdcall
#   else
#       define S_PUSHER_EXPORT
#       define S_PUSHER_CALL
#   endif
#else
#   define S_PUSHER_EXPORT
#   define S_PUSHER_CALL
#endif

#define S_Pusher_Handle void*

struct S_MEDIA_INFO {
    uint32_t MediaType;             /* 媒体类型，0-视音混合，1-只有视频，2-只有音频 */
    uint32_t SpsLength;                /* sps长度，0-从后续frame获取 */
    uint32_t PpsLength;                /* pps长度，0-从后续frame获取 */
    char Sps[255];                /* sps数据，0-从后续frame获取 */
    char Pps[128];                /* pps数据，0-从后续frame获取 */

    uint32_t AudioSamplerate;        /* 采样率 */
    uint32_t AudioChannel;            /* 通道号 */
    uint32_t AudioBitsPerSample;    /* 采样位数，只支持16, 0-aac采样率、通道号等信息从后续frame获取*/
    uint32_t AudioProfile;            /* aac级别，0-m,1-lc,2-ssr */
};

enum S_PUSHER_STATE
{
    S_PUSHER_STATE_PUSH_SUCCESS = 0,             /* 推流成功 */
    S_PUSHER_STATE_CONNECT_TIME_OUT = 1,         /* 连接失败 */
    S_PUSHER_STATE_DISCONNECTED = 2,             /* 断开连接 */
    S_PUSHER_STATE_ERROR = 3
};

using S_Pusher_Callback = std::function<void (int state, void *userdata)>;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 创建推流句柄，返回句柄handle
 * @return 推流句柄
 */
S_PUSHER_EXPORT S_Pusher_Handle S_PUSHER_CALL S_Pusher_Create();

/**
 * 销毁推流句柄
 *  @param handle 推流句柄
 */
S_PUSHER_EXPORT int S_PUSHER_CALL S_Pusher_Release(S_Pusher_Handle handle);

/**
 * 设置状态回调函数
 * @param handle 推流句柄
 * @param 回调函数指针
 * @param 用户指针
 * @return 返回成功与否，0-成功
 */
S_PUSHER_EXPORT int S_PUSHER_CALL S_Pusher_SetCallback(S_Pusher_Handle handle,  S_Pusher_Callback callback, void *userdata);

/**
 * 初始化推流url、及流传输方式
 * @param handle 推流句柄
 * @param url 推流地址url，例如：rtmp://127.0.0.1:1935/live/11
 * @param rtp_type 流传输方式："0"-RTP Over TCP，"1"-RTP Over UDP
 * @return 返回成功与否，0-成功
 */
S_PUSHER_EXPORT int S_PUSHER_CALL
S_Pusher_init_media(S_Pusher_Handle handle, S_MEDIA_INFO &mediaInfo, const char *url, const char *rtp_type = "0");

/**
 * 输入单帧H264视频，帧起始字节00 00 01,00 00 00 01均可
 * @param handle 推流句柄
 * @param data 单帧H264数据
 * @param len 单帧H264数据字长度
 * @param dts 解码时间戳，单位毫秒
 * @param pts 播放时间戳，单位毫秒
 * @return 返回成功与否，0-成功
 */
S_PUSHER_EXPORT int S_PUSHER_CALL
S_Pusher_input_h264(S_Pusher_Handle handle, void *data, int len, uint32_t dts, uint32_t pts);

/**
 * 输入单帧AAC音频
 * @param handle 推流句柄
 * @param data 单帧AAC数据
 * @param len 单帧AAC数据字节数
 * @param dts 时间戳，毫秒
 * @param adts头，0-不带，1-带着
 * @return 返回成功与否，0-成功
 */
S_PUSHER_EXPORT int S_PUSHER_CALL
S_Pusher_input_aac(S_Pusher_Handle handle, void *data, int len, uint32_t dts, int with_adts_header = 1);

/**
 * 停止推送
 *  @param handle 推流句柄
 *  @return 返回成功与否，0-成功
 */
S_PUSHER_EXPORT int S_PUSHER_CALL S_Pusher_StopPusher(S_Pusher_Handle handle);

#ifdef __cplusplus
}
#endif

#endif //S_PUSHER_S_PUSHERAPI_H