#include <gtest/gtest.h>
#include "../multiset.hpp"
#include <string>

using namespace std;

TEST(MultisetTest, CreateAtom) {
    Multiset m('a');
    EXPECT_TRUE(m.isAtom());
    EXPECT_FALSE(m.isSet());
    EXPECT_EQ(m.toString(), "a");
}

TEST(MultisetTest, CreateEmptySet) {
    Multiset m;
    EXPECT_TRUE(m.isSet());
    EXPECT_FALSE(m.isAtom());
    EXPECT_EQ(m.toString(), "{}");
}

TEST(MultisetTest, ParseSimpleSet) {
    Multiset m("{a,b,c}");
    EXPECT_TRUE(m.isSet());
    EXPECT_EQ(m.toString(), "{a, b, c}");
}

TEST(MultisetTest, ParseWithSpaces) {
    Multiset m(" { a ,  b , c } ");
    EXPECT_EQ(m.toString(), "{a, b, c}");
}

TEST(MultisetTest, ParseNested) {
    Multiset m("{a,{b,{c}}}");
    EXPECT_EQ(m.toString(), "{a, {b, {c}}}");
}

TEST(MultisetTest, ParseEmptyInside) {
    Multiset m("{a,{},b}");
    EXPECT_EQ(m.toString(), "{a, {}, b}");
}

TEST(MultisetTest, ParseDuplicates) {
    Multiset m("{a,a,b,a}");
    EXPECT_EQ(m.toString(), "{a, a, b, a}");
}

TEST(MultisetTest, ParseDigits) {
    Multiset m("{1,2,{3}}");
    EXPECT_EQ(m.toString(), "{1, 2, {3}}");
}

TEST(MultisetTest, DeepNesting) {
    Multiset m("{a,{b,{c,{d,{e}}}}}");
    EXPECT_EQ(m.toString(), "{a, {b, {c, {d, {e}}}}}");
}

TEST(MultisetTest, CopyConstructor) {
    Multiset a("{a,{b,c}}");
    Multiset b(a);
    EXPECT_EQ(a.toString(), b.toString());
}

TEST(MultisetTest, AssignmentOperator) {
    Multiset a("{a,b}");
    Multiset b("{x}");
    b = a;
    EXPECT_EQ(b.toString(), "{a, b}");
}

TEST(MultisetTest, SelfAssignment) {
    Multiset a("{a,b,c}");
    a = a;
    EXPECT_EQ(a.toString(), "{a, b, c}");
}

TEST(MultisetTest, AssignEmptySet) {
    Multiset a("{a,b,c}");
    Multiset empty("{}");
    a = empty;
    EXPECT_EQ(a.toString(), "{}");
}

TEST(MultisetTest, OnlyEmptySet) {
    Multiset a("{}");
    EXPECT_EQ(a.toString(), "{}");
}

TEST(MultisetTest, ManyEmptySets) {
    Multiset m("{},{},{}");
    Multiset w("{ {}, {}, {} }");
    EXPECT_EQ(w.toString(), "{{}, {}, {}}");
}
