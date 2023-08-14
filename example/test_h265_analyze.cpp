#include<iostream>
#include<fstream>
#include<iomanip>
#include "FileStream.h"
#include "h265_stream.h"
using namespace std;
int main() {
    std::cout << "hello world" << std::endl;
    FileStream fs("D:/myCode/StreamAnalyze/testVideo.h265");
    if (fs.getStreamType() != STREAM_TYPE::STREAM_H265) {
        return -1;
    }
    h265_stream_t* h = h265_new();
    int i = 0;
    while (true)
    {

        auto nalunit = fs.getNextNALUnit();
        if (nalunit.isNULL()) {
            break;
        }
        h265_read_nal_unit(h, nalunit.getNALUnit().data(), nalunit.getNALUnitSize());
        switch (h->nal->nal_unit_type)
        {
            case NAL_UNIT_VPS:
                std::cout << "NAL_UNIT_VPS" << std::endl;
            case NAL_UNIT_SPS:
                std::cout << "NAL_UNIT_SPS" << std::endl;
            case NAL_UNIT_PPS:
                std::cout << "NAL_UNIT_PPS" << std::endl;
            case NAL_UNIT_AUD:
                std::cout << "NAL_UNIT_AUD" << std::endl;
            case NAL_UNIT_EOS:
                std::cout << "NAL_UNIT_EOS" << std::endl;
            case NAL_UNIT_EOB:
                std::cout << "NAL_UNIT_EOB" << std::endl;
            case NAL_UNIT_FILLER_DATA:
                std::cout << "NAL_UNIT_FILLER_DATA" << std::endl;
            case NAL_UNIT_PREFIX_SEI:
                std::cout << "NAL_UNIT_PREFIX_SEI" << std::endl;
            case NAL_UNIT_SUFFIX_SEI:
                std::cout << "NAL_UNIT_SUFFIX_SEI" << std::endl;
        default:
            break;
        }

    }

    h265_free(h);
    system("pause");

}
