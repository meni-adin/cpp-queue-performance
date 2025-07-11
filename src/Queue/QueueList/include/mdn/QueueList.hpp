
#ifndef MDN_QUEUE_LIST_HPP
#define MDN_QUEUE_LIST_HPP

#include <iostream>

#include "mdn/Queue.hpp"

namespace mdn {

    template<typename T>
    class QueueList : public mdn::Queue<T> {
    public:

        void
        enqueue(T value) override;

        T
        dequeue();

        bool
        isEmpty() const override;

        size_t
        size() const override;

        ~QueueList() override;

    private:
        struct Node {
        public:
            T                     value;
            std::unique_ptr<Node> next;

            Node(const T &value, std::unique_ptr<Node> next) :
                value(value), next(std::move(next)) {
            }
        };

        using pNode = Node *;

        std::unique_ptr<Node> listHead_;
        pNode
            first_,
            second_;
        size_t size_{};
    };

    template<typename T>
    void
    QueueList<T>::enqueue(T value) {
        auto newNode = std::make_unique<Node>(value, std::move(listHead_));
        listHead_    = std::move(newNode);
        ++size_;
    }

    // template<typename T>
    // T
    // dequeue();

    template<typename T>
    bool
    QueueList<T>::isEmpty() const {
        return size_ == 0;
    }

    template<typename T>
    size_t
    QueueList<T>::size() const {
        return size_;
    }

    template<typename T>
    QueueList<T>::~QueueList() {
    }

}  // namespace mdn

#endif  // MDN_QUEUE_LIST_HPP
