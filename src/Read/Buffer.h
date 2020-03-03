//
// Created by xzl on 2019/3/18.
//

#ifndef ZLTOOLKIT_BUFFER_H
#define ZLTOOLKIT_BUFFER_H

#include <memory>
#include <string>
#include <deque>
#include <mutex>
#include <vector>
#include <atomic>
#include <sstream>
#include <functional>
#include "Utils/Util.h"
#include "Utils/List.h"

using namespace std;
//缓存基类
class Buffer : public noncopyable {
public:
    typedef std::shared_ptr<Buffer> Ptr;
    Buffer(){};
    virtual ~Buffer(){};
    //返回数据长度
    virtual char *data() const = 0 ;
    virtual uint32_t size() const = 0;

    virtual string toString() const {
        return string(data(),size());
    }

    virtual uint32_t getCapacity() const{
        return size();
    }
};

//指针式缓存对象，
class BufferRaw : public Buffer{
public:
    typedef std::shared_ptr<BufferRaw> Ptr;
    BufferRaw(uint32_t capacity = 0) {
        if(capacity){
            setCapacity(capacity);
        }
    }

    BufferRaw(const char *data,int size = 0){
        assign(data,size);
    }

    ~BufferRaw() {
        if(_data){
            delete [] _data;
        }
    }
    //在写入数据时请确保内存是否越界
    char *data() const override {
        return _data;
    }
    //有效数据大小
    uint32_t size() const override{
        return _size;
    }
    //分配内存大小
    void setCapacity(uint32_t capacity){
        if(_data){
            do{
                if(capacity > _capacity){
                    //请求的内存大于当前内存，那么重新分配
                    break;
                }

                if(_capacity < 2 * 1024){
                    //2K以下，不重复开辟内存，直接复用
                    return;
                }

                if(2 * capacity > _capacity){
                    //如果请求的内存大于当前内存的一半，那么也复用
                    return;
                }
            }while(false);

            delete [] _data;
        }
        _data = new char[capacity];
        _capacity = capacity;
    }
    //设置有效数据大小
    void setSize(uint32_t size){
        if(size > _capacity){
            throw std::invalid_argument("Buffer::setSize out of range");
        }
        _size = size;
    }
    //赋值数据
    void assign(const char *data,uint32_t size = 0){
        if(size <=0 ){
            size = strlen(data);
        }
        setCapacity(size + 1);
        memcpy(_data,data,size);
        _data[size] = '\0';
        setSize(size);
    }

    uint32_t getCapacity() const override{
        return _capacity;
    }
private:
    char *_data = nullptr;
    uint32_t _capacity = 0;
    uint32_t _size = 0;
};

#endif //ZLTOOLKIT_BUFFER_H
