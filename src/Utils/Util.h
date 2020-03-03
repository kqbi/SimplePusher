//
// Created by kqbi on 2020/2/18.
//

#ifndef S_PUSHER_UTIL_H
#define S_PUSHER_UTIL_H

#include <stdio.h>
#include <string.h>
#include <memory>
#include <string>
#include <sstream>

#define StrPrinter _StrPrinter()

class _StrPrinter : public std::string {
public:
    _StrPrinter() {}

    template<typename T>
    _StrPrinter &operator<<(T &&data) {
        _stream << std::forward<T>(data);
        this->std::string::operator=(_stream.str());
        return *this;
    }

    std::string operator<<(std::ostream &(*f)(std::ostream &)) const {
        return *this;
    }

private:
    std::stringstream _stream;
};

//禁止拷贝基类
class noncopyable {
protected:
    noncopyable() {}

    ~noncopyable() {}

private:
    //禁止拷贝
    noncopyable(const noncopyable &that) = delete;

    noncopyable(noncopyable &&that) = delete;

    noncopyable &operator=(const noncopyable &that) = delete;

    noncopyable &operator=(noncopyable &&that) = delete;
};

/**
 * 获取1970年至今的毫秒数
 * @return
 */
uint64_t getCurrentMillisecond();


/**
 * 获取1970年至今的微秒数
 * @return
 */
uint64_t getCurrentMicrosecond();

/**
 * 获取时间字符串
 * @param fmt 时间格式，譬如%Y-%m-%d %H:%M:%S
 * @return 时间字符串
 */
std::string getTimeStr(const char *fmt, time_t time = 0);

std::string FindField(const char *buf, const char *start, const char *end, int bufSize = 0);

#endif //S_PUSHER_UTIL_H
