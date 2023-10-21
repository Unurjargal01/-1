#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <deque>
class LruCache {
public:
    explicit LruCache(size_t max_size) : max_size_(max_size) {
    }
    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;

    using ListIt = std::list<std::pair<std::string, std::string>>::iterator;
    void Set(const std::string& key, const std::string& value) {
        // if key is in the log_, just delete, then value;
        if (data_.find(key) != data_.end()) {
            Remove(key);
        }
        // if key is not, delete the first one, and add key and value;
        if (max_size_ == log_.size()) {
            // *log_.begin() is value;
            Remove((*log_.begin()).first);
            // in the Remove function has also erase log_
            // log_.erase(log_.begin());
        }
        Insert(key, value);
    }

    bool Get(const std::string& key, std::string* value) {
        // if key is in the log_, change the position, return true;
        if (data_.find(key) == data_.end()) {
            return false;
        }
        std::string val = Remove(key);
        Insert(key, val);
        *value = val;
        return true;
    }

    std::string Remove(const std::string& key) {
        auto u = data_.find(key);
        if (u != data_.end()) {
            log_.erase(u->second);
            data_.erase(u);
            return (*(u->second)).second;
        }
        return "";
    }
    void Insert(const std::string& key, const std::string& value) {
        ListIt it = log_.insert(log_.end(), {key, value});
        data_.insert({key, it});
    }

private:
    size_t max_size_;
    std::list<std::pair<std::string, std::string>> log_;
    std::unordered_map<std::string, ListIt> data_;
};
