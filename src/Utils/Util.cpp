//
// Created by kqbi on 2020/2/18.
//

#include "Util.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <time.h>
#include <Utils/onceToken.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include<sys/time.h>
#endif

static inline uint64_t getCurrentMicrosecondOrigin() {
#if !defined(_WIN32)
    struct timeval tv;
gettimeofday(&tv, NULL);
return tv.tv_sec * 1000000LL + tv.tv_usec;
#else
    return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
#endif
}

static std::atomic<uint64_t> s_currentMicrosecond(getCurrentMicrosecondOrigin());
static std::atomic<uint64_t> s_currentMillisecond(getCurrentMicrosecondOrigin() / 1000);

static inline bool initMillisecondThread() {
    static std::thread s_thread([]() {
        std::cout << "Stamp thread started!" << std::endl;
        uint64_t now;
        while (true) {
            now = getCurrentMicrosecondOrigin();
            s_currentMicrosecond.store(now, memory_order_release);
            s_currentMillisecond.store(now / 1000, memory_order_release);
#if !defined(_WIN32)
            //休眠0.5 ms
        usleep(500);
#else
            Sleep(1);
#endif
        }
    });
    static onceToken s_token([]() {
        s_thread.detach();
    });
    return true;
}

uint64_t getCurrentMillisecond() {
    static bool flag = initMillisecondThread();
    return s_currentMillisecond.load(memory_order_acquire);
}

uint64_t getCurrentMicrosecond() {
    static bool flag = initMillisecondThread();
    return s_currentMicrosecond.load(memory_order_acquire);
}

std::string getTimeStr(const char *fmt, time_t time) {
    std::tm tm_snapshot;
    if (!time) {
        time = ::time(NULL);
    }
#if defined(_WIN32)
    localtime_s(&tm_snapshot, &time); // thread-safe
#else
    localtime_r(&time, &tm_snapshot); // POSIX
#endif
    char buffer[1024];
    auto success = strftime(buffer, sizeof(buffer), fmt, &tm_snapshot);
    if (0 == success)
        return string(fmt);
    return buffer;
}

std::string FindField(const char *buf, const char *start, const char *end, int bufSize) {
    if (bufSize <= 0) {
        bufSize = strlen(buf);
    }
    const char *msg_start = buf, *msg_end = buf + bufSize;
    int len = 0;
    if (start != NULL) {
        len = strlen(start);
        msg_start = strstr(buf, start);
    }
    if (msg_start == NULL) {
        return "";
    }
    msg_start += len;
    if (end != NULL) {
        msg_end = strstr(msg_start, end);
        if (msg_end == NULL) {
            return "";
        }
    }
    return std::string(msg_start, msg_end);
}

