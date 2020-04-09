/*
        Buffer for writing
*/
#include "BufferWrite.hh"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

BufferWrite::BufferWrite(std::string& fileTarget)
{
    buffer     = new char[BUFFER_SIZE + POSTBUFFER_SIZE];
    postbuffer = buffer + BUFFER_SIZE;
    phead      = buffer;
    remove(fileTarget.c_str());
    creat(fileTarget.c_str(), S_IREAD | S_IWRITE);
    fn = open(fileTarget.c_str(), O_WRONLY | O_APPEND);
}
BufferWrite::~BufferWrite()
{
    flush();
    delete[] buffer;
    close(fn);
}

void BufferWrite::flushOnlyBuffer()
{
    size_t size = write(fn, buffer, BUFFER_SIZE);
    // copy the stuff in postbuffer to buffer
    for (int i = 0; i < phead - postbuffer; i++) buffer[i] = postbuffer[i];
    phead -= BUFFER_SIZE;
}

void BufferWrite::writeG0(double x, double y)
{
    *this << "G0 F4800 X" << x << " Y" << y << '\n';
    if (phead >= postbuffer) flushOnlyBuffer();
}
void BufferWrite::writeG1(double x, double y, double e)
{
    *this << "G1 F2000 X" << x << " Y" << y << " E" << e << '\n';
    if (phead >= postbuffer) flushOnlyBuffer();
}
void BufferWrite::flush()
{
    uint32_t sizeToWrite = phead - buffer;
    size_t size          = write(fn, buffer, sizeToWrite);
    phead                = buffer;
}

BufferWrite& operator<<(BufferWrite& buf, const char* ch)
{
    // check buffer is full or not. If full, only flush the buffer
    for (int i = 0; ch[i] != '\0'; i++, buf.phead++) *buf.phead = ch[i];
    return buf;
}
BufferWrite& operator<<(BufferWrite& buf, char ch)
{
    *buf.phead = ch;
    buf.phead++;
    return buf;
}
BufferWrite& operator<<(BufferWrite& buf, std::string& str)
{
    return buf << str.c_str();
}
BufferWrite& operator<<(BufferWrite& buf, double val)
{
#if 0
    //TODO: translating double to string REALLY take so much time!!!
    //TODO: make our own doubleToString()
    //snprintf(buf.phead, 9, "%lf", val);
    //buf.phead += 9;
    std::string str = std::to_string(val);
    return buf << str;
#endif
#if 1  // faster way for DoubleToString
    buf.phead = formatDouble(val, buf.phead);
    return buf;
#endif
}
