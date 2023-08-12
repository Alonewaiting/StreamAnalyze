#include "NALUnit.h"
#include <assert.h>
#include <algorithm>
#include<iomanip>
NALUnit::NALUnit(const std::vector<uint8_t>& data, uint8_t startCodeSize)
{
    init(data.data(),data.size(),startCodeSize);
}

NALUnit::NALUnit(const uint8_t* data, size_t size, uint8_t startCodeSize )
{
    init(data, size, startCodeSize);
}

NALUnit::NALUnit()
{
}

void NALUnit::init(const uint8_t* data, size_t size, uint8_t startCodeSize)
{
    m_nalUnitData.resize(size,0);
    memcpy(m_nalUnitData.data(), data, size);
    if (startCodeSize == 3) {
        m_startCode = { 0,0,1 };
    }
    else {
        m_startCode = { 0,0,0,1 };
    }
}

uint8_t NALUnit::operator[](const size_t i) const
{
    // TODO: 在此处插入 return 语句
    assert(i < m_nalUnitData.size());
    return m_nalUnitData[i];
}



std::ostream& operator<<(std::ostream& os, const NALUnit& nal)
{
    // TODO: 在此处插入 return 语句
    std::for_each(nal.m_nalUnitData.begin(), nal.m_nalUnitData.end(), [&os](const uint8_t& data) {
        os << std::setw(2) << std::setfill('0') << std::hex << (int)data << "  ";

        });
    return os;
}
