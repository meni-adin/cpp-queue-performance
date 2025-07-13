
#ifndef MDN_QUEUE_VECTOR_HPP
#define MDN_QUEUE_VECTOR_HPP

#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueVector : public mdn::Queue<T> {
    public:

        QueueVector();

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

        ~QueueVector() override;

    private:
        static inline constexpr size_t initialVecSize_{2};
        static inline constexpr size_t vecGrowthFactor_{2};
        std::vector<T>                 vec_;
        size_t
            size_{},
            frontIdx_{0},
            backIdx_{0};

        void
        verifyQueueNotEmpty();
    };

    template<typename T>
    QueueVector<T>::QueueVector() {
        vec_.resize(initialVecSize_);
    }

    template<typename T>
    void
    QueueVector<T>::enqueue(T value) {
        if (size_ == vec_.size()) {
            vec_.resize(vec_.size() * vecGrowthFactor_);
            for (size_t idx = 0; idx < backIdx_; ++idx) {
                vec_[size_ + idx] = vec_[idx];
                vec_[idx]         = T{};
            }
            backIdx_ += size_;
        }
        vec_[backIdx_] = value;
        backIdx_       = (backIdx_ + 1) % vec_.size();
        ++size_;
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
    const T
    &QueueVector<T>::front() {
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
    QueueVector<T>::~QueueVector() {
    }

    template<typename T>
    void
    QueueVector<T>::verifyQueueNotEmpty() {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
    }
}  // namespace mdn

#endif  // MDN_QUEUE_VECTOR_HPP
