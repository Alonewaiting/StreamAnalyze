#include<iostream>
#include<fstream>
#include "FileStream.h"
#include "h265_stream.h"

#define BS_READ_F_(bitsize) \
    val = bs_read_f(b, bitsize);  \
    bs_write_f(bout, bitsize, val); 
    
#define BS_READ_U_(bitsize) \
    val = bs_read_u(b, bitsize);  \
    bs_write_u(bout, bitsize, val); 

using namespace std;
void changeNALU2Alpha(std::vector<uint8_t>& buffer) {
    //int forbidden_zero_bit;  1
    //int nal_unit_type;   6
    //int nuh_layer_id;    6
    //int nuh_temporal_id_plus1;  3

    // xxxx xxx0 0000 1xxx
    if (buffer.size() < 2) {
        return;
    }
    buffer[0] &= 0xFE; //第7位强制设置1
    buffer[1] &= 0x07; //保留后三位数字
    buffer[1] |= 0x08; //把倒数第四位变成1

}

void toRBSP(std::vector<uint8_t>& buffer) {
    std::vector<uint8_t> tempBuffer;
    for (int i = 0; i < buffer.size(); ++i) {
        if (i < 2) {
            tempBuffer.push_back(buffer[i]);
            continue;
        }
        if (buffer[i] == 0x03 && buffer[i - 1] == 0x00 && buffer[i - 2] == 0x00) {
            continue;
        }
        tempBuffer.push_back(buffer[i]);
        
    }
    buffer = tempBuffer;

}
void toSODB(std::vector<uint8_t>& buffer) {
    std::vector<uint8_t> tempBuffer;
    int count = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        if (buffer[i] == 0x00 && i > 2) {
            count++;   
        }
        else
        {
            count = 0;
        }
        tempBuffer.push_back(buffer[i]);
        if (count == 2) {
            count = 0;
            tempBuffer.push_back(0x03);
        }

    }
    buffer = tempBuffer;

}
void h265ReadPtl(profile_tier_level_t* ptl, bs_t* b, bs_t* bout, int profilePresentFlag, int max_sub_layers_minus1) {
    int i = 0;
    if (profilePresentFlag)
    {
        ptl->general_profile_space = bs_read_u(b, 2);
        ptl->general_tier_flag = bs_read_u1(b);
        ptl->general_profile_idc = bs_read_u(b, 5);
        for (i = 0; i < 32; i++)
        {
            ptl->general_profile_compatibility_flag[i] = bs_read_u1(b);
        }
        ptl->general_progressive_source_flag = bs_read_u1(b);
        ptl->general_interlaced_source_flag = bs_read_u1(b);
        ptl->general_non_packed_constraint_flag = bs_read_u1(b);
        ptl->general_frame_only_constraint_flag = bs_read_u1(b);
        if (ptl->general_profile_idc == 4 || ptl->general_profile_compatibility_flag[4] ||
            ptl->general_profile_idc == 5 || ptl->general_profile_compatibility_flag[5] ||
            ptl->general_profile_idc == 6 || ptl->general_profile_compatibility_flag[6] ||
            ptl->general_profile_idc == 7 || ptl->general_profile_compatibility_flag[7])
        {
            ptl->general_max_12bit_constraint_flag = bs_read_u1(b);
            ptl->general_max_10bit_constraint_flag = bs_read_u1(b);
            ptl->general_max_8bit_constraint_flag = bs_read_u1(b);
            ptl->general_max_422chroma_constraint_flag = bs_read_u1(b);
            ptl->general_max_420chroma_constraint_flag = bs_read_u1(b);
            ptl->general_max_monochrome_constraint_flag = bs_read_u1(b);
            ptl->general_intra_constraint_flag = bs_read_u1(b);
            ptl->general_one_picture_only_constraint_flag = bs_read_u1(b);
            ptl->general_lower_bit_rate_constraint_flag = bs_read_u1(b);
            uint64_t tmp1 = bs_read_u(b, 32);
            uint64_t tmp2 = bs_read_u(b, 2);
            ptl->general_reserved_zero_34bits = tmp1 + tmp2;
        }
        else
        {
            uint64_t tmp1 = bs_read_u(b, 32);
            uint64_t tmp2 = bs_read_u(b, 11);
            ptl->general_reserved_zero_43bits = tmp1 + tmp2;
        }
        if ((ptl->general_profile_idc >= 1 && ptl->general_profile_idc <= 5) ||
            ptl->general_profile_compatibility_flag[1] || ptl->general_profile_compatibility_flag[2] ||
            ptl->general_profile_compatibility_flag[3] || ptl->general_profile_compatibility_flag[4] ||
            ptl->general_profile_compatibility_flag[5])
        {
            ptl->general_inbld_flag = bs_read_u1(b);
        }
        else
        {
            ptl->general_reserved_zero_bit = bs_read_u1(b);
        }
    }
    ptl->general_level_idc = bs_read_u8(b);
    ptl->sub_layer_profile_present_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_level_present_flag.resize(max_sub_layers_minus1);
    for (i = 0; i < max_sub_layers_minus1; i++)
    {
        ptl->sub_layer_profile_present_flag[i] = bs_read_u1(b);
        ptl->sub_layer_level_present_flag[i] = bs_read_u1(b);
    }
    if (max_sub_layers_minus1 > 0)
    {
        for (i = max_sub_layers_minus1; i < 8; i++)
        {
            ptl->reserved_zero_2bits[i] = bs_read_u(b, 2);
        }
    }
    ptl->sub_layer_profile_space.resize(max_sub_layers_minus1);
    ptl->sub_layer_tier_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_profile_idc.resize(max_sub_layers_minus1);
    ptl->sub_layer_profile_compatibility_flag.resize(max_sub_layers_minus1);
    for (int j = 0; j < max_sub_layers_minus1; j++)
    {
        ptl->sub_layer_profile_compatibility_flag[j].resize(32);
    }
    ptl->sub_layer_progressive_source_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_interlaced_source_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_non_packed_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_frame_only_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_max_12bit_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_max_10bit_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_max_8bit_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_max_422chroma_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_max_420chroma_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_max_monochrome_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_intra_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_one_picture_only_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_lower_bit_rate_constraint_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_reserved_zero_34bits.resize(max_sub_layers_minus1);
    ptl->sub_layer_reserved_zero_43bits.resize(max_sub_layers_minus1);
    ptl->sub_layer_inbld_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_reserved_zero_bit.resize(max_sub_layers_minus1);
    ptl->sub_layer_level_idc.resize(max_sub_layers_minus1);
    for (i = 0; i < max_sub_layers_minus1; i++)
    {
        if (ptl->sub_layer_profile_present_flag[i])
        {
            ptl->sub_layer_profile_space[i] = bs_read_u(b, 2);
            ptl->sub_layer_tier_flag[i] = bs_read_u1(b);
            ptl->sub_layer_profile_idc[i] = bs_read_u(b, 5);
            for (int j = 0; j < 32; j++)
            {
                ptl->sub_layer_profile_compatibility_flag[i][j] = bs_read_u1(b);
            }
            ptl->sub_layer_progressive_source_flag[i] = bs_read_u1(b);
            ptl->sub_layer_interlaced_source_flag[i] = bs_read_u1(b);
            ptl->sub_layer_non_packed_constraint_flag[i] = bs_read_u1(b);
            ptl->sub_layer_frame_only_constraint_flag[i] = bs_read_u1(b);
            if (ptl->sub_layer_profile_idc[i] == 4 || ptl->sub_layer_profile_compatibility_flag[i][4] ||
                ptl->sub_layer_profile_idc[i] == 5 || ptl->sub_layer_profile_compatibility_flag[i][5] ||
                ptl->sub_layer_profile_idc[i] == 6 || ptl->sub_layer_profile_compatibility_flag[i][6] ||
                ptl->sub_layer_profile_idc[i] == 7 || ptl->sub_layer_profile_compatibility_flag[i][7])
            {
                ptl->sub_layer_max_12bit_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_10bit_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_8bit_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_422chroma_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_420chroma_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_max_monochrome_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_intra_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_one_picture_only_constraint_flag[i] = bs_read_u1(b);
                ptl->sub_layer_lower_bit_rate_constraint_flag[i] = bs_read_u1(b);
                uint64_t tmp1 = bs_read_u(b, 32);
                uint64_t tmp2 = bs_read_u(b, 2);
                ptl->sub_layer_reserved_zero_34bits[i] = tmp1 + tmp2;
            }
            else
            {
                uint64_t tmp1 = bs_read_u(b, 32);
                uint64_t tmp2 = bs_read_u(b, 12);
                ptl->sub_layer_reserved_zero_43bits[i] = tmp1 + tmp2;
            }
            // to check
            if ((ptl->sub_layer_profile_idc[i] >= 1 && ptl->sub_layer_profile_idc[i] <= 5) ||
                ptl->sub_layer_profile_compatibility_flag[i][1] ||
                ptl->sub_layer_profile_compatibility_flag[i][2] ||
                ptl->sub_layer_profile_compatibility_flag[i][3] ||
                ptl->sub_layer_profile_compatibility_flag[i][4] ||
                ptl->sub_layer_profile_compatibility_flag[i][5])
            {
                ptl->sub_layer_inbld_flag[i] = bs_read_u1(b);
            }
            else
            {
                ptl->sub_layer_reserved_zero_bit[i] = bs_read_u1(b);
            }
        }
        if (ptl->sub_layer_level_present_flag[i])
        {
            ptl->sub_layer_level_idc[i] = bs_read_u8(b);
        }
    }
}


void changeNALUSPS(std::vector<uint8_t>& buffer) {
  
   
    toRBSP(buffer);
    std::vector<uint8_t> bufferOut;
    bufferOut.resize(buffer.size()* 2, 0);
    //skip header
    bs_t* bout = bs_new(bufferOut.data(), bufferOut.size());
    bs_t* b = bs_new(buffer.data(), buffer.size());
    int sps_max_sub_layers_minus1 = 0;
    int sps_seq_parameter_set_id = 0;
    int val = 0;
    BS_READ_F_(1);
    BS_READ_U_(6);
    BS_READ_U_(6);
    BS_READ_U_(3);
 
    BS_READ_U_(4);
    sps_max_sub_layers_minus1 = bs_read_u(b, 3); //sps_max_sub_layers_minus1
    BS_READ_U_(1);

    profile_tier_level_t profile_tier_level;
    memset(&profile_tier_level, '\0', sizeof(profile_tier_level_t));
    h265ReadPtl(&profile_tier_level, b, bout, 1 , sps_max_sub_layers_minus1);

    int pos1 = bs_pos(b);
    int left1 = b->bits_left;
    sps_seq_parameter_set_id = bs_read_ue(b);
    int pos2 = bs_pos(b);
    int val = 1;
    int left2 = b->bits_left;
    auto valxx = buffer[pos1];
    buffer[pos1] |= 1;
    valxx = buffer[pos1];

    toSODB(buffer);
}
void changeNALUPPS(std::vector<uint8_t>& buffer) {
    toRBSP(buffer);

    toSODB(buffer);
}
void changeNALUVPS(std::vector<uint8_t>& buffer) {
    toRBSP(buffer);

    toSODB(buffer);
}

int main() {
    std::cout << "hello world" << std::endl;
    FileStream alpha("D:/testData/test/alpha.h265");
    FileStream major("D:/testData/test/major.h265");
    std::ofstream fpOut("D:/testData/test/mergex.h265", std::ios::out | std::ios::binary);
  



    int i = 0;
    while (true)
    {
        auto majorNALU = major.getNextNALUnit();
        if (majorNALU.isNULL()) {
            break;
        }
        fpOut.write(reinterpret_cast<char*>(majorNALU.getStartCode().data()), majorNALU.getStartSize());
        fpOut.write(reinterpret_cast<char*>(majorNALU.getNALUnit().data()), majorNALU.getNALUnitSize());
        auto alphaNALU = alpha.getNextNALUnit();
        if (alphaNALU.isNULL()) {
            break;
        }
        auto alphadata = alphaNALU.getNALUnit();
        changeNALU2Alpha(alphadata);
        switch (alphaNALU.getNALUType())
        {
        case NAL_UNIT_VPS:
            changeNALUVPS(alphadata);
            break;
        case NAL_UNIT_PPS:
            changeNALUPPS(alphadata);
            break;
        case NAL_UNIT_SPS:
            changeNALUSPS(alphadata);
            break;
        default:
            break;
        }
        fpOut.write(reinterpret_cast<char*>(alphaNALU.getStartCode().data()), alphaNALU.getStartSize());
        fpOut.write(reinterpret_cast<char*>(alphadata.data()), alphadata.size());
        if (i++ > 100) {
            break;
        }


    }

}
