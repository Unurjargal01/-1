#pragma once

#include <string>
#include <cstddef>
#include <vector>

struct State {
    int ref_count;
    std::vector<std::string> v;
    State() : ref_count(1) {
    }
};

class COWVector {
public:
    COWVector() {
        state_ = new State;
    }
    COWVector(const COWVector& other) : state_(other.Ptr()) {
        state_->ref_count += 1;
    };
    // COWVector(COWVector&& other);
    COWVector& operator=(const COWVector& other) {
        if (this != &other) {
            state_->ref_count -= 1;
            if (state_->ref_count == 0) {
                delete state_;
            }

            state_ = other.Ptr();
            state_->ref_count += 1;
        }
        return *this;
    }
    // COWVector& operator= (COWVector&& other);
    ~COWVector() {
        state_->ref_count -= 1;
        if (state_->ref_count == 0) {
            delete state_;
        }
    }
    size_t Size() const {
        return (state_->v).size();
    }
    const std::string& Get(size_t at) const {
        return (state_->v)[at];
    }
    const std::string& Back() const {
        return (state_->v).back();
    }
    // Make Copy in the following instance;
    void Resize(size_t size) {
        if (size == (state_->v).size()) {
            return;
        } else if (!copied_) {
            MakeCopy();
        }
        (state_->v).resize(size);
    }

    void PushBack(const std::string& s) {
        if (!copied_) {
            MakeCopy();
        }
        (state_->v).push_back(s);
    };

    void Set(size_t at, const std::string& value) {
        if (!copied_) {
            MakeCopy();
        }
        (state_->v)[at] = value;
    };

private:
    State* state_ = nullptr;
    bool copied_ = false;
    State* Ptr() const {
        return state_;
    }
    void MakeCopy() {
        copied_ = true;
        State* temp = new State;
        temp->v = state_->v;
        state_->ref_count -= 1;
        if (state_->ref_count == 0) {
            delete state_;
        }
        state_ = temp;
    }
};