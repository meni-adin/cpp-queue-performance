
#ifndef MDN_QUEUE_HPP
#define MDN_QUEUE_HPP

namespace mdn {

    template<typename T>
    class Queue {
    public:

        virtual void
        enqueue(T value) = 0;

        // virtual T
        // dequeue() = 0;

        // virtual const T
        // &front() = 0;

        virtual bool
        isEmpty() const = 0;

        virtual size_t
        size() const = 0;

        // virtual void
        // clear() = 0;

        virtual ~Queue() = default;
    };

}  // namespace mdn

#endif  // MDN_QUEUE_HPP
