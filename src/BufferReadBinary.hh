/*
        Buffer for reading binary STL file
*/
#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>
//#include "Components.hh"

class BufferReadBinary
{
private:
    char* prebuffer;  // just before buffer
    char* buffer;
    char* phead;
    int fn;
    uint32_t BUFFER_SIZE    = 32768;  // 32K
    uint32_t PREBUFFER_SIZE = 64;
    uint32_t readIn;
    uint32_t remain;  // bytes that left in buffer, not enough for one triangle
    uint32_t numTrianglesInFile;
    uint32_t numTrianglesInBlock;

public:
    BufferReadBinary(std::string& fileTarget);
    ~BufferReadBinary();
    uint32_t getNumTrianglesInFile();
    uint32_t getNumTrianglesInBlock();
    void step2Bytes();
    void stepFloat();
    float getFloat();
    void readBuffer();
};
