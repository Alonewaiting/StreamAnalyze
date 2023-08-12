#include<iostream>
#include<fstream>


#include "FileStream.h"
using namespace std;
int main() {
    std::cout<<"hello world"<<std::endl;
    FileStream fs("D:/myCode/StreamAnalyze/foreman_cif.h264");
    auto type = fs.getStreamType();
    //1
    auto nal = fs.getNextNALUnit();
    std::cout << nal;
    std::cout << std::endl;
    //1
    auto nalHead = fs.getFirstNALUnit();
    std::cout << nalHead;
    std::cout << std::endl;
    //2
    nal = fs.getNextNALUnit();
    std::cout << nal;
    std::cout << std::endl;
    //1
    nalHead = fs.getFirstNALUnit();
    std::cout << nalHead;
    std::cout << std::endl;
    //3
    nal = fs.getNextNALUnit();
    std::cout << nal;
    std::cout << std::endl;
    system("pause");

}
