#pragma once

#include <reader.h>

#include <vector>
#include <memory>

class LimitReader : public Reader {
public:
    LimitReader(std::unique_ptr<Reader> reader, size_t limit)
        : reader_(std::move(reader)), limit_(limit) {
    }

    size_t Read(char* buf, size_t len) override {
        auto read_len = std::min<size_t>(limit_, len);
        // read_len = std::min<size_t>(read_len, 100);
        limit_ -= read_len;
        return reader_->Read(buf, read_len);
    }

private:
    std::unique_ptr<Reader> reader_;
    size_t limit_;
};

class TeeReader : public Reader {
public:
    TeeReader(std::vector<std::unique_ptr<Reader>> readers)
        : readers_(std::move(readers)), it_(readers_.begin()) {
    }

    size_t Read(char* buf, size_t len) override {
        size_t ans = 0;

        // std::cout<<"SIZE " << readers_.size() <<std::endl;
        while (len != 0 && it_ != readers_.end()) {
            size_t read_len = (*it_)->Read(buf + ans, len);
            if (!read_len) {
                ++it_;
            } else {
                ans += read_len;
                len -= read_len;
            }
        }
        return ans;
    }

private:
    std::vector<std::unique_ptr<Reader>> readers_;
    std::vector<std::unique_ptr<Reader>>::const_iterator it_;
};

class HexDecodingReader : public Reader {
public:
    HexDecodingReader(std::unique_ptr<Reader> reader) : reader_(std::move(reader)) {
    }

    size_t Read(char* buf, size_t len) override {
        size_t ans = 0;
        std::string str;
        while (len != 0) {
            std::vector<char> chunk(4);
            size_t readen = reader_->Read(chunk.data(), 2);
            if (readen == 0) {
                break;
            }
            int int_value = std::stoi(chunk.data(), nullptr, 16);
            str += static_cast<char>(int_value);

            len -= 1;
            ans += 1;
        }
        std::ranges::copy_n(str.begin(), ans, buf);
        return ans;
    }

private:
    std::unique_ptr<Reader> reader_;
};
