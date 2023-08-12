#pragma once
#include "CommonDef.h"
#include<vector>
#include<iostream>

typedef int NALType;
class NALUnit {
public:
    NALUnit(const std::vector<uint8_t>& data, uint8_t startCodeSize = 4);
    NALUnit(const uint8_t* data, size_t size, uint8_t startCodeSize = 4);
    NALUnit();
    void init(const uint8_t* data, size_t size, uint8_t startCodeSize = 4);
    std::vector<uint8_t> getNALUnit() const { return m_nalUnitData;}
    size_t getNALUnitSize() const { return m_nalUnitData.size() + m_startCode.size();}
    std::vector<uint8_t> getStartCode() const { return m_startCode; }
    size_t getStartSize() const { return m_startCode.size(); }
    STREAM_TYPE getStreamType() const { return m_streamType;}
    SLICE_TYPE getSliceType() const {return m_sliceType;}
    bool isNULL() const { return m_nalUnitData.empty(); }
    NALType getNALUType()const { return m_nalType; }
    std::string getNALUTypeName() const ;
    uint8_t operator[] (const size_t i) const;
    //使用友元函数 输出运算符重载
    friend std::ostream& operator<<(std::ostream& os, const NALUnit& nal);
private:
    std::vector<uint8_t> m_nalUnitData;
    std::vector<uint8_t> m_startCode;
    STREAM_TYPE m_streamType = STREAM_TYPE::STREAM_NONE;
    SLICE_TYPE m_sliceType = SLICE_TYPE::SLICE_N;
    NALType m_nalType = 0;
};
