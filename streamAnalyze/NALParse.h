#ifndef PALPARSE_H
#define PALPARSE_H

#include "NALUnit.h"
#include "CommonDef.h"
#include "TreeList.hpp"
#include "bsDef.hpp"
#include "h264_stream.h"
#include "h265_stream.h"
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
    int parseNALU(NALUnit& vNal);
    std::shared_ptr<TreeList<ParameterDescription>> getRootTree()const { return m_root; }
    void getVideoInfo(videoinfo_t* videoInfo);
  
private:
    void h264debugNALU(h264_stream_t* h, nal_t* nal);
    void h264debugSliceHeader(h264_stream_t* h, NALTreeNode& root);
    void h264debugSPS(sps_t* sps, NALTreeNode& root);
    void h264debugPPS(pps_t* pps, NALTreeNode& root);
    void h264debugAUD(aud_t* aud, NALTreeNode& root);
    void h264debugSEI(h264_stream_t* h, NALTreeNode& root);

    void h265debugNALU(h265_stream_t* h, h265_nal_t* nal);
    void h265DebugPtl(profile_tier_level_t* ptl, int profilePresentFlag, int max_sub_layers_minus1, NALTreeNode& root);
    void h265DebugSubLayerHrdParameters(sub_layer_hrd_parameters_t* subhrd, int sub_pic_hrd_params_present_flag, int CpbCnt, int subLayerId, const char* p, NALTreeNode& root);
    void h265DebugHrdParameters(hrd_parameters_t* hrd, int commonInfPresentFlag, int maxNumSubLayersMinus1, NALTreeNode& root);
    void h265DebugVps(h265_vps_t* vps, NALTreeNode& root);
    void h265DebugScalingList(scaling_list_data_t* sld, NALTreeNode& root);
    void h265DebugShortTermRefPicSet(h265_sps_t* sps, st_ref_pic_set_t* st, referencePictureSets_t* rps, int stRpsIdx, NALTreeNode& root);
    void h265DebugVuiParameters(vui_parameters_t* vui, int maxNumSubLayersMinus1, NALTreeNode& root);
    void h265DebugSps(h265_sps_t* sps, NALTreeNode& root);
    void h265DebugPps(h265_pps_t* pps, NALTreeNode& root);
    void h265DebugAud(h265_aud_t* aud, NALTreeNode& root);
    void h265DebugSeis(h265_stream_t* h, NALTreeNode& root);
    void h265DebugRefPicListsModification(h265_slice_header_t* hrd, NALTreeNode& root);
    void h265DebugPredWeightTable(h265_stream_t* h, NALTreeNode& root);
    void h265DebugSliceHeader(h265_stream_t* h, NALTreeNode& root);


private:
    char m_tmpStore[1024];
    char m_outputInfo[OUTPUT_SIZE];

private:
    //TODO H265 and h264 base funtion.
    h264_stream_t* m_hH264;
    h265_stream_t* m_hH265;
    STREAM_TYPE m_streamType;
    std::shared_ptr<TreeList<ParameterDescription>> m_root;
};
#endif
