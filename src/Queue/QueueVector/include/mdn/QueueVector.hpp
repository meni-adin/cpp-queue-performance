
#ifndef MDN_QUEUE_VECTOR_HPP
#define MDN_QUEUE_VECTOR_HPP

#include <algorithm>
#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueVector : public mdn::Queue<T> {
    public:
        QueueVector();

        QueueVector(const QueueVector &other) = default;

        QueueVector(QueueVector &&other) noexcept = default;

        ~QueueVector() override = default;

        QueueVector &
        operator=(const QueueVector &other);

        QueueVector &
        operator=(QueueVector &&other) noexcept;

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
        swap(QueueVector &other) noexcept;

    private:
        static inline constexpr size_t initialVecSize_{2};
        static inline constexpr size_t vecGrowthFactor_{2};
        std::vector<T>                 vec_;
        size_t
            size_{},
            frontIdx_{0},
            backIdx_{0};

        void
        verifyQueueNotEmpty() const;

        void
        resizeIfRequired();

        void
        promoteIndices();
    };

    template<typename T>
    QueueVector<T>::QueueVector() {
        vec_.resize(initialVecSize_);
    }

    template<typename T>
    QueueVector<T> &
    QueueVector<T>::operator=(const QueueVector &other) {  // NOLINT(bugprone-unhandled-self-assignment, cert-oop54-cpp)
        QueueVector temp(other);
        swap(temp);
        return *this;
    }

    template<typename T>
    QueueVector<T> &
    QueueVector<T>::operator=(QueueVector &&other) noexcept {
        swap(other);
        return *this;
    }

    template<typename T>
    void
    QueueVector<T>::enqueue(const T &value) {
        resizeIfRequired();
        vec_[backIdx_] = value;
        promoteIndices();
    }

    template<typename T>
    void
    QueueVector<T>::enqueue(T &&value) {
        resizeIfRequired();
        vec_[backIdx_] = std::move(value);
        promoteIndices();
    }

    template<typename T>
    T
    QueueVector<T>::dequeue() {
        verifyQueueNotEmpty();

        auto value      = vec_[frontIdx_];
        vec_[frontIdx_] = T{};
        frontIdx_       = (frontIdx_ + 1) % vec_.size();
        --size_;

        return value;
    }

    template<typename T>
    const T &
    QueueVector<T>::front() const {
        verifyQueueNotEmpty();
        return vec_[frontIdx_];
    }

    template<typename T>
    bool
    QueueVector<T>::isEmpty() const {
        return size_ == 0;
    }

    template<typename T>
    size_t
    QueueVector<T>::size() const {
        return size_;
    }

    template<typename T>
    void
    QueueVector<T>::clear() {
        vec_.resize(initialVecSize_);
        for (auto &elem : vec_) {
            elem = T{};
        }
        size_ = frontIdx_ = backIdx_ = 0;
    }

    template<typename T>
    void
    QueueVector<T>::swap(QueueVector &other) noexcept {
        using std::swap;
        swap(vec_, other.vec_);
        swap(size_, other.size_);
        swap(frontIdx_, other.frontIdx_);
        swap(backIdx_, other.backIdx_);
    }

    template<typename T>
    void
    QueueVector<T>::verifyQueueNotEmpty() const {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }

    template<typename T>
    void
    QueueVector<T>::resizeIfRequired() {
        if (size_ == vec_.size()) {
            vec_.resize(vec_.size() * vecGrowthFactor_);
            for (size_t idx = 0; idx < backIdx_; ++idx) {
                vec_[size_ + idx] = vec_[idx];
                vec_[idx]         = T{};
            }
            backIdx_ += size_;
        }
    }

    template<typename T>
    void
    QueueVector<T>::promoteIndices() {
        backIdx_ = (backIdx_ + 1) % vec_.size();
        ++size_;
    }
}  // namespace mdn

#endif  // MDN_QUEUE_VECTOR_HPP
