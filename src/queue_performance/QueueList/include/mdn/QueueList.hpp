
#ifndef MDN_QUEUE_LIST_HPP
#define MDN_QUEUE_LIST_HPP

#include "mdn/Queue.hpp"

namespace mdn {

class QueueList : mdn::Queue {
public:
    void hello() override;
};

}  // namespace mdn

#endif  // MDN_QUEUE_LIST_HPP
