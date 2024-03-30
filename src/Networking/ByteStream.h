#pragma once

#include <vector>
#include <cstdint>

class ByteStream {
public:
    ByteStream();

    ByteStream(char *data, size_t size);

    [[nodiscard]] const char *GetData() const;

    [[nodiscard]] size_t GetSize() const;

public:
    template<typename T>
    void Write(T value) {
        WriteBytes(&value, sizeof(T));
    }

    void Write(char value);

    void Write(const char *value);

    void Write(bool value);

    void Write(int16_t value);

    void Write(uint16_t value);

    void Write(int32_t value);

    void Write(uint32_t value);

    void Write(int64_t value);

    void Write(uint64_t value);

    void Write(float value);

public:
    template<typename T>
    void Read(T &value) {
        ReadBytes(&value, sizeof(value));
    }

    void Read(char &value);

    void Read(bool &value);

    void Read(int16_t &value);

    void Read(uint16_t &value);

    void Read(int32_t &value);

    void Read(uint32_t &value);

    void Read(int64_t &value);

    void Read(uint64_t &value);

    void Read(float &value);

private:
    void WriteBytes(const void *source, unsigned int size);

    void ReadBytes(void *destination, size_t size);

private:
    std::vector<char> data;
    size_t size;
    size_t position;
};
