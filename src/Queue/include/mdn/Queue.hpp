
#ifndef MDN_QUEUE_HPP
#define MDN_QUEUE_HPP

namespace mdn {

    template<typename T>
    class Queue {
    public:

        Queue() = default;

        Queue(const Queue &) = default;

        Queue(Queue &&) = default;

        Queue &
        operator=(const Queue &other) = delete;

        Queue &
        operator=(const Queue &&other) = delete;

        virtual ~Queue() = default;

        virtual void
        enqueue(const T &value) = 0;

        virtual void
        enqueue(T &&value) = 0;

        virtual T
        dequeue() = 0;

        virtual const T &
        front() const = 0;

        [[nodiscard]]
        virtual bool
        isEmpty() const = 0;

        [[nodiscard]]
        virtual size_t
        size() const = 0;

        virtual void
        clear() = 0;
    };

}  // namespace mdn

#endif  // MDN_QUEUE_HPP
