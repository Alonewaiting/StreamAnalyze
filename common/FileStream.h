#pragma once
#include <fstream>
#include "NALUnit.h"
#include "CommonDef.h"
class FileStream {
public:
    FileStream(const std::string& fileName);
    NALUnit getNextNALUnit();
    NALUnit getFirstNALUnit();
    std::string  getFileName() const { return m_fileName; }
    STREAM_TYPE getStreamType();
private:
    inline bool findStartcode3(const std::vector<uint8_t> & buffer)
    {
        if(buffer.size() < 3){
            return false;
        }
        return (buffer[buffer.size() - 3] == 0 && buffer[buffer.size() - 2] == 0 && buffer[buffer.size() - 1] == 1);
    }
    inline bool findStartcode4(const std::vector<uint8_t>& buffer)
    {
        if (buffer.size() < 4) {
            return false;
        }
        return (buffer[buffer.size() - 4] == 0 && buffer[buffer.size() - 3] == 0 && buffer[buffer.size() - 2] == 0 && buffer[buffer.size() - 1] == 1);
    }
    bool getNALUnitHead(std::vector<uint8_t>& buffer,uint8_t & startCode);
private:
   std::fstream m_fp;
   std::string m_fileName;
   STREAM_TYPE m_strType;
   bool m_reachEOF = false;


};
