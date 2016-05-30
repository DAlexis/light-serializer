/*
 * serialization-test.cpp
 *
 *  Created on: 30 марта 2016 г.
 *      Author: dalexies
 */


#include "gtest/gtest.h"

#include "serialization.hpp"
#include <string>
#include <math.h>

using namespace std;
using namespace lightser;

TEST(ByteStreamWrapper, Creation)
{
	ASSERT_NO_THROW(ByteStreamWrapper());
	ASSERT_ANY_THROW(ByteStreamWrapper(nullptr, ByteStreamWrapper::fromBuffer));
}

TEST(ByteStreamWrapper, SimpleTypesDynamicArray)
{
	ByteStreamWrapper bsw;
	int    a0 = 25,     a1 = 0;
	double b0 = 3.1415, b1 = 0;
	char   c0 = 75,     c1 = 0;

	ASSERT_NO_THROW(bsw & a0);
	ASSERT_NO_THROW(bsw & b0);
	ASSERT_NO_THROW(bsw & c0);

	ByteStreamWrapper deser(bsw.buffer(), ByteStreamWrapper::fromBuffer, bsw.size());

	ASSERT_NO_THROW(deser & a1);
	ASSERT_NO_THROW(deser & b1);
	ASSERT_NO_THROW(deser & c1);

	ASSERT_EQ(a0, a1);
	ASSERT_EQ(b0, b1);
	ASSERT_EQ(c0, c1);
}

TEST(ByteStreamWrapper, SimpleTypesStaticArray)
{
	size_t size = 20;
	uint8_t buffer[size];

	ByteStreamWrapper bsw(buffer, ByteStreamWrapper::toBuffer, size);
	int    a0 = 25,     a1 = 0;
	double b0 = 3.1415, b1 = 0;
	char   c0 = 75,     c1 = 0;

	ASSERT_NO_THROW(bsw & a0);
	ASSERT_NO_THROW(bsw & b0);
	ASSERT_NO_THROW(bsw & c0);

	ByteStreamWrapper deser(bsw.buffer(), ByteStreamWrapper::fromBuffer, bsw.size());

	ASSERT_NO_THROW(deser & a1);
	ASSERT_NO_THROW(deser & b1);
	ASSERT_NO_THROW(deser & c1);

	ASSERT_EQ(a0, a1);
	ASSERT_EQ(b0, b1);
	ASSERT_EQ(c0, c1);
}

TEST(ByteStreamWrapper, SizeTest)
{
	size_t size = 4;
	uint8_t buffer[size];
	ByteStreamWrapper bsw(buffer, ByteStreamWrapper::toBuffer, size);

	uint8_t a = 0;
	ASSERT_NO_THROW(bsw & a);
	ASSERT_NO_THROW(bsw & a);
	ASSERT_NO_THROW(bsw & a);
	ASSERT_NO_THROW(bsw & a);
	ASSERT_ANY_THROW(bsw & a);
}

TEST(ByteStreamWrapper, StringTest)
{
	ByteStreamWrapper bsw;
	string str0 = "Test text", str1;

	ASSERT_NO_THROW(bsw & Serializer<string>(str0));

	ByteStreamWrapper deser(bsw.buffer(), ByteStreamWrapper::fromBuffer, bsw.size());

	ASSERT_NO_THROW(deser & Serializer<string>(str1));

	ASSERT_EQ(str0, str1);
}

TEST(ByteStreamWrapper, VectorTest)
{
	std::vector<int> v0 = {43, 19, 55, 14};
	std::vector<int> v1 = {0, 0, 0, 0};

	ByteStreamWrapper bsw;

	ASSERT_NO_THROW(bsw & Serializer<std::vector<int>>(v0));

	ByteStreamWrapper deser(bsw.buffer(), ByteStreamWrapper::fromBuffer, bsw.size());

	ASSERT_NO_THROW(deser & Serializer<std::vector<int>>(v1));

	ASSERT_EQ(v0, v1);
}
