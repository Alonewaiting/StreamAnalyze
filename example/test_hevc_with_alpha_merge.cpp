#include<iostream>
#include<fstream>
#include "FileStream.h"
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


int main() {
    std::cout << "hello world" << std::endl;
    FileStream alpha("D:/testData/test/alpha.h265");
    FileStream major("D:/testData/test/major.h265");
    std::ofstream fpOut("D:/testData/test/mergex.h265", std::ios::out | std::ios::binary);
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
        fpOut.write(reinterpret_cast<char*>(alphaNALU.getStartCode().data()), alphaNALU.getStartSize());
        fpOut.write(reinterpret_cast<char*>(alphadata.data()), alphadata.size());



    }
    system("pause");

}
