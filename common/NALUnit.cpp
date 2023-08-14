#include "NALUnit.h"
#include <assert.h>
#include <algorithm>
#include<iomanip>

static const char* const h264_nal_type_name[32] = {
    "Unspecified 0", //H264_NAL_UNSPECIFIED
    "Coded slice of a non-IDR picture", // H264_NAL_SLICE
    "Coded slice data partition A", // H264_NAL_DPA
    "Coded slice data partition B", // H264_NAL_DPB
    "Coded slice data partition C", // H264_NAL_DPC
    "IDR", // H264_NAL_IDR_SLICE
    "SEI", // H264_NAL_SEI
    "SPS", // H264_NAL_SPS
    "PPS", // H264_NAL_PPS
    "AUD", // H264_NAL_AUD
    "End of sequence", // H264_NAL_END_SEQUENCE
    "End of stream", // H264_NAL_END_STREAM
    "Filler data", // H264_NAL_FILLER_DATA
    "SPS extension", // H264_NAL_SPS_EXT
    "Prefix", // H264_NAL_PREFIX
    "Subset SPS", // H264_NAL_SUB_SPS
    "Depth parameter set", // H264_NAL_DPS
    "Reserved 17", // H264_NAL_RESERVED17
    "Reserved 18", // H264_NAL_RESERVED18
    "Auxiliary coded picture without partitioning", // H264_NAL_AUXILIARY_SLICE
    "Slice extension", // H264_NAL_EXTEN_SLICE
    "Slice extension for a depth view or a 3D-AVC texture view", // H264_NAL_DEPTH_EXTEN_SLICE
    "Reserved 22", // H264_NAL_RESERVED22
    "Reserved 23", // H264_NAL_RESERVED23
    "Unspecified 24", // H264_NAL_UNSPECIFIED24
    "Unspecified 25", // H264_NAL_UNSPECIFIED25
    "Unspecified 26", // H264_NAL_UNSPECIFIED26
    "Unspecified 27", // H264_NAL_UNSPECIFIED27
    "Unspecified 28", // H264_NAL_UNSPECIFIED28
    "Unspecified 29", // H264_NAL_UNSPECIFIED29
    "Unspecified 30", // H264_NAL_UNSPECIFIED30
    "Unspecified 31", // H264_NAL_UNSPECIFIED31
};


NALUnit::NALUnit(const std::vector<uint8_t>& data, uint8_t startCodeSize, STREAM_TYPE streamType)
{
    init(data.data(),data.size(),startCodeSize, streamType);
}

NALUnit::NALUnit(const uint8_t* data, size_t size, uint8_t startCodeSize , STREAM_TYPE streamType)
{
    init(data, size, startCodeSize, streamType);
}

NALUnit::NALUnit()
{
}

void NALUnit::init(const uint8_t* data, size_t size, uint8_t startCodeSize, STREAM_TYPE streamType)
{
    m_nalUnitData.resize(size,0);
    memcpy(m_nalUnitData.data(), data, size);
    if (startCodeSize == 3) {
        m_startCode = { 0,0,1 };
    }
    else {
        m_startCode = { 0,0,0,1 };
    }
    m_streamType = streamType;
    if (m_streamType == STREAM_TYPE::STREAM_H264) {
        m_nalType = m_nalUnitData[0] & 0x1f;
    }
    else {
        m_nalType = (m_nalUnitData[0] >> 1) & 0x3f; // 6 bit
    }
}

std::string NALUnit::getNALUTypeName() const
{
    return h264_nal_type_name[m_nalType];
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
