/*
 * serialization.cpp
 *
 *  Created on: 30 марта 2016 г.
 *      Author: dalexies
 */

#include "serialization.hpp"
#include <iterator>
#include <streambuf>
#include <fstream>

using namespace lightser;

ByteStreamWrapper::ByteStreamWrapper(std::istream& file) :
    m_direction(fromBuffer)
{
    m_fileContents = std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
    m_streamSize = m_fileContents.size();
    m_buffer = m_fileContents.data();
}

ByteStreamWrapper::ByteStreamWrapper(void* buffer, uint8_t direction, size_t streamSize) :
    m_buffer(static_cast<uint8_t*>(buffer)),
    m_direction(direction),
    m_streamSize(streamSize)
{
    if (buffer == nullptr && direction == fromBuffer)
        throw std::logic_error("ByteStreamWrapper: Cannot read from nullptr buffer");
}

size_t ByteStreamWrapper::size() const
{
    if (m_direction == toBuffer)
        return m_buffer == nullptr ? m_spaceToPut.size() : m_offset;
    else
        return m_streamSize;
}

void* ByteStreamWrapper::buffer()
{
    return m_buffer == nullptr ? m_spaceToPut.data() : m_buffer;
}

bool ByteStreamWrapper::empty() const
{
    if (m_direction == toBuffer)
        return m_offset == 0;
    else
        return m_offset == m_streamSize; // We have readed all bytes
}

uint8_t ByteStreamWrapper::direction() const
{
    return m_direction;
}

void ByteStreamWrapper::doSerDeser(IBinarySerializable& object)
{
    size_t objSize = object.size();
    switch(m_direction)
    {
    case toBuffer:
        if (m_streamSize != 0)
        {
            // We deal with fixed-size byte array
            if (m_streamSize - m_offset < objSize)
                throw std::out_of_range("ByteStreamWrapper: Not enough space in byte stream");

            object.serialize(&m_buffer[m_offset]);
            m_offset += objSize;
        } else {
            // We deal with vector
            m_spaceToPut.resize(m_offset + objSize);
            object.serialize(&m_spaceToPut[m_offset]);
            m_offset += objSize;
        }
        break;

    case fromBuffer:
        if (m_streamSize - m_offset < objSize)
            throw std::out_of_range("ByteStreamWrapper: Stream tail is smaller than requested object");
        object.deserialize(&m_buffer[m_offset]);
        m_offset += objSize;
        break;

    case sizeCalculator:
        m_offset += objSize;
        break;
    }
}

ByteStreamWrapper& ByteStreamWrapper::operator&(IBinarySerializable& object)
{
    doSerDeser(object);
    return *this;
}

ByteStreamWrapper& ByteStreamWrapper::operator&(IBinarySerializable&& object)
{
    doSerDeser(object);
    return *this;
}

std::ostream& lightser::operator<<(std::ostream& stream, const ByteStreamWrapper& obj)
{
    stream.write(reinterpret_cast<char*>(obj.m_buffer), obj.m_offset);
    return stream;
}
