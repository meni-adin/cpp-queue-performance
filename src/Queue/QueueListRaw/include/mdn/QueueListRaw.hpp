
#ifndef MDN_QUEUE_LIST_RAW_HPP
#define MDN_QUEUE_LIST_RAW_HPP

#include <algorithm>
#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueListRaw : public mdn::Queue<T> {
    public:
        QueueListRaw() = default;

        QueueListRaw(const QueueListRaw &other);

        QueueListRaw(QueueListRaw &&other) noexcept;

        ~QueueListRaw() override;

        QueueListRaw &
        operator=(const QueueListRaw &other);

        QueueListRaw &
        operator=(QueueListRaw &&other) noexcept;

        void
        enqueue(const T &value) override;

        void
        enqueue(T &&value) override;

        T
        dequeue() override;

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
        swap(QueueListRaw &other) noexcept;

    private:
        struct Node {
        public:
            T     value;
            Node *next{};

            explicit Node(const T &value) :
                value(value) {
            }

            Node(const Node &) = delete;

            Node &
            operator=(const Node &) = delete;
        };

        Node  *front_{};
        Node  *back_{};
        size_t size_{};

        void
        verifyQueueNotEmpty() const;

        void
        enqueueHelper(Node *newNode);

        void
        clearHelper();
    };

    template<typename T>
    QueueListRaw<T>::QueueListRaw(const QueueListRaw &other) :
        size_(other.size_) {
        if (size_ == 0) {
            return;
        }

        auto srcNode  = other.front_;
        auto destNode = front_ = new Node(srcNode->value);
        srcNode                = srcNode->next;
        while (srcNode != nullptr) {
            destNode->next = new Node(srcNode->value);
            srcNode        = srcNode->next;
            destNode       = destNode->next;
        }
        back_ = destNode;
    }

    template<typename T>
    QueueListRaw<T>::QueueListRaw(QueueListRaw &&other) noexcept :
        front_(other.front_), back_(other.back_), size_(other.size_) {
        other.front_ = other.back_ = nullptr;
        other.size_                = 0;
    }

    template<typename T>
    QueueListRaw<T>::~QueueListRaw() {
        clearHelper();
    }

    template<typename T>
    QueueListRaw<T> &
    QueueListRaw<T>::operator=(const QueueListRaw &other) {  // NOLINT(bugprone-unhandled-self-assignment, cert-oop54-cpp)
        QueueListRaw temp(other);
        swap(temp);
        return *this;
    }

    template<typename T>
    QueueListRaw<T> &
    QueueListRaw<T>::operator=(QueueListRaw &&other) noexcept {
        swap(other);
        return *this;
    }

    template<typename T>
    void
    QueueListRaw<T>::enqueue(const T &value) {
        auto newNode = new Node(value);
        enqueueHelper(newNode);
    }

    template<typename T>
    void
    QueueListRaw<T>::enqueue(T &&value) {
        auto newNode = new Node(std::move(value));
        enqueueHelper(newNode);
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
            auto toDelete = front_;
            front_        = front_->next;
            delete toDelete;
        }
        --size_;

        return value;
    }

    template<typename T>
    const T &
    QueueListRaw<T>::front() const {
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
    void
    QueueListRaw<T>::clear() {
        clearHelper();
    }

    template<typename T>
    void
    QueueListRaw<T>::swap(QueueListRaw &other) noexcept {
        using std::swap;
        swap(front_, other.front_);
        swap(back_, other.back_);
        swap(size_, other.size_);
    }

    template<typename T>
    void
    QueueListRaw<T>::verifyQueueNotEmpty() const {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }

    template<typename T>
    void
    QueueListRaw<T>::enqueueHelper(Node *newNode) {
        if (size_ == 0) {
            front_ = back_ = newNode;
        } else {
            back_->next = newNode;
            back_       = newNode;
        }
        ++size_;
    }

    template<typename T>
    void
    QueueListRaw<T>::clearHelper() {
        while (front_ != nullptr) {
            auto toDelete = front_;
            front_        = front_->next;
            delete toDelete;
        }
        back_ = nullptr;
        size_ = 0;
    }
}  // namespace mdn

#endif  // MDN_QUEUE_LIST_RAW_HPP
