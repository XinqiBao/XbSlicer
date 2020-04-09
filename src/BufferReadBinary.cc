/*
        Buffer for reading binary STL file
*/
#include "BufferReadBinary.hh"

BufferReadBinary::BufferReadBinary(std::string& fileTarget)
    : remain(0), readIn(0), numTrianglesInBlock(0)
{
    prebuffer          = new char[PREBUFFER_SIZE + BUFFER_SIZE];
    buffer             = prebuffer + PREBUFFER_SIZE;
    fn                 = open(fileTarget.c_str(), O_RDONLY);
    readIn             = read(fn, buffer, BUFFER_SIZE);
    phead              = buffer + 80;
    numTrianglesInFile = *(uint32_t*)phead;
    phead += sizeof(uint32_t);
    readIn -= 84;
    numTrianglesInBlock = readIn / 50;
    remain              = readIn % 50;
}
BufferReadBinary::~BufferReadBinary()
{
    delete[] prebuffer;
    close(fn);
}
uint32_t BufferReadBinary::getNumTrianglesInFile()
{
    return numTrianglesInFile;
}
uint32_t BufferReadBinary::getNumTrianglesInBlock()
{
    return numTrianglesInBlock;
}
void BufferReadBinary::step2Bytes() { phead += 2; }
void BufferReadBinary::stepFloat() { phead += sizeof(float); }
float BufferReadBinary::getFloat() { return *(float*)phead; }
void BufferReadBinary::readBuffer()
{
    char* pd = buffer - remain;
    char* pf = buffer + BUFFER_SIZE - remain;
    for (int i = remain; i > 0; i--, pd++, pf++) *pd = *pf;
    readIn              = read(fn, buffer, BUFFER_SIZE);
    phead               = buffer - remain;
    numTrianglesInBlock = (readIn + remain) / 50;
    remain              = (readIn + remain) % 50;
}
