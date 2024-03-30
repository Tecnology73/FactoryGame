#include "ByteStream.h"
#include "WinSock2.h"
#include <stdexcept>

ByteStream::ByteStream() :
    data(),
    size(0),
    position(0) {
}

ByteStream::ByteStream(char *data, size_t size) :
    data(data, data + size),
    size(size),
    position(0) {
}

[[nodiscard]] const char *ByteStream::GetData() const {
    return data.data();
}

[[nodiscard]] size_t ByteStream::GetSize() const {
    return data.size();
}

void ByteStream::Write(char value) {
    WriteBytes(&value, sizeof(value));
}

void ByteStream::Write(const char *value) {
    auto len = strlen(value);
    Write(len);
    WriteBytes(value, len);
}

void ByteStream::Write(bool value) {
    WriteBytes(&value, sizeof(value));
}

void ByteStream::Write(int16_t value) {
    uint16_t networkValue = htons(static_cast<uint16_t>(value));
    WriteBytes(&networkValue, sizeof(networkValue));
}

void ByteStream::Write(uint16_t value) {
    uint16_t networkValue = htons(value);
    WriteBytes(&networkValue, sizeof(networkValue));
}

void ByteStream::Write(int32_t value) {
    uint32_t networkValue = htonl(static_cast<uint32_t>(value));
    WriteBytes(&networkValue, sizeof(networkValue));
}

void ByteStream::Write(uint32_t value) {
    uint32_t networkValue = htonl(value);
    WriteBytes(&networkValue, sizeof(networkValue));
}

void ByteStream::Write(int64_t value) {
    int64_t networkValue = htonll(value);
    WriteBytes(&networkValue, sizeof(networkValue));
}

void ByteStream::Write(uint64_t value) {
    uint64_t networkValue = htonll(value);
    WriteBytes(&networkValue, sizeof(networkValue));
}

void ByteStream::Write(float value) {
    uint32_t networkValue = htonl(*reinterpret_cast<uint32_t *>(&value));
    WriteBytes(&networkValue, sizeof(networkValue));
}

//
// Read
//

void ByteStream::Read(char &value) {
    ReadBytes(&value, sizeof(value));
}

void ByteStream::Read(bool &value) {
    ReadBytes(&value, sizeof(value));
}

void ByteStream::Read(int16_t &value) {
    ReadBytes(&value, sizeof(value));
    value = static_cast<int16_t>(ntohs(value));
}

void ByteStream::Read(uint16_t &value) {
    ReadBytes(&value, sizeof(value));
    value = ntohs(value);
}

void ByteStream::Read(int32_t &value) {
    uint32_t temp;
    ReadBytes(&temp, sizeof(temp));
    value = static_cast<int32_t>(ntohl(temp));
}

void ByteStream::Read(uint32_t &value) {
    ReadBytes(&value, sizeof(value));
    value = ntohl(value);
}

void ByteStream::Read(int64_t &value) {
    ReadBytes(&value, sizeof(value));
    value = ntohll(value);
}

void ByteStream::Read(uint64_t &value) {
    ReadBytes(&value, sizeof(value));
    value = ntohll(value);
}

void ByteStream::Read(float &value) {
    uint32_t temp;
    ReadBytes(&temp, sizeof(temp));
    temp = ntohl(temp);
    value = *reinterpret_cast<float *>(&temp);
}


void ByteStream::WriteBytes(const void *source, unsigned int size) {
    auto bytes = static_cast<const char *>(source);
    if (position + size > this->size) {
        data.resize(position + size);
    }

    memcpy(data.data() + position, bytes, size);
    position += size;
}

void ByteStream::ReadBytes(void *destination, size_t size) {
    auto bytes = static_cast<char *>(destination);
    if (size > this->size - position) {
        throw std::runtime_error("Cannot read from ByteStream");
    }

    memcpy(bytes, data.data() + position, size);
    position += size;
}