
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <ranges>

#include "mdn/Queue.hpp"
#include "mdn/QueueListRaw.hpp"
#include "mdn/QueueListSmart.hpp"
#include "mdn/QueueVector.hpp"

// #define ENABLE_STRESS_TESTS

using namespace testing;

using QueueTypes = ::testing::Types<
    mdn::QueueListRaw<uint32_t>,
    mdn::QueueListRaw<std::string>,
    mdn::QueueListSmart<uint32_t>,
    mdn::QueueListSmart<std::string>,
    mdn::QueueVector<uint32_t>,
    mdn::QueueVector<std::string>>;

class FlexibleUint32 {
public:
    explicit FlexibleUint32(uint32_t val) :
        value_(val) {
    }

    operator std::string() const {
        return std::to_string(value_);
    }

    explicit FlexibleUint32(std::string val) :
        value_(static_cast<uint32_t>(std::stoul(val))) {
    }

    operator uint32_t() const {
        return value_;
    }

private:
    uint32_t value_;
};

template<typename QueueT>
class QueueTest : public ::testing::Test {
protected:
    static constexpr std::size_t singleElementCount    = 1;
    static constexpr std::size_t multipleElementsCount = 10;
    using ElemType                                     = decltype(std::declval<QueueT>().dequeue());

    struct TestConfig {
        std::size_t elemCount;
        bool        useMove;
    };

    template<typename QueueType>
    void
    helper_EnqueueElements(QueueType &queue, const TestConfig &testConfig) {
        for (uint32_t i = 0; i < testConfig.elemCount; ++i) {
            ElemType value = FlexibleUint32(i);
            ASSERT_EQ(queue.size(), i);
            if (testConfig.useMove) {
                queue.enqueue(std::move(value));
                value = static_cast<ElemType>(FlexibleUint32(i));
            } else {
                queue.enqueue(value);
            }
            ASSERT_EQ(queue.front(), static_cast<ElemType>(FlexibleUint32(0)));
        }
        ASSERT_EQ(queue.size(), testConfig.elemCount);
    }

    template<typename QueueType>
    void
    helper_DequeueElements(QueueType &queue, const TestConfig &testConfig) {
        for (uint32_t i = 0; i < testConfig.elemCount; ++i) {
            ElemType value = FlexibleUint32(i);
            ASSERT_EQ(queue.size(), testConfig.elemCount - i);
            ASSERT_EQ(queue.front(), value);
            ASSERT_EQ(queue.dequeue(), value);
        }
        ASSERT_EQ(queue.size(), 0);
    }

    template<typename QueueType>
    void
    helper_EnqueueDequeueElements(QueueType &queue, const TestConfig &testConfig) {
        ASSERT_NO_FATAL_FAILURE(helper_EnqueueElements(queue, testConfig));
        ASSERT_NO_FATAL_FAILURE(helper_DequeueElements(queue, testConfig));
    }
};

TYPED_TEST_SUITE(QueueTest, QueueTypes);

TYPED_TEST(QueueTest, CreateAndDestroy) {
    const TypeParam queue;
}

TYPED_TEST(QueueTest, IsEmpty) {
    const TypeParam queue;

    ASSERT_EQ(queue.isEmpty(), true);
}

TYPED_TEST(QueueTest, SizeOfEmptyQueue) {
    const TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, EnqueueSingleElementCopy) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::singleElementCount,
        .useMove   = false,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, EnqueueSingleElementMove) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::singleElementCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, EnqueueMultipleElementsCopy) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = false,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, EnqueueMultipleElementsMove) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, DequeueEmpty) {
    TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
    ASSERT_THROW(queue.dequeue(), std::out_of_range);
    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, EnqueueDequeueSingleElementCopy) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::singleElementCount,
        .useMove   = false,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueDequeueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, EnqueueDequeueSingleElementMove) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::singleElementCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueDequeueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, EnqueueDequeueMultipleElementsCopy) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = false,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueDequeueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, EnqueueDequeueMultipleElementsMove) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueDequeueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, FrontEmpty) {
    TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
    ASSERT_THROW(queue.front(), std::out_of_range);
    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, ClearEmpty) {
    TypeParam queue;

    queue.clear();
    ASSERT_EQ(queue.size(), 0);
    ASSERT_THROW(queue.front(), std::out_of_range);
}

TYPED_TEST(QueueTest, Clear) {
    TypeParam queue;

    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));

    queue.clear();
    ASSERT_EQ(queue.size(), 0);
    ASSERT_THROW(queue.front(), std::out_of_range);
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueDequeueElements(queue, testConfig));
}

TYPED_TEST(QueueTest, CopyConstructor) {
    TypeParam queue;

    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));

    TypeParam queueCopy(queue);
    queue.clear();
    ASSERT_EQ(queue.size(), 0);
    ASSERT_EQ(queueCopy.size(), testConfig.elemCount);

    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_DequeueElements(queueCopy, testConfig));
}

TYPED_TEST(QueueTest, MoveConstructor) {
    TypeParam queue;

    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));

    TypeParam queueCopy(std::move(queue));  // need to make queue go out of scope
    ASSERT_EQ(queueCopy.size(), testConfig.elemCount);

    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_DequeueElements(queueCopy, testConfig));
}

TYPED_TEST(QueueTest, CopyAssignmentOperator) {
    TypeParam queue;
    TypeParam queueCopy;

    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));

    queueCopy = queue;
    queue.clear();
    ASSERT_EQ(queue.size(), 0);
    ASSERT_EQ(queueCopy.size(), testConfig.elemCount);

    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_DequeueElements(queueCopy, testConfig));
}

TYPED_TEST(QueueTest, MoveAssignmentOperator) {
    TypeParam queue;
    TypeParam queueCopy;

    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::multipleElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));

    queueCopy = std::move(queue);
    ASSERT_EQ(queueCopy.size(), testConfig.elemCount);

    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_DequeueElements(queueCopy, testConfig));
}

#ifdef ENABLE_STRESS_TESTS

template<typename QueueT>
class QueueStressTest : public QueueTest<QueueT> {
protected:
    static constexpr std::size_t stressElementsCount = 10000000;
};

TYPED_TEST_SUITE(QueueStressTest, QueueTypes);

TYPED_TEST(QueueStressTest, EnqueueMultipleElements) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::stressElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueElements(queue, testConfig));
}

TYPED_TEST(QueueStressTest, DequeueMultipleElements) {
    TypeParam                        queue;
    typename TestFixture::TestConfig testConfig = {
        .elemCount = TestFixture::stressElementsCount,
        .useMove   = true,
    };
    ASSERT_NO_FATAL_FAILURE(TestFixture::helper_EnqueueDequeueElements(queue, testConfig));
}

TYPED_TEST(QueueStressTest, RandomEnqueueAndDequeue) {
    TypeParam                          queue;
    std::mt19937                       operationRng;
    std::mt19937                       moveRng;
    std::uniform_int_distribution<int> opDist(0, 1);
    uint32_t                           enqueueCount{}, dequeueCount{};

    for (uint32_t i = 0; i < TestFixture::stressElementsCount; ++i) {
        if (opDist(operationRng)) {  // Enqueue
            typename TestFixture::ElemType value = FlexibleUint32(enqueueCount);
            if (opDist(moveRng)) {
                queue.enqueue(std::move(value));
                value = static_cast<TestFixture::ElemType>(FlexibleUint32(enqueueCount));
            } else {
                queue.enqueue(value);
            }
            ++enqueueCount;
        } else {  // Dequeue
            if (queue.isEmpty()) {
                ASSERT_THROW(queue.front(), std::out_of_range);
            } else {
                typename TestFixture::ElemType value = FlexibleUint32(dequeueCount);
                ASSERT_EQ(queue.dequeue(), value);
                ++dequeueCount;
            }
        }
    }
}

#endif  // ENABLE_STRESS_TESTS

int
main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
