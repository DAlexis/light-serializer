/*
 * serialization.hpp
 *
 *  Created on: 25 марта 2016 г.
 *      Author: dalexies
 */

#ifndef UTILS_SERIALIZATION_SERIALIZATION_HPP_
#define UTILS_SERIALIZATION_SERIALIZATION_HPP_

#include <stdexcept>
#include <string>
#include <vector>
#include <istream>
#include <cstddef>
#include <cstdint>
#include <cstring>

#define LIGHTSER_WRAP_STDEXCEPT(Name) \
    class Name : public std::Name \
    { \
    public: \
      explicit Name(const std::string& __arg) : std::Name(__arg) {} \
    }

namespace lightser
{

template<typename T>
void zerify(T& object)
{
	memset(&object, 0, sizeof(T));
}

template<typename T>
void putAndInc(void*& pointer, const T& object)
{
	memcpy(pointer, &object, sizeof(T));
	pointer = static_cast<void*>(static_cast<uint8_t*>(pointer)+sizeof(T));
}

template<typename T>
void getAndInc(const void*& pointer, T& object)
{
	memcpy(&object, pointer, sizeof(T));
	pointer = static_cast<const void*>(static_cast<const uint8_t*>(pointer)+sizeof(T));
}

class IBinarySerializable
{
public:
	virtual ~IBinarySerializable() {}
	virtual size_t size() const = 0;
	virtual void serialize(void* target) const = 0;
	virtual void deserialize(const void* source) = 0;
};

class ByteStreamWrapper
{
public:
	constexpr static uint8_t toBuffer   = 0;
	constexpr static uint8_t fromBuffer = 1;
	constexpr static uint8_t sizeCalculator = 2;

    ByteStreamWrapper(std::istream& file);
    ByteStreamWrapper(void* buffer = nullptr, uint8_t direction = toBuffer, size_t streamSize = 0);
    size_t size() const;
    void* buffer();
    bool empty() const;
    uint8_t direction() const;

    void doSerDeser(IBinarySerializable& object);
    ByteStreamWrapper& operator&(IBinarySerializable& object);
    ByteStreamWrapper& operator&(IBinarySerializable&& object);

	template<typename T>
	void doSerDeser(T& object)
	{
		switch(m_direction)
		{
		case toBuffer:
			if (m_streamSize != 0)
			{
				// We deal with fixed-size byte array
				if (m_streamSize - m_offset < sizeof(T))
                    throw ByteStreamWrapper::out_of_range("ByteStreamWrapper: Not enough space in byte stream");
				memcpy(&m_buffer[m_offset], &object, sizeof(T));
				m_offset += sizeof(T);
			} else {
				// We deal with vector
				m_spaceToPut.resize(m_offset + sizeof(T));
				memcpy(&m_spaceToPut[m_offset], &object, sizeof(T));
				m_offset += sizeof(T);
			}
			break;

		case fromBuffer:
			if (m_streamSize - m_offset < sizeof(T))
                throw ByteStreamWrapper::out_of_range("ByteStreamWrapper: Stream tail is smaller than requested object");
			memcpy(&object, &m_buffer[m_offset], sizeof(T));
			m_offset += sizeof(T);
			break;

		case sizeCalculator:
			m_offset += sizeof(T);
			break;
		}
	}

	template<typename T>
	ByteStreamWrapper& operator&(T& object)
	{
		doSerDeser(object);
		return *this;
	}

    friend std::ostream& operator<<(std::ostream& stream, const ByteStreamWrapper& obj);

    LIGHTSER_WRAP_STDEXCEPT(out_of_range);

private:

	uint8_t* m_buffer;
	uint8_t m_direction;
	size_t m_streamSize;
	size_t m_offset = 0;

	std::vector<uint8_t> m_spaceToPut;
    std::vector<uint8_t> m_fileContents;
};

std::ostream& operator<<(std::ostream& stream, const ByteStreamWrapper& obj);

class IBSWFriendly
{
public:
	virtual ~IBSWFriendly() {}
	virtual void serDeser(ByteStreamWrapper& bsw) = 0;
};

template<typename T>
class Serializer : public IBinarySerializable
{
public:
	Serializer(T& object) { throw std::runtime_error("Serialization of type does not supported"); }
	virtual size_t size() const override { return 0; }
	virtual void serialize(void* target) const override {}
	virtual void deserialize(const void* source) override {}
};

template<>
class Serializer<std::string> : public IBinarySerializable
{
public:
	Serializer(std::string& object) : m_obj(object) { }

	virtual size_t size() const override
	{
		return sizeof(size_t) + m_obj.length();
	}

	virtual void serialize(void* target) const override
	{
		size_t size = m_obj.length();
		putAndInc(target, size);
		memcpy(target, m_obj.data(), sizeof(char)*size);
	}

	virtual void deserialize(const void* source) override
	{
		size_t size = 0;
		getAndInc(source, size);
		m_obj.assign(static_cast<const char*>(source), size);
	}

private:
	std::string& m_obj;
};

template<typename T>
class Serializer<std::vector<T>> : public IBinarySerializable
{
public:
	Serializer(std::vector<T>& object) : m_obj(object) { }
	virtual size_t size() const override
	{
		return sizeof(size_t) + sizeof(T)*m_obj.size();
	}

	virtual void serialize(void* target) const override
	{
		size_t size = m_obj.size();
		putAndInc(target, size);
		memcpy(target, m_obj.data(), sizeof(T)*size);
	}

	virtual void deserialize(const void* source) override
	{
		size_t size = 0;
		getAndInc(source, size);
		const T* begin = static_cast<const T*>(source);
		m_obj.assign(begin, begin+size);
	}

private:
	std::vector<T>& m_obj;
};

}

#endif /* UTILS_SERIALIZATION_SERIALIZATION_HPP_ */
