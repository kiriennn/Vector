#include <gtest/gtest.h>
#include "../src/vector.h"

namespace {
    template<typename T>
    void CheckEmpty(const Vector<T>& vec) {
        EXPECT_EQ(vec.Size(), 0);
        EXPECT_EQ(vec.Capacity(), 0);
        EXPECT_EQ(vec.Data(), nullptr);  
    }

    template<typename T>
    void CheckEqual(const Vector<T>& vec, const std::vector<T>& true_vec) {
        EXPECT_EQ(vec.Size(), true_vec.size());
        for (size_t i = 0; i < vec.Size(); ++i) {
            EXPECT_EQ(vec[i], true_vec[i]);
        }
    }

    template<typename T>
    void CheckEqual(const Vector<T>& vec, const Vector<T>& true_vec) {
        EXPECT_EQ(vec.Size(), true_vec.Size());
        for (size_t i = 0; i < vec.Size(); ++i) {
            EXPECT_EQ(vec[i], true_vec[i]);
        }
    }
} 

TEST(VectorTest, Constructors) {
    Vector<std::string> vec;
    CheckEmpty(vec);

    Vector<int> vec1(5);
    CheckEqual(vec1, std::vector<int>(5));

    Vector<int> vec2 = {1, 2, 3};
    CheckEqual(vec2, std::vector<int>{1, 2, 3});

    Vector<int> vec3 = vec1;
    CheckEqual(vec1, vec3);

    Vector<int> vec4 = std::move(vec1);
    CheckEqual(vec3, vec3);
    CheckEmpty(vec1);

    vec2 = vec4;
    CheckEqual(vec3, vec4);

    vec3 = std::move(vec4);
    CheckEqual(vec3, vec2);
    CheckEmpty(vec4);

    Vector<int> vec5{1, 5, 9};
    vec5[1] = 42;
    CheckEqual(vec5, std::vector<int>{1, 42, 9});
}

TEST(VectorTest, Comparison) {
    Vector<int> vec;
    Vector<int> vec1 = {1, 2, 3};
    Vector<int> vec2 = {1, 2, 3};
    Vector<int> vec3 = {1, 2, 4};
    Vector<int> vec4 = {42};

    EXPECT_TRUE(vec == vec);
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 == vec3);
    EXPECT_TRUE(vec4 > vec3);
    EXPECT_TRUE(vec < vec1);
    EXPECT_TRUE(vec1 <= vec2);
    EXPECT_TRUE(vec1 >= vec2);
    EXPECT_TRUE(vec1 != vec);
}

TEST(VectorTest, Modifiers) {
    Vector<int> vec = {1, 2};
    vec.PushBack(42);
    CheckEqual(vec, std::vector<int>{1, 2, 42});

    vec.PopBack();
    vec.PopBack();
    CheckEqual(vec, std::vector<int>{1});

    Vector<int> vec1(5);
    vec1.Reserve(10);
    EXPECT_EQ(vec1.Capacity(), 10);
    EXPECT_EQ(vec1.Size(), 5);

    Vector<int> vec2 = {1, 2, 3};
    vec2.Clear();
    CheckEqual(vec2, std::vector<int>{});

    vec2 = {1, 2, 3};

    vec.Swap(vec2);
    CheckEqual(vec, std::vector<int>{1, 2, 3});
    CheckEqual(vec2, std::vector<int>{1});
}

TEST(VectorTest, Iterators) {
    Vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    EXPECT_EQ(*it, 1);

    it = vec.end();
    EXPECT_EQ(it - vec.begin(), 3);

    const Vector<int> cvec = {1, 2, 3};
    auto it1 = cvec.begin();
    EXPECT_EQ(*it1, 1);

    it1 = cvec.end();
    EXPECT_EQ(it1 - cvec.begin(), 3);

    it = vec.begin();
    ++it;
    EXPECT_EQ(*it, 2);
    it += 1;
    EXPECT_EQ(*it, 3);
    it -= 2;
    EXPECT_EQ(*it, 1);

    int i = 0;
    for (auto element : vec) {
        EXPECT_EQ(element, vec[i]);
        ++i;
    }

    i = 0;
    for (const auto element : vec) {
        EXPECT_EQ(element, vec[i]);
        ++i;
    }
}
