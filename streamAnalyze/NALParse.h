#ifndef PALPARSE_H
#define PALPARSE_H

#include "NALUnit.h"
#include "CommonDef.h"
#include "TreeList.hpp"
#include "bsDef.hpp"
#include "h264_stream.h"
#include <vector>
const int OUTPUT_SIZE = 512 * 1024;
using std::vector;
struct ParameterDescription {
    std::vector<std::pair<std::string, BIT(int)>> m_pareterDesciption;
};



class NALParser
{
    using NALTreeNode = std::shared_ptr <TreeNode<ParameterDescription>>;
public:
    NALParser(STREAM_TYPE streamType);
    ~NALParser();
    int release(void);
    int parseNALU(const NALUnit& vNal);
    std::shared_ptr<TreeList<ParameterDescription>> getRootTree()const { return m_root; }
    void getVideoInfo(videoinfo_t* videoInfo);
  
private:
    void h264debugNALU(h264_stream_t* h, nal_t* nal);
    void h264debugSliceHeader(h264_stream_t* h, NALTreeNode& root);
    void h264debugSPS(sps_t* sps, NALTreeNode& root);
    void h264debugPPS(pps_t* pps, NALTreeNode& root);
    void h264debugAUD(aud_t* aud, NALTreeNode& root);
    void h264debugSEI(h264_stream_t* h, NALTreeNode& root);

private:
    char m_tmpStore[1024];
    char m_outputInfo[OUTPUT_SIZE];

private:
    h264_stream_t* m_hH264;
    STREAM_TYPE m_streamType;
    std::shared_ptr<TreeList<ParameterDescription>> m_root;
};
#endif
