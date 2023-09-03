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

static const char* const hevc_nal_type_name[64] = {
    "TRAIL_N", // HEVC_NAL_TRAIL_N
    "TRAIL_R", // HEVC_NAL_TRAIL_R
    "TSA_N", // HEVC_NAL_TSA_N
    "TSA_R", // HEVC_NAL_TSA_R
    "STSA_N", // HEVC_NAL_STSA_N
    "STSA_R", // HEVC_NAL_STSA_R
    "RADL_N", // HEVC_NAL_RADL_N
    "RADL_R", // HEVC_NAL_RADL_R
    "RASL_N", // HEVC_NAL_RASL_N
    "RASL_R", // HEVC_NAL_RASL_R
    "RSV_VCL_N10", // HEVC_NAL_VCL_N10
    "RSV_VCL_R11", // HEVC_NAL_VCL_R11
    "RSV_VCL_N12", // HEVC_NAL_VCL_N12
    "RSV_VLC_R13", // HEVC_NAL_VCL_R13
    "RSV_VCL_N14", // HEVC_NAL_VCL_N14
    "RSV_VCL_R15", // HEVC_NAL_VCL_R15
    "BLA_W_LP", // HEVC_NAL_BLA_W_LP
    "BLA_W_RADL", // HEVC_NAL_BLA_W_RADL
    "BLA_N_LP", // HEVC_NAL_BLA_N_LP
    "IDR_W_RADL", // HEVC_NAL_IDR_W_RADL
    "IDR_N_LP", // HEVC_NAL_IDR_N_LP
    "CRA_NUT", // HEVC_NAL_CRA_NUT
    "RSV_IRAP_VCL22", // HEVC_NAL_RSV_IRAP_VCL22
    "RSV_IRAP_VCL23", // HEVC_NAL_RSV_IRAP_VCL23
    "RSV_VCL24", // HEVC_NAL_RSV_VCL24
    "RSV_VCL25", // HEVC_NAL_RSV_VCL25
    "RSV_VCL26", // HEVC_NAL_RSV_VCL26
    "RSV_VCL27", // HEVC_NAL_RSV_VCL27
    "RSV_VCL28", // HEVC_NAL_RSV_VCL28
    "RSV_VCL29", // HEVC_NAL_RSV_VCL29
    "RSV_VCL30", // HEVC_NAL_RSV_VCL30
    "RSV_VCL31", // HEVC_NAL_RSV_VCL31
    "VPS", // HEVC_NAL_VPS
    "SPS", // HEVC_NAL_SPS
    "PPS", // HEVC_NAL_PPS
    "AUD", // HEVC_NAL_AUD
    "EOS_NUT", // HEVC_NAL_EOS_NUT
    "EOB_NUT", // HEVC_NAL_EOB_NUT
    "FD_NUT", // HEVC_NAL_FD_NUT
    "SEI_PREFIX", // HEVC_NAL_SEI_PREFIX
    "SEI_SUFFIX", // HEVC_NAL_SEI_SUFFIX
    "RSV_NVCL41", // HEVC_NAL_RSV_NVCL41
    "RSV_NVCL42", // HEVC_NAL_RSV_NVCL42
    "RSV_NVCL43", // HEVC_NAL_RSV_NVCL43
    "RSV_NVCL44", // HEVC_NAL_RSV_NVCL44
    "RSV_NVCL45", // HEVC_NAL_RSV_NVCL45
    "RSV_NVCL46", // HEVC_NAL_RSV_NVCL46
    "RSV_NVCL47", // HEVC_NAL_RSV_NVCL47
    "UNSPEC48", // HEVC_NAL_UNSPEC48
    "UNSPEC49", // HEVC_NAL_UNSPEC49
    "UNSPEC50", // HEVC_NAL_UNSPEC50
    "UNSPEC51", // HEVC_NAL_UNSPEC51
    "UNSPEC52", // HEVC_NAL_UNSPEC52
    "UNSPEC53", // HEVC_NAL_UNSPEC53
    "UNSPEC54", // HEVC_NAL_UNSPEC54
    "UNSPEC55", // HEVC_NAL_UNSPEC55
    "UNSPEC56", // HEVC_NAL_UNSPEC56
    "UNSPEC57", // HEVC_NAL_UNSPEC57
    "UNSPEC58", // HEVC_NAL_UNSPEC58
    "UNSPEC59", // HEVC_NAL_UNSPEC59
    "UNSPEC60", // HEVC_NAL_UNSPEC60
    "UNSPEC61", // HEVC_NAL_UNSPEC61
    "UNSPEC62", // HEVC_NAL_UNSPEC62
    "UNSPEC63", // HEVC_NAL_UNSPEC63
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
    if (size == 0) {
        m_nalUnitData.clear();
        return;
    }
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
    if (STREAM_TYPE::STREAM_H264 == m_streamType) {
        return h264_nal_type_name[m_nalType];
    }
    else if (STREAM_TYPE::STREAM_H265 == m_streamType) {
        return hevc_nal_type_name[m_nalType];
    }
    
    return {};
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
