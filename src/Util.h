#pragma once

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

#define LOG_DEBUG(tag, format, ...) fprintf(stdout, "[DEBUG] [" tag "] " format "\n", ##__VA_ARGS__); fflush(stdout)
#define LOG_INFO(tag, format, ...) fprintf(stdout, "[INFO] [" tag "] " format "\n", ##__VA_ARGS__); fflush(stdout)
#define LOG_WARN(tag, format, ...) fprintf(stderr, "[WARN] [" tag "] " format "\n", ##__VA_ARGS__); fflush(stderr)
#define LOG_ERROR(tag, format, ...) fprintf(stderr, "[ERROR] [" tag "] " format "\n", ##__VA_ARGS__); fflush(stderr)

struct IPAddress {
    std::string ip;
    uint16_t port;

    bool operator<(const IPAddress &rhs) const {
        return std::tie(ip, port) < std::tie(rhs.ip, rhs.port);
    }
};

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using Vector = std::vector<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename K, typename V>
using UnorderedMap = std::unordered_map<K, V>;

template<typename T, typename... Args>
UniquePtr<T> MakeUnique(Args &&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
SharedPtr<T> MakeShared(Args &&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
