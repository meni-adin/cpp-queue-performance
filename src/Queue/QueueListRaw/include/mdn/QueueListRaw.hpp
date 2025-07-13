
#ifndef MDN_QUEUE_LIST_RAW_HPP
#define MDN_QUEUE_LIST_RAW_HPP

#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueListRaw : public mdn::Queue<T> {
    public:

        void
        enqueue(T value) override;

        T
        dequeue() override;

        const T
        &front() override;

        bool
        isEmpty() const override;

        size_t
        size() const override;

        ~QueueListRaw() override;

    private:
        struct Node {
        public:
            T     value;
            Node *next;
            Node *prev{};

            Node(const T &value, Node *next) :
                value(value), next(next) {
                if (this->next != nullptr) {
                    this->next->prev = this;
                }
            }
        };

        Node  *front_{};
        Node  *back_{};
        size_t size_{};

        void
        verifyQueueNotEmpty();
    };

    template<typename T>
    void
    QueueListRaw<T>::enqueue(T value) {
        back_ = new Node(value, back_);
        if (size_ == 0) {
            front_ = back_;
        }
        ++size_;
    }

    template<typename T>
    T
    QueueListRaw<T>::dequeue() {
        verifyQueueNotEmpty();

        auto value = front_->value;
        if (front_ == back_) {
            delete front_;
            front_ = back_ = nullptr;
        } else {
            front_ = front_->prev;
            delete front_->next;
        }
        --size_;

        return value;
    }

    template<typename T>
    const T
    &QueueListRaw<T>::front() {
        verifyQueueNotEmpty();
        return front_->value;
    }

    template<typename T>
    bool
    QueueListRaw<T>::isEmpty() const {
        return size_ == 0;
    }

    template<typename T>
    size_t
    QueueListRaw<T>::size() const {
        return size_;
    }

    template<typename T>
    QueueListRaw<T>::~QueueListRaw() {
        while (front_ != nullptr) {
            auto next = front_->next;
            delete front_;
            front_ = next;
        }
    }

    template<typename T>
    void
    QueueListRaw<T>::verifyQueueNotEmpty() {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }
}  // namespace mdn

#endif  // MDN_QUEUE_LIST_RAW_HPP
