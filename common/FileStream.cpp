#include "FileStream.h"
#include <algorithm>
#define MAX_NAL_SIZE 100*1024*1024
FileStream::FileStream(const std::string& fileName)
{
    if(m_fp.is_open()){
       m_fp.close();
    }
    m_fp.open(fileName,std::ios::in | std::ios::binary);
    m_fileName =  fileName;
    m_strType = getStreamType();
}
//读取单个NALUnit
NALUnit FileStream::getNextNALUnit()
{
    std::vector<uint8_t> buffer;
    uint8_t startCode = 4;
    //0001 xxxxx 0001 两个间隔就是NALUnit
    if (!getNALUnitHead(buffer,startCode)) {
        return {};
    }
    buffer.clear();
    uint8_t preStartCode = startCode;
    //find next head
    if (!getNALUnitHead(buffer,startCode)) {
        return {};
    }
    m_fp.seekg(-startCode,std::ios::cur);
    for (auto i = 0; i < startCode; i++) {
        buffer.pop_back();
    }
    return NALUnit(buffer, preStartCode, m_strType);
}

NALUnit FileStream::getFirstNALUnit()
{
    //获取当前位置
    std::streampos sp = m_fp.tellg();
    //seek 到开头
    m_fp.seekg(0,std::ios::beg);
    auto nalUnit = getNextNALUnit();
    //恢复到获取之前的位置
    m_fp.seekg(sp,std::ios::beg);
    return nalUnit;
}

STREAM_TYPE FileStream::getStreamType()
{
    //todo 
    //暂时通过文件名判断，如果需要更详细的参考ffmpeg  h264_probe  hevc_probe
    auto namePos = m_fileName.find_last_of('.');
    auto strVal = m_fileName.substr(namePos + 1, m_fileName.size() - namePos);
    std::transform(strVal.begin(), strVal.end(), strVal.begin(), ::toupper);
    if(strVal == "HEVC" || strVal == "H265"){
        m_strType = STREAM_TYPE::STREAM_H265;
    }
    if (strVal == "H264") {
        m_strType = STREAM_TYPE::STREAM_H264;
        
    }
    return m_strType;

    //auto nalUnit = getFirstNALUnit();
    //auto nalType = nalUnit[0] & 0x1f;
    //if (nalType > 0 && nalType < 12) {
    //    return STREAM_TYPE::STREAM_H264;
    //}
    //nalType = (nalUnit[0] >> 1) & 0x3f; // 6 bit
    //if (nalType >= 0 && nalType <= 47) // ok
    //{
    //    return STREAM_TYPE::STREAM_H265;
    //}

    //return STREAM_TYPE::STREAM_NONE;
}

bool FileStream::getNALUnitHead(std::vector<uint8_t>& buffer, uint8_t& startCode)
{
    bool isFound = false;
    bool info3 = false;
    bool info4 = false;
    buffer.reserve(MAX_NAL_SIZE);
    //find first head
    while (!isFound && !m_fp.eof())
    {
        buffer.push_back(m_fp.get());
        info4 = findStartcode4(buffer);
        if (!info4) {
            info3 = findStartcode3(buffer);
            startCode = info3 ? 3 : startCode;
            isFound = info3;
        }
        else {
            startCode = 4;
            isFound = info4;
        }
        if (buffer.size() > MAX_NAL_SIZE) {
            return false;
        }
    }
    if (m_fp.eof()) {
        //最后一个NALUnit
        startCode = 0;
        return true;
    }
    return true;
}
