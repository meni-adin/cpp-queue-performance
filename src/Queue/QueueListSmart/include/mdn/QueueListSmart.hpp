
#ifndef MDN_QUEUE_LIST_SMART_HPP
#define MDN_QUEUE_LIST_SMART_HPP

#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueListSmart : public mdn::Queue<T> {
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

        ~QueueListSmart() override;

    private:
        struct Node {
        public:
            T                     value;
            std::unique_ptr<Node> next;
            Node                 *prev{};

            Node(const T &value, std::unique_ptr<Node> next) :
                value(value), next(std::move(next)) {
                if (this->next != nullptr) {
                    this->next->prev = this;
                }
            }
        };

        Node                 *front_;
        std::unique_ptr<Node> back_;
        size_t                size_{};

        void
        verifyQueueNotEmpty();
    };

    template<typename T>
    void
    QueueListSmart<T>::enqueue(T value) {
        back_ = std::make_unique<Node>(value, std::move(back_));
        if (size_ == 0) {
            front_ = back_.get();
        }
        ++size_;
    }

    template<typename T>
    T
    QueueListSmart<T>::dequeue() {
        verifyQueueNotEmpty();

        auto value = front_->value;
        if (front_ == back_.get()) {
            back_.reset();
            front_ = nullptr;
        } else {
            front_ = front_->prev;
            front_->next.reset();
        }
        --size_;

        return value;
    }

    template<typename T>
    const T
    &QueueListSmart<T>::front() {
        verifyQueueNotEmpty();
        return front_->value;
    }

    template<typename T>
    bool
    QueueListSmart<T>::isEmpty() const {
        return size_ == 0;
    }

    template<typename T>
    size_t
    QueueListSmart<T>::size() const {
        return size_;
    }

    template<typename T>
    QueueListSmart<T>::~QueueListSmart() {
    }

    template<typename T>
    void
    QueueListSmart<T>::verifyQueueNotEmpty() {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }
}  // namespace mdn

#endif  // MDN_QUEUE_LIST_SMART_HPP
