
#ifndef MDN_QUEUE_HPP
#define MDN_QUEUE_HPP

namespace mdn {

class Queue {
public:

    virtual void hello() = 0;

    virtual ~Queue() = 0;
};

}  // namespace mdn

namespace mdn {
inline Queue::~Queue() = default;
}  // namespace mdn

#endif  // MDN_QUEUE_HPP
