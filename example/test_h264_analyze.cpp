#include<iostream>
#include<fstream>
#include<iomanip>
#include "FileStream.h"
#include "h264_stream.h"
using namespace std;
int main() {
    std::cout << "hello world" << std::endl;
    FileStream fs("D:/myCode/StreamAnalyze/foreman_cif.h264");
    if (fs.getStreamType() != STREAM_TYPE::STREAM_H264) {
        return -1;
    }
    h264_stream_t* h = h264_new();
    int i = 0; 
    while (true)
    {
       
        auto nalunit = fs.getNextNALUnit();
        if (nalunit.isNULL()) {
            break;
        }
        read_nal_unit(h, nalunit.getNALUnit().data(), nalunit.getNALUnitSize());
        switch (h->nal->nal_unit_type)
        {
        case NAL_UNIT_TYPE_SPS: {
            int constraint_byte = h->sps->constraint_set0_flag << 7;
            constraint_byte = h->sps->constraint_set1_flag << 6;
            constraint_byte = h->sps->constraint_set2_flag << 5;
            constraint_byte = h->sps->constraint_set3_flag << 4;
            constraint_byte = h->sps->constraint_set4_flag << 3;
            constraint_byte = h->sps->constraint_set4_flag << 3;
            std::cout << std::setw(2) << std::setfill('0') << "codec: avc1." << std::hex << h->sps->profile_idc << constraint_byte << h->sps->level_idc << "  ";
            debug_sps(h->sps);
        }
        }
        debug_nal(h,h->nal);
        i++;
        if(i> 10){
            break;

        }// parser 10 nalunit

    }
    
    h264_free(h);
    system("pause");

}
