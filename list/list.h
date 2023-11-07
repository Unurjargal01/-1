#pragma once

#include <cstddef>
#include <iterator>
#include <utility>

struct ListNodeBase {
    ListNodeBase* prev_;
    ListNodeBase* next_;
    ListNodeBase() : prev_(this), next_(this) {}
};

template <typename T>
struct ListNode : public ListNodeBase {
    T value;
    ListNode(const T& val) : value(val) {}
    ListNode(T&& val) : value(std::move(val)) {}
};

template <typename T>
class List {
public:
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        ListNodeBase* node_;
        Iterator() = default;
        Iterator(ListNodeBase* node) : node_(node) {}
        Iterator(const Iterator& other) {
            node_ = other.node_;
        }
        Iterator(Iterator&& other) noexcept : node_(other.node_) {
            other.node_ = nullptr;
        }
        Iterator& operator= (const Iterator& other) {
            if (this != &other) {
                node_ = other.node_;
            }
            return *this;
        }
        Iterator& operator=(Iterator&& other) noexcept {
            if (this != &other) {
                node_ = other.node_;
                other.node_ = nullptr;
            }
            return *this;
        }
        ~Iterator() {}

        Iterator& operator++() {
            node_ = node_->next_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            ++(*this);
            return old;
        }

        Iterator& operator--() {
            node_ = node_->prev_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator old = *this;
            --(*this);
            return old;
        }

        T& operator*() const {
            if (node_ == nullptr || node_->next_ == nullptr) {
                throw std::logic_error("Attempt to dereference end iterator.");
            }
            return static_cast<ListNode<T>*>(node_)->value;
        }

        T* operator->() const {
            if (node_ == nullptr || node_->next_ == nullptr) {
                throw std::logic_error("Attempt to dereference end iterator.");
            }
            return &(static_cast<ListNode<T>*>(node_)->value);
        }

        bool operator==(const Iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const Iterator& other) const {
            return node_ != other.node_;
        }

    };

    List() : head_(new ListNodeBase()), size_(0) {}

    List(const List& other) : List() {
        for (const auto& value : other) {
            PushBack(value);
        }
    }

    List(List&& other) noexcept : head_(other.head_), size_(other.size_) {
        other.head_ = new ListNodeBase();
        other.size_ = 0;
    }

    ~List() {
        Clear();
        delete head_;
    }

    List& operator=(const List& other) {
        if (this != &other) {
            Clear();
            for (const auto& value : other) {
                PushBack(value);
            }
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            Clear();
            delete head_;
            head_ = other.head_;
            size_ = other.size_;
            other.head_ = new ListNodeBase();
            other.size_ = 0;
        }
        return *this;
    }

    bool IsEmpty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    void PushBack(const T& elem) {
        ListNode<T>* temp = new ListNode<T>(elem);
        LinkAfter(temp, head_->prev_);
    }

    void PushBack(T&& elem) {
        ListNode<T>* temp = new ListNode<T>(std::move(elem));
        LinkAfter(temp, head_->prev_);
    }

    void PushFront(const T& elem) {
        ListNode<T>* temp = new ListNode<T>(elem);
        LinkAfter(temp, head_);
    }

    void PushFront(T&& elem) {
        ListNode<T>* temp = new ListNode<T>(std::move(elem));
        LinkAfter(temp, head_);
    }

    T& Front() {
        return static_cast<ListNode<T>*>(head_->next_)->value;
    }

    const T& Front() const {
        return static_cast<ListNode<T>*>(head_->next_)->value;
    }

    T& Back() {
        return static_cast<ListNode<T>*>(head_->prev_)->value;
    }

    const T& Back() const {
        return static_cast<ListNode<T>*>(head_->prev_)->value;
    }

    void PopBack() {
        if (!IsEmpty()) {
            Unlink(head_->prev_);
        }
    }

    void PopFront() {
        if (!IsEmpty()) {
            Unlink(head_->next_);
        }
    }

    void Erase(Iterator pos) {
        if (pos.node_ != head_) {
            Unlink(pos.node_);
        }
    }

    Iterator Begin() {
        return Iterator(head_->next_);
    }

    Iterator End() {
        return Iterator(head_);
    }

    void Unlink(ListNodeBase* node) {
        if (node != head_) {
            ListNodeBase* prev = node->prev_;
            ListNodeBase* next = node->next_;
            prev->next_ = next;
            next->prev_ = prev;
            --size_;
            delete static_cast<ListNode<T>*>(node);
        }
    }

    void LinkAfter(ListNodeBase* target, ListNodeBase* after) {
        ListNodeBase* next = after->next_;
        after->next_ = target;
        target->prev_ = after;
        target->next_ = next;
        next->prev_ = target;
        ++size_;
    }

    void Clear() {
        while (!IsEmpty()) {
            PopFront();
        }
    }

private:
    ListNodeBase* head_;
    size_t size_;
};


template <typename T>
List<T>::Iterator begin(const List<T>& L) {
    return L.Begin();
};

template <typename T>
List<T>::Iterator end(const List<T>& L) {
    return L.End();
};
