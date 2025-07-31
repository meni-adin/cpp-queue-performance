
#ifndef MDN_QUEUE_LIST_SMART_HPP
#define MDN_QUEUE_LIST_SMART_HPP

#include <algorithm>
#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueListSmart : public mdn::Queue<T> {
    public:
        QueueListSmart() = default;

        QueueListSmart(const QueueListSmart &other);

        QueueListSmart(QueueListSmart &&other) noexcept;

        ~QueueListSmart() override = default;

        QueueListSmart &
        operator=(const QueueListSmart &other);

        QueueListSmart &
        operator=(QueueListSmart &&other) noexcept;

        void
        enqueue(const T &value) override;

        void
        enqueue(T &&value) override;

        T
        dequeue() override;

        [[nodiscard]]
        const T &
        front() const override;

        [[nodiscard]]
        bool
        isEmpty() const override;

        [[nodiscard]]
        size_t
        size() const override;

        void
        clear() override;

        void
        swap(QueueListSmart &other) noexcept;

    private:
        struct Node {
        public:
            T                     value;
            std::unique_ptr<Node> next;

            Node(const Node &other) :
                value(other.value) {
            }

            Node(Node &&other) noexcept :
                value(std::move(other.value)) {
            }

            explicit Node(const T &value) :
                value(value) {
            }

            explicit Node(T &&value) noexcept :
                value(std::move(value)) {
            }

            Node &
            operator=(const Node &other) = delete;

            Node &
            operator=(Node &&other) = delete;

            ~Node() = default;
        };

        std::unique_ptr<Node> front_;
        Node                 *back_{};
        size_t                size_{};

        void
        verifyQueueNotEmpty() const;

        void
        enqueueHelper(std::unique_ptr<Node> newNode);
    };

    template<typename T>
    QueueListSmart<T>::QueueListSmart(const QueueListSmart &other) :
        size_(other.size_) {
        if (size_ == 0) {
            return;
        }

        auto srcNode  = other.front_.get();
        front_        = std::make_unique<Node>(*srcNode);
        auto destNode = front_.get();
        srcNode       = srcNode->next.get();
        while (srcNode != nullptr) {
            destNode->next = std::make_unique<Node>(*srcNode);
            srcNode        = srcNode->next.get();
            destNode       = destNode->next.get();
        }
        back_ = destNode;
    }

    template<typename T>
    QueueListSmart<T>::QueueListSmart(QueueListSmart &&other) noexcept :
        front_(std::move(other.front_)), back_(other.back_), size_(other.size_) {
        other.back_ = nullptr;
        other.size_ = 0;
    }

    template<typename T>
    QueueListSmart<T> &
    QueueListSmart<T>::operator=(const QueueListSmart &other) {  // NOLINT(bugprone-unhandled-self-assignment, cert-oop54-cpp)
        QueueListSmart temp(other);
        swap(temp);
        return *this;
    }

    template<typename T>
    QueueListSmart<T> &
    QueueListSmart<T>::operator=(QueueListSmart &&other) noexcept {
        swap(other);
        return *this;
    }

    template<typename T>
    void
    QueueListSmart<T>::enqueue(const T &value) {
        auto newNode = std::make_unique<Node>(value);
        enqueueHelper(std::move(newNode));
    }

    template<typename T>
    void
    QueueListSmart<T>::enqueue(T &&value) {
        auto newNode = std::make_unique<Node>(std::move(value));
        enqueueHelper(std::move(newNode));
    }

    template<typename T>
    T
    QueueListSmart<T>::dequeue() {
        verifyQueueNotEmpty();

        auto value = std::move(front_->value);
        if (front_.get() == back_) {
            front_.reset();
            back_ = nullptr;
        } else {
            front_ = std::move(front_->next);
        }
        --size_;

        return value;
    }

    template<typename T>
    const T &
    QueueListSmart<T>::front() const {
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
    void
    QueueListSmart<T>::clear() {
        front_.reset();
        back_ = nullptr;
        size_ = 0;
    }

    template<typename T>
    void
    QueueListSmart<T>::swap(QueueListSmart &other) noexcept {
        using std::swap;
        swap(front_, other.front_);
        swap(back_, other.back_);
        swap(size_, other.size_);
    }

    template<typename T>
    void
    QueueListSmart<T>::verifyQueueNotEmpty() const {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }

    template<typename T>
    void
    QueueListSmart<T>::enqueueHelper(std::unique_ptr<Node> newNode) {
        if (size_ == 0) {
            front_ = std::move(newNode);
            back_  = front_.get();
        } else {
            back_->next = std::move(newNode);
            back_       = back_->next.get();
        }
        ++size_;
    }
}  // namespace mdn

#endif  // MDN_QUEUE_LIST_SMART_HPP
