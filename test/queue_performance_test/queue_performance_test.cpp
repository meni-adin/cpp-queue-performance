
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <ranges>

#include "mdn/Queue.hpp"
#include "mdn/QueueListRaw.hpp"
#include "mdn/QueueListSmart.hpp"
#include "mdn/QueueVector.hpp"

using namespace testing;

using QueueTypes = ::testing::Types<
    mdn::QueueListRaw<int>,
    mdn::QueueListSmart<int>,
    mdn::QueueVector<int>
    >;

template<typename T>
struct TypeListSize;

template<template<typename...> class TypesContainer, typename... Ts>
struct TypeListSize<TypesContainer<Ts...>> {
    static constexpr std::size_t value = sizeof...(Ts);
};

const std::string STRESS_TEST_SUITE_NAME = "QueueStressTest";

std::vector<std::string>
splitString(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string              token;
    size_t                   start = 0;
    size_t                   end   = s.find(delimiter);

    while (end != std::string::npos) {
        token = s.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
        end   = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));

    return tokens;
}

class TimeReporter : public ::testing::EmptyTestEventListener {
public:

    void
    OnTestEnd(const ::testing::TestInfo &test_info) override {
        auto splittedTestName = splitString(test_info.test_case_name(), '/');
        ASSERT_EQ(splittedTestName.size(), 2);
        if (splittedTestName[0] == STRESS_TEST_SUITE_NAME) {
            auto typeIdx = std::stoul(splittedTestName[1]);
            if (testsResults_.count(test_info.name())) {
                testsResults_[test_info.name()][typeIdx] = test_info.result()->elapsed_time();
            } else {
                testsResults_[test_info.name()].resize(numTypes);
                testsResults_[test_info.name()][typeIdx] = test_info.result()->elapsed_time();
            }
        }
    }

    void
    OnTestProgramEnd(const ::testing::UnitTest &unit_test) override {
        (void)unit_test;

        if (testsResults_.empty()) {
            std::cout << "\n--- No tests found for suite: '" << STRESS_TEST_SUITE_NAME << "' ---" << std::endl;
            return;
        }

        std::cout << "\n--- Time Report for Suite: '" << STRESS_TEST_SUITE_NAME << "' ---" << std::endl;
        std::cout << "Test Name\t\tTime (ms)" << std::endl;
        std::cout << "---------------------------------" << std::endl;

        for (const auto &test : testsResults_) {
            std::cout << test.first;
            if (test.first.length() < 16) {  // Heuristic for simple alignment
                std::cout << "\t\t";
            } else if (test.first.length() < 24) {
                std::cout << "\t";
            }
            std::cout << test.second[0] << std::endl;
        }
        std::cout << "---------------------------------" << std::endl;
    }

private:
    static inline constexpr std::size_t      numTypes = TypeListSize<QueueTypes>::value;
    std::map<std::string, std::vector<long>> testsResults_;
};

template<typename QueueT>
class QueueTest : public ::testing::Test {};

template<typename QueueT>
class QueueStressTest : public QueueTest<QueueT> {
protected:
    static inline constexpr int numElements = 1000;
};

TYPED_TEST_SUITE(QueueTest, QueueTypes);
TYPED_TEST_SUITE(QueueStressTest, QueueTypes);

TYPED_TEST(QueueTest, CreateAndDestroy) {
    TypeParam queue;
}

TYPED_TEST(QueueTest, IsEmpty) {
    TypeParam queue;

    ASSERT_EQ(queue.isEmpty(), true);
}

TYPED_TEST(QueueTest, SizeOfEmptyQueue) {
    TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, EnqueueSingleElement) {
    TypeParam queue;

    queue.enqueue(1);
    ASSERT_EQ(queue.size(), 1);
}

TYPED_TEST(QueueStressTest, EnqueueMultipleElements) {
    auto      start = std::chrono::high_resolution_clock::now();
    TypeParam queue;

    for (int i : std::views::iota(0, TestFixture::numElements)) {
        queue.enqueue(i);
    }
    ASSERT_EQ(queue.size(), TestFixture::numElements);

    auto                          end      = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
}

TYPED_TEST(QueueTest, DequeueEmpty) {
    TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
    ASSERT_THROW(queue.dequeue(), std::out_of_range);
    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, DequeueSingleElement) {
    TypeParam queue;

    queue.enqueue(1);
    ASSERT_EQ(queue.size(), 1);
    auto elem = queue.dequeue();
    ASSERT_EQ(elem, 1);
    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueStressTest, DequeueMultipleElements) {
    TypeParam queue;

    for (int i : std::views::iota(0, TestFixture::numElements)) {
        queue.enqueue(i);
    }
    ASSERT_EQ(queue.size(), TestFixture::numElements);

    for (int i : std::views::iota(0, TestFixture::numElements)) {
        auto elem = queue.dequeue();
        ASSERT_EQ(elem, i);
    }
    ASSERT_EQ(queue.size(), 0);
}

TYPED_TEST(QueueTest, FrontEmpty) {
    TypeParam queue;

    ASSERT_EQ(queue.size(), 0);
    ASSERT_THROW(queue.front(), std::out_of_range);
    ASSERT_EQ(queue.size(), 0);
}


TYPED_TEST(QueueTest, Front) {
    TypeParam queue;

    queue.enqueue(1);
    queue.enqueue(2);
    auto elem = queue.front();
    ASSERT_EQ(elem, 1);
    ASSERT_EQ(queue.size(), 2);
    ASSERT_EQ(elem, 1);
    ASSERT_EQ(queue.size(), 2);
}


int
main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    // ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    // listeners.Append(new TimeReporter);  // GTest takes ownership

    return RUN_ALL_TESTS();
}
