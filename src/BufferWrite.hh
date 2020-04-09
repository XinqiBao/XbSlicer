/*
        Buffer for writing
*/

#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

#include "DoubleToString.hh"
//#include "Components.hh"

class BufferWrite
{
private:
    char* postbuffer;  // just after buffer
    char* buffer;
    char* phead;
    int fn;
    uint32_t BUFFER_SIZE     = 32768;  // 32K
    uint32_t POSTBUFFER_SIZE = 256;
    void flushOnlyBuffer();

public:
    void writeG0(double x, double y);
    void writeG1(double x, double y, double e);
    void flush();

    BufferWrite(std::string& fileTarget);
    ~BufferWrite();
    friend BufferWrite& operator<<(BufferWrite& buf, const char* ch);
    friend BufferWrite& operator<<(BufferWrite& buf, char ch);
    friend BufferWrite& operator<<(BufferWrite& buf, std::string& str);
    friend BufferWrite& operator<<(BufferWrite& buf, double val);
};
