#include "NALParse.h"
#define my_printf(...) do { \
    sprintf(m_outputInfo, __VA_ARGS__);\
    } while(0)

#define my_printf_flag(buffer, value) do { \
    sprintf(m_tmpStore, "%s: %d  [%s] (1 bit)", buffer,value,value?"True":"False");\
    sprintf(m_outputInfo, m_tmpStore);\
    } while(0)

#define my_printf_flag2(buffer, idx, value) do { \
    sprintf(m_tmpStore, "%s[%d]: %d  [%s] (1 bit)", buffer,idx, value,value?"True":"False");\
    sprintf(m_outputInfo, m_tmpStore);\
    } while(0)

#define my_printf_flag3(buffer, idx1, idx2, value) do { \
    sprintf(m_tmpStore, "%s[%d][%d]: %d  [%s] (1 bit)", buffer,idx1,idx2,value,value?"True":"False");\
    sprintf(m_outputInfo, m_tmpStore);\
    } while(0)

// 树形项
#define AddTreeItem(para,_item) \
   _item->m_element.m_pareterDesciption.push_back({m_outputInfo,para });


NALParser::NALParser(STREAM_TYPE streamType) {
    //just h264
    m_streamType = streamType;
    m_hH264 = h264_new();
    
}
NALParser::~NALParser() {
    release();
}
int NALParser::release(void) {
    if (m_hH264) {
        h264_free(m_hH264);
    }
    return 0;
}
int NALParser::parseNALU(const NALUnit& vNal) {
    read_nal_unit(m_hH264, vNal.getNALUnit().data(), vNal.getNALUnitSize());
    h264debugNALU(m_hH264,m_hH264->nal);
    return 0;
}

void NALParser::getVideoInfo(videoinfo_t* videoInfo)
{
    if (m_streamType == STREAM_TYPE::STREAM_H264) {
       memcpy(videoInfo, m_hH264->info, sizeof(videoinfo_t));
    }
    else if (m_streamType == STREAM_TYPE::STREAM_H265) {

    }
}

void NALParser::h264debugNALU(h264_stream_t* h, nal_t* nal)
{
    my_printf("NAL");
    this->m_root = std::make_shared<TreeList<ParameterDescription>>("NAL");
    auto rootNode = this->m_root->getRoot();

    my_printf("forbidden_zero_bit: %d (1 bit)", nal->forbidden_zero_bit.get()); AddTreeItem(nal->forbidden_zero_bit, rootNode);
    my_printf("nal_ref_idc: %d (2 bits)", nal->nal_ref_idc.get()); AddTreeItem(nal->nal_ref_idc, rootNode);
    // TODO make into subroutine
    const char* nal_unit_type_name;
    switch (nal->nal_unit_type.get())
    {
    case  NAL_UNIT_TYPE_UNSPECIFIED:                   nal_unit_type_name = "Unspecified"; break;
    case  NAL_UNIT_TYPE_CODED_SLICE_NON_IDR:           nal_unit_type_name = "Coded slice of a non-IDR picture"; break;
    case  NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A:  nal_unit_type_name = "Coded slice data partition A"; break;
    case  NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B:  nal_unit_type_name = "Coded slice data partition B"; break;
    case  NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C:  nal_unit_type_name = "Coded slice data partition C"; break;
    case  NAL_UNIT_TYPE_CODED_SLICE_IDR:               nal_unit_type_name = "Coded slice of an IDR picture"; break;
    case  NAL_UNIT_TYPE_SEI:                           nal_unit_type_name = "Supplemental enhancement information (SEI)"; break;
    case  NAL_UNIT_TYPE_SPS:                           nal_unit_type_name = "Sequence parameter set"; break;
    case  NAL_UNIT_TYPE_PPS:                           nal_unit_type_name = "Picture parameter set"; break;
    case  NAL_UNIT_TYPE_AUD:                           nal_unit_type_name = "Access unit delimiter"; break;
    case  NAL_UNIT_TYPE_END_OF_SEQUENCE:               nal_unit_type_name = "End of sequence"; break;
    case  NAL_UNIT_TYPE_END_OF_STREAM:                 nal_unit_type_name = "End of stream"; break;
    case  NAL_UNIT_TYPE_FILLER:                        nal_unit_type_name = "Filler data"; break;
    case  NAL_UNIT_TYPE_SPS_EXT:                       nal_unit_type_name = "Sequence parameter set extension"; break;
        // 14..18    // Reserved
    case  NAL_UNIT_TYPE_CODED_SLICE_AUX:               nal_unit_type_name = "Coded slice of an auxiliary coded picture without partitioning"; break;
        // 20..23    // Reserved
        // 24..31    // Unspecified
    default:                                           nal_unit_type_name = "Unknown"; break;
    }
    my_printf("nal_unit_type: %d (%s) (5 bits)", nal->nal_unit_type.get(), nal_unit_type_name);
    AddTreeItem(nal->nal_unit_type, rootNode);

    if (nal->nal_unit_type == NAL_UNIT_TYPE_CODED_SLICE_NON_IDR) { h264debugSliceHeader(h, rootNode); }
    else if (nal->nal_unit_type == NAL_UNIT_TYPE_CODED_SLICE_IDR) { h264debugSliceHeader(h, rootNode); }
    else if (nal->nal_unit_type == NAL_UNIT_TYPE_SPS) { h264debugSPS(h->sps, rootNode); }
    else if (nal->nal_unit_type == NAL_UNIT_TYPE_PPS) { h264debugPPS(h->pps, rootNode); }
    else if (nal->nal_unit_type == NAL_UNIT_TYPE_AUD) { h264debugAUD(h->aud, rootNode); }
    else if (nal->nal_unit_type == NAL_UNIT_TYPE_SEI) { h264debugSEI(h, rootNode); }
}
void NALParser::h264debugSliceHeader(h264_stream_t* h, NALTreeNode& root)
{
    sps_t* sps = h->sps;
    pps_t* pps = h->pps;
    slice_header_t* sh = h->sh;
    nal_t* nal = h->nal;

    my_printf("slice_layer_without_partitioning_rbsp()");
    auto slice = std::make_shared<TreeNode<ParameterDescription>>("slice_layer_without_partitioning_rbsp()");
    this->m_root->putChild(slice, root);
    my_printf("slice_header()");
    auto iheader = std::make_shared<TreeNode<ParameterDescription>>("slice_header()");
    this->m_root->putChild(iheader, slice);


    my_printf("first_mb_in_slice: %d  (v bits)", sh->first_mb_in_slice.get()); AddTreeItem(sh->first_mb_in_slice, iheader);
    const char* slice_type_name = NULL;
    switch (sh->slice_type.get())
    {
    case SH_SLICE_TYPE_P:       slice_type_name = "P slice"; break;
    case SH_SLICE_TYPE_B:       slice_type_name = "B slice"; break;
    case SH_SLICE_TYPE_I:       slice_type_name = "I slice"; break;
    case SH_SLICE_TYPE_SP:      slice_type_name = "SP slice"; break;
    case SH_SLICE_TYPE_SI:      slice_type_name = "SI slice"; break;
    case SH_SLICE_TYPE_P_ONLY:  slice_type_name = "P slice only"; break;
    case SH_SLICE_TYPE_B_ONLY:  slice_type_name = "B slice only"; break;
    case SH_SLICE_TYPE_I_ONLY:  slice_type_name = "I slice only"; break;
    case SH_SLICE_TYPE_SP_ONLY: slice_type_name = "SP slice only"; break;
    case SH_SLICE_TYPE_SI_ONLY: slice_type_name = "SI slice only"; break;
    default:                    slice_type_name = "Unknown"; break;
    }
    my_printf("slice_type: %d (%s)  (v bits)", sh->slice_type.get(), slice_type_name); AddTreeItem(sh->slice_type, iheader);

    my_printf("pic_parameter_set_id: %d  (v bits)", sh->pic_parameter_set_id);
    auto ppsid = std::make_shared<TreeNode<ParameterDescription>>("slice_header()");
    this->m_root->putChild(ppsid, iheader);

    if (sps->separate_colour_plane_flag == 1)
    {
        my_printf("colour_plane_id: %d  (2 bits)", sh->colour_plane_id.get()); AddTreeItem(sh->colour_plane_id, ppsid);
    }
    my_printf("frame_num: %d  (%d bits)", sh->frame_num.get(), sh->frame_num_bytes.get()); AddTreeItem(sh->frame_num_bytes,iheader);
    if (!sps->frame_mbs_only_flag.get())
    {
        my_printf_flag("field_pic_flag", sh->field_pic_flag.get());

        auto fpf = std::make_shared<TreeNode<ParameterDescription>>("slice_header()");
        this->m_root->putChild(fpf, iheader);

        if (sh->field_pic_flag.get())
        {
            my_printf_flag("bottom_field_flag", sh->bottom_field_flag.get());
            AddTreeItem(sh->bottom_field_flag, fpf);
        }
    }
    if (nal->nal_unit_type == 5)
    {
        my_printf("idr_pic_id: %d  (v bits)", sh->idr_pic_id.get()); AddTreeItem(sh->idr_pic_id, iheader);
    }
    if (sps->pic_order_cnt_type == 0)
    {
        my_printf("pic_order_cnt_lsb: %d  (%d bits)", sh->pic_order_cnt_lsb.get(), sh->pic_order_cnt_lsb_bytes.get()); AddTreeItem(sh->pic_order_cnt_lsb, iheader);
        if (pps->bottom_field_pic_order_in_frame_present_flag.get() && !sh->field_pic_flag.get())
        {
            my_printf("delta_pic_order_cnt_bottom: %d  (v bits)", sh->delta_pic_order_cnt_bottom.get()); AddTreeItem(sh->delta_pic_order_cnt_bottom, iheader);
        }
    }

    if (sps->pic_order_cnt_type.get() == 1 && !sps->delta_pic_order_always_zero_flag.get())
    {
        my_printf("delta_pic_order_cnt[0]: %d  (v bits)", sh->delta_pic_order_cnt[0]); AddTreeItem(sh->delta_pic_order_cnt[0] , iheader);
        if (pps->bottom_field_pic_order_in_frame_present_flag.get() && !sh->field_pic_flag.get())
        {
            my_printf("delta_pic_order_cnt[1]: %d  (v bits)", sh->delta_pic_order_cnt[1]);
            AddTreeItem(sh->delta_pic_order_cnt[1] , iheader);
        }
    }
    if (pps->redundant_pic_cnt_present_flag.get())
    {
        my_printf("redundant_pic_cnt: %d  (v bits)", sh->redundant_pic_cnt.get()); AddTreeItem(sh->redundant_pic_cnt , iheader);
    }
    if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_B))
    {
        my_printf_flag("direct_spatial_mv_pred_flag", sh->direct_spatial_mv_pred_flag.get());
        AddTreeItem(sh->direct_spatial_mv_pred_flag , iheader);
    }
    if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_P) || is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SP) || is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_B))
    {
        my_printf_flag("num_ref_idx_active_override_flag", sh->num_ref_idx_active_override_flag.get());
        AddTreeItem(sh->num_ref_idx_active_override_flag, iheader);
        if (sh->num_ref_idx_active_override_flag.get())
        {
            my_printf("num_ref_idx_l0_active_minus1: %d  (v bits)", sh->num_ref_idx_l0_active_minus1.get());
            AddTreeItem(sh->num_ref_idx_l0_active_minus1 , iheader);
            if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_B))
            {
                my_printf("num_ref_idx_l1_active_minus1: %d  (v bits)", sh->num_ref_idx_l1_active_minus1.get());
                AddTreeItem(sh->num_ref_idx_l1_active_minus1 , iheader);
            }
        }
    }
    // ref_pic_list_modification
    if (nal->nal_unit_type == 20 || nal->nal_unit_type == 21)
    {
        // todo.....
        my_printf("ref_pic_list_mvc_modification()");
        auto rplmm = std::make_shared<TreeNode<ParameterDescription>>("ref_pic_list_mvc_modification()");
        this->m_root->putChild(rplmm, iheader);

      
    }
    else
    {
        my_printf("ref_pic_list_modification()");
        auto rplm = std::make_shared<TreeNode<ParameterDescription>>("ref_pic_list_modification()");
        this->m_root->putChild(rplm, iheader);

        if (!is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_I) && !is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SI))
        {
            my_printf_flag("ref_pic_list_modification_flag_l0", sh->rplm.ref_pic_list_modification_flag_l0.get());
            AddTreeItem(sh->rplm.ref_pic_list_modification_flag_l0, rplm);
            if (sh->rplm.ref_pic_list_modification_flag_l0.get())
            {
                for (unsigned int i = 0; i < sh->rplm.rplm.size(); i++)
                {
                    my_printf("modification_of_pic_nums_idc: %d  (v bits)", sh->rplm.rplm[i].modification_of_pic_nums_idc.get());
                    AddTreeItem(sh->rplm.rplm[i].modification_of_pic_nums_idc , rplm);
                    if (sh->rplm.rplm[i].modification_of_pic_nums_idc.get() == 0 ||
                        sh->rplm.rplm[i].modification_of_pic_nums_idc.get() == 1)
                    {
                        my_printf("abs_diff_pic_num_minus1: %d  (v bits)", sh->rplm.rplm[i].abs_diff_pic_num_minus1.get());
                        AddTreeItem(sh->rplm.rplm[i].abs_diff_pic_num_minus1, rplm);
                    }
                    else if (sh->rplm.rplm[i].modification_of_pic_nums_idc.get() == 2)
                    {
                        my_printf("long_term_pic_num: %d  (v bits)", sh->rplm.rplm[i].long_term_pic_num.get());
                        AddTreeItem(sh->rplm.rplm[i].long_term_pic_num, rplm);
                    }
                }
            }
        }
        if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_B))
        {
            my_printf_flag("ref_pic_list_modification_flag_l1", sh->rplm.ref_pic_list_modification_flag_l1.get());
            AddTreeItem(sh->rplm.ref_pic_list_modification_flag_l1, rplm);
            if (sh->rplm.ref_pic_list_modification_flag_l1.get())
            {
                for (unsigned int i = 0; i < sh->rplm.rplm.size(); i++)
                {
                    my_printf("modification_of_pic_nums_idc: %d  (v bits)", sh->rplm.rplm[i].modification_of_pic_nums_idc.get());
                    AddTreeItem(sh->rplm.rplm[i].modification_of_pic_nums_idc, rplm);
                    if (sh->rplm.rplm[i].modification_of_pic_nums_idc.get() == 0 ||
                        sh->rplm.rplm[i].modification_of_pic_nums_idc.get() == 1)
                    {
                        my_printf("abs_diff_pic_num_minus1: %d  (v bits)", sh->rplm.rplm[i].abs_diff_pic_num_minus1.get());
                        AddTreeItem(sh->rplm.rplm[i].abs_diff_pic_num_minus1, rplm);
                    }
                    else if (sh->rplm.rplm[i].modification_of_pic_nums_idc.get() == 2)
                    {
                        my_printf("long_term_pic_num: %d  (v bits)", sh->rplm.rplm[i].long_term_pic_num.get());
                        AddTreeItem(sh->rplm.rplm[i].long_term_pic_num , rplm);
                    }
                }
            }
        }
    }

    // pred_weight_table()
    if ((pps->weighted_pred_flag.get() && (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_P) || is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SP))) ||
        (pps->weighted_bipred_idc.get() == 1 && is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_B)))
    {
        my_printf("pred_weight_table()");
        auto ipwt = std::make_shared<TreeNode<ParameterDescription>>("pred_weight_table()");
        this->m_root->putChild(ipwt, iheader);

        my_printf("luma_log2_weight_denom: %d  (v bits)", sh->pwt.luma_log2_weight_denom.get()); AddTreeItem(sh->pwt.luma_log2_weight_denom, ipwt);
        if (sps->ChromaArrayType != 0)
        {
            my_printf("chroma_log2_weight_denom: %d  (v bits)", sh->pwt.chroma_log2_weight_denom.get());
            AddTreeItem(sh->pwt.chroma_log2_weight_denom, ipwt);
        }
        auto ilwl0 = std::make_shared<TreeNode<ParameterDescription>>("luma_weight_l0()");
        this->m_root->putChild(ilwl0, ipwt);
        // 将luma和chroma分开显示
        if (sh->num_ref_idx_l0_active_minus1.get() >= 0)
        {
            my_printf("luma_weight_l0()");
            AddTreeItem(0 , ilwl0);
            
        }
        for (int i = 0; i <= sh->num_ref_idx_l0_active_minus1.get(); i++)
        {
            my_printf_flag2("luma_weight_l0_flag", i, sh->pwt.luma_weight_l0_flag[i]);
            auto ilwl0f = std::make_shared<TreeNode<ParameterDescription>>("luma_weight_l0_flag");
            this->m_root->putChild(ilwl0f, ilwl0);
   
            if (sh->pwt.luma_weight_l0_flag[i])
            {
                my_printf("luma_weight_l0[%d]: %d  (v bits)", i, sh->pwt.luma_weight_l0[i]);
                AddTreeItem(sh->pwt.luma_weight_l0[i],ilwl0f);
                my_printf("luma_offset_l0[%d]: %d  (v bits)", i, sh->pwt.luma_offset_l0[i]);
                AddTreeItem(sh->pwt.luma_offset_l0[i], ilwl0f);
            }
        }
        auto icwl0 = std::make_shared<TreeNode<ParameterDescription>>("chroma_weight_l0()");
        this->m_root->putChild(icwl0, ipwt);

        if (sh->num_ref_idx_l0_active_minus1.get() >= 0)
        {
            my_printf("chroma_weight_l0()");
            AddTreeItem(0, ipwt);
           
        }
        for (int i = 0; i <= sh->num_ref_idx_l0_active_minus1.get(); i++)
        {
            if (sps->ChromaArrayType != 0)
            {
                my_printf_flag2("chroma_weight_l0_flag", i, sh->pwt.chroma_weight_l0_flag[i]);
                auto icwl0f = std::make_shared<TreeNode<ParameterDescription>>("chroma_weight_l0_flag");
                this->m_root->putChild(icwl0f, icwl0);

                if (sh->pwt.chroma_weight_l0_flag[i])
                {
                    for (int j = 0; j < 2; j++)
                    {
                        my_printf("chroma_weight_l0[%d][%d]: %d  (v bits)", i, j, sh->pwt.chroma_weight_l0[i][j]);
                        AddTreeItem(sh->pwt.chroma_weight_l0[i][j], icwl0f);
                        my_printf("chroma_weight_l0[%d][%d]: %d  (v bits)", i, j, sh->pwt.chroma_offset_l0[i][j]);
                        AddTreeItem(sh->pwt.chroma_offset_l0[i][j] , icwl0f);
                    }
                }
            }
        }
        if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_B))
        {
            auto ilwl1 = std::make_shared<TreeNode<ParameterDescription>>("luma_weight_l1()");
            this->m_root->putChild(icwl0, ipwt);


            if (sh->num_ref_idx_l1_active_minus1.get() >= 0)
            {
                my_printf("luma_weight_l1()");
            }
            for (int i = 0; i <= sh->num_ref_idx_l1_active_minus1.get(); i++)
            {
                my_printf_flag2("luma_weight_l1_flag", i, sh->pwt.luma_weight_l1_flag[i]);

                auto ilwl1f = std::make_shared<TreeNode<ParameterDescription>>("chroma_weight_l0()");
                this->m_root->putChild(ilwl1f, ilwl1);
       
                if (sh->pwt.luma_weight_l1_flag[i])
                {
                    my_printf("luma_weight_l1[%d]: %d  (v bits)", i, sh->pwt.luma_weight_l1[i]);
                    AddTreeItem(sh->pwt.luma_weight_l1[i], ilwl1f);
                    my_printf("luma_offset_l1[%d]: %d  (v bits)", i, sh->pwt.luma_offset_l1[i]);
                    AddTreeItem(sh->pwt.luma_offset_l1[i], ilwl1f);
                }
            }
            auto icwl1 = std::make_shared<TreeNode<ParameterDescription>>("chroma_weight_l1()");
            this->m_root->putChild(icwl0, ipwt);


            if (sh->num_ref_idx_l1_active_minus1.get() >= 0)
            {
                my_printf("chroma_weight_l1()");
               AddTreeItem(0, ipwt);
            }
            for (int i = 0; i <= sh->num_ref_idx_l1_active_minus1.get(); i++)
            {
                if (sps->ChromaArrayType != 0)
                {
                    my_printf_flag2("chroma_weight_l1_flag", i, sh->pwt.chroma_weight_l1_flag[i]);
                    auto icwl1f = std::make_shared<TreeNode<ParameterDescription>>("chroma_weight_l1_flag()");
                    this->m_root->putChild(icwl1f, icwl1);
                    AddTreeItem(sh->pwt.chroma_weight_l1_flag[i], icwl1f);
                    if (sh->pwt.chroma_weight_l1_flag[i])
                    {
                        for (int j = 0; j < 2; j++)
                        {
                            my_printf("chroma_weight_l1[%d][%d]: %d  (v bits)", i, j, sh->pwt.chroma_weight_l1[i][j]);
                            AddTreeItem(sh->pwt.chroma_weight_l1[i][j], icwl1f);
                            my_printf("chroma_offset_l1[%d][%d]: %d  (v bits)", i, j, sh->pwt.chroma_offset_l1[i][j]);
                            AddTreeItem(sh->pwt.chroma_offset_l1[i][j], icwl1f);
                        }
                    }
                }
            }
        }
    }
    // dec_ref_pic_marking()
    if (nal->nal_ref_idc != 0)
    {
        my_printf("dec_ref_pic_marking()");
        auto idrpm = std::make_shared<TreeNode<ParameterDescription>>("dec_ref_pic_marking()");
        this->m_root->putChild(idrpm, iheader);


        if (h->nal->nal_unit_type == 5)
        {
            my_printf_flag("no_output_of_prior_pics_flag", sh->drpm.no_output_of_prior_pics_flag.get());
            AddTreeItem(sh->drpm.no_output_of_prior_pics_flag,idrpm);
            my_printf_flag("long_term_reference_flag", sh->drpm.long_term_reference_flag.get());
            AddTreeItem(sh->drpm.no_output_of_prior_pics_flag, idrpm);
        }
        else
        {
            my_printf_flag("adaptive_ref_pic_marking_mode_flag", sh->drpm.adaptive_ref_pic_marking_mode_flag.get());
            auto arpmmf = std::make_shared<TreeNode<ParameterDescription>>("adaptive_ref_pic_marking_mode_flag()");
            this->m_root->putChild(arpmmf, idrpm);

            if (sh->drpm.adaptive_ref_pic_marking_mode_flag.get())
            {
                for (unsigned int i = 0; i < sh->drpm.drpm.size(); i++)
                {
                    my_printf("memory_management_control_operation[%u]: %d  (v bits)", i, sh->drpm.drpm[i].memory_management_control_operation.get());
                    AddTreeItem(sh->drpm.drpm[i].memory_management_control_operation, arpmmf);
                    if (sh->drpm.drpm[i].memory_management_control_operation.get() == 1 ||
                        sh->drpm.drpm[i].memory_management_control_operation.get() == 3)
                    {
                        my_printf("difference_of_pic_nums_minus1[%u]: %d  (v bits)", i, sh->drpm.drpm[i].difference_of_pic_nums_minus1.get());
                        AddTreeItem(sh->drpm.drpm[i].difference_of_pic_nums_minus1, arpmmf);
                    }
                    if (sh->drpm.drpm[i].memory_management_control_operation.get() == 2)
                    {
                        my_printf("long_term_pic_num[%u]: %d  (v bits)", i, sh->drpm.drpm[i].long_term_pic_num.get());
                        AddTreeItem(sh->drpm.drpm[i].long_term_pic_num , arpmmf);
                    }
                    if (sh->drpm.drpm[i].memory_management_control_operation.get() == 3 ||
                        sh->drpm.drpm[i].memory_management_control_operation.get() == 6)
                    {
                        my_printf("long_term_frame_idx[%u]: %d  (v bits)", i, sh->drpm.drpm[i].long_term_frame_idx.get());
                        AddTreeItem(sh->drpm.drpm[i].long_term_frame_idx, arpmmf);
                    }
                    if (sh->drpm.drpm[i].memory_management_control_operation.get() == 4)
                    {
                        my_printf("max_long_term_frame_idx_plus1[%u]: %d  (v bits)", i, sh->drpm.drpm[i].max_long_term_frame_idx_plus1.get());
                        AddTreeItem(sh->drpm.drpm[i].max_long_term_frame_idx_plus1, arpmmf);
                    }
                }
            }
        }
    }
    if (pps->entropy_coding_mode_flag.get() && !is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_I) && !is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SI))
    {
        my_printf("cabac_init_idc: %d", sh->cabac_init_idc.get());
        AddTreeItem(sh->cabac_init_idc, iheader);
    }
    my_printf("slice_qp_delta: %d", sh->slice_qp_delta.get());
    AddTreeItem(sh->slice_qp_delta, iheader);
    if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SP) || is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SI))
    {
        if (is_slice_type(sh->slice_type.get(), SH_SLICE_TYPE_SP))
        {
            my_printf_flag("sp_for_switch_flag", sh->sp_for_switch_flag.get());
            AddTreeItem(sh->sp_for_switch_flag, iheader);
        }
        my_printf("slice_qs_delta: %d  (v bits)", sh->slice_qs_delta.get());
        AddTreeItem(sh->slice_qs_delta, iheader);
    }
    if (pps->deblocking_filter_control_present_flag.get())
    {
        my_printf("disable_deblocking_filter_idc: %d  (v bits)", sh->disable_deblocking_filter_idc.get());
        AddTreeItem(sh->disable_deblocking_filter_idc, iheader);
        if (sh->disable_deblocking_filter_idc.get() != 1)
        {
            my_printf("slice_alpha_c0_offset_div2: %d  (v bits)", sh->slice_alpha_c0_offset_div2.get());
            AddTreeItem(sh->slice_alpha_c0_offset_div2 ,iheader);
            my_printf("slice_beta_offset_div2: %d  (v bits)", sh->slice_beta_offset_div2.get());
            AddTreeItem(sh->slice_beta_offset_div2, iheader);
        }
    }

    if (pps->num_slice_groups_minus1.get() > 0 &&
        pps->slice_group_map_type.get() >= 3 && pps->slice_group_map_type.get() <= 5)
    {
        my_printf("slice_group_change_cycle: %d  (%d bits)", sh->slice_group_change_cycle.get(), sh->slice_group_change_cycle_bytes.get());
        AddTreeItem(sh->slice_group_change_cycle, iheader);
    }

    my_printf("slice_data()"); AddTreeItem(0,slice);
    my_printf("rbsp_slice_trailing_bits()"); AddTreeItem(0, slice);
}

void NALParser::h264debugSPS(sps_t* sps, NALTreeNode& root)
{
    my_printf("seq_parameter_set_data()");
    auto isps = std::make_shared<TreeNode<ParameterDescription>>("seq_parameter_set_data()");
    this->m_root->putChild(isps , root);

    my_printf("profile_idc: %d  (8 bits)", sps->profile_idc.get()); AddTreeItem(sps->profile_idc,isps);
    my_printf_flag("constraint_set0_flag", sps->constraint_set0_flag.get()); AddTreeItem(sps->constraint_set0_flag, isps);
    my_printf_flag("constraint_set1_flag", sps->constraint_set1_flag.get()); AddTreeItem(sps->constraint_set1_flag, isps);
    my_printf_flag("constraint_set2_flag", sps->constraint_set2_flag.get()); AddTreeItem(sps->constraint_set2_flag, isps);
    my_printf_flag("constraint_set3_flag", sps->constraint_set3_flag.get()); AddTreeItem(sps->constraint_set3_flag, isps);
    my_printf_flag("constraint_set4_flag", sps->constraint_set4_flag.get()); AddTreeItem(sps->constraint_set4_flag, isps);
    my_printf_flag("constraint_set5_flag", sps->constraint_set5_flag.get()); AddTreeItem(sps->constraint_set5_flag, isps);
    my_printf("reserved_zero_2bits: %d  (2 bits)", sps->reserved_zero_2bits.get()); AddTreeItem(sps->reserved_zero_2bits, isps);
    my_printf("level_idc: %d  (8 bits)", sps->level_idc.get()); AddTreeItem(sps->level_idc, isps);
    my_printf("seq_parameter_set_id: %d  (v bits)", sps->seq_parameter_set_id.get()); AddTreeItem(sps->seq_parameter_set_id, isps);
    if (sps->profile_idc == 100 || sps->profile_idc == 110 ||
        sps->profile_idc == 122 || sps->profile_idc == 144)
    {
        my_printf("chroma_format_idc: %d  (8 bits)", sps->chroma_format_idc.get()); AddTreeItem(sps->chroma_format_idc, isps);
        if (sps->chroma_format_idc == 3)
        {
            my_printf_flag("separate_colour_plane_flag", sps->separate_colour_plane_flag.get()); AddTreeItem(sps->chroma_format_idc,isps);
        }
        my_printf("bit_depth_luma_minus8: %d  (v bits)", sps->bit_depth_luma_minus8.get()); AddTreeItem(sps->bit_depth_luma_minus8, isps);
        my_printf("bit_depth_chroma_minus8: %d  (v bits)", sps->bit_depth_chroma_minus8.get()); AddTreeItem(sps->bit_depth_chroma_minus8 ,isps);
        my_printf_flag("qpprime_y_zero_transform_bypass_flag", sps->qpprime_y_zero_transform_bypass_flag.get()); AddTreeItem(sps->qpprime_y_zero_transform_bypass_flag, isps);
        my_printf_flag("seq_scaling_matrix_present_flag", sps->seq_scaling_matrix_present_flag.get());
        auto ssmpf = std::make_shared<TreeNode<ParameterDescription>>("seq_scaling_matrix_present_flag");
        m_root->putChild(ssmpf, isps);
        if (sps->seq_scaling_matrix_present_flag.get())
        {
            for (int i = 0; i < ((sps->chroma_format_idc != 3) ? 8 : 12); i++)
            {
                my_printf_flag2("seq_scaling_list_present_flag", i, sps->seq_scaling_list_present_flag[i]);
                AddTreeItem(sps->seq_scaling_list_present_flag[i],ssmpf);
                if (sps->seq_scaling_list_present_flag[i])
                {
                    if (i < 6)
                    {
                        my_printf("ScalingList4x4[%d]: %d  (v bits)", i, sps->ScalingList4x4[i]);
                        AddTreeItem(sps->ScalingList4x4[i],ssmpf);
                    }
                    else
                    {
                        my_printf("ScalingList8x8[%d]: %d  (v bits)", i, sps->ScalingList8x8[i]);
                        AddTreeItem(sps->ScalingList8x8[i],ssmpf);
                    }
                }
            }
        }
    }
    my_printf("log2_max_frame_num_minus4: %d  (v bits)", sps->log2_max_frame_num_minus4.get()); AddTreeItem(sps->log2_max_frame_num_minus4,isps);
    my_printf("pic_order_cnt_type: %d  (v bits)", sps->pic_order_cnt_type.get()); AddTreeItem(sps->pic_order_cnt_type, isps);
    if (sps->pic_order_cnt_type.get() == 0)
    {
        my_printf("log2_max_pic_order_cnt_lsb_minus4: %d  (v bits)", sps->log2_max_pic_order_cnt_lsb_minus4.get());
        AddTreeItem(sps->log2_max_pic_order_cnt_lsb_minus4,isps);
    }
    else if (sps->pic_order_cnt_type == 1)
    {
        my_printf_flag("delta_pic_order_always_zero_flag", sps->delta_pic_order_always_zero_flag.get()); AddTreeItem(sps->delta_pic_order_always_zero_flag,isps);
        my_printf("offset_for_non_ref_pic: %d  (v bits)", sps->offset_for_non_ref_pic.get()); AddTreeItem(sps->offset_for_non_ref_pic, isps);
        my_printf("offset_for_top_to_bottom_field: %d  (v bits)", sps->offset_for_top_to_bottom_field.get()); AddTreeItem(sps->offset_for_top_to_bottom_field, isps);
        my_printf("num_ref_frames_in_pic_order_cnt_cycle: %d  (v bits)", sps->num_ref_frames_in_pic_order_cnt_cycle.get()); AddTreeItem(sps->num_ref_frames_in_pic_order_cnt_cycle, isps);
        for (int i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle.get(); i++)
        {
            my_printf("offset_for_ref_frame[%d]: %d  (v bits)", i, sps->offset_for_ref_frame[i]); AddTreeItem(sps->offset_for_ref_frame[i],isps);
        }
    }
    my_printf("max_num_ref_frames: %d  (v bits)", sps->max_num_ref_frames.get()); AddTreeItem(sps->max_num_ref_frames,isps);
    my_printf_flag("gaps_in_frame_num_value_allowed_flag", sps->gaps_in_frame_num_value_allowed_flag.get()); AddTreeItem(sps->gaps_in_frame_num_value_allowed_flag,isps);
    my_printf("pic_width_in_mbs_minus1: %d  (v bits)", sps->pic_width_in_mbs_minus1.get()); AddTreeItem(sps->pic_width_in_mbs_minus1, isps);
    my_printf("pic_height_in_map_units_minus1: %d  (v bits)", sps->pic_height_in_map_units_minus1.get()); AddTreeItem(sps->pic_height_in_map_units_minus1, isps);
    my_printf_flag("frame_mbs_only_flag", sps->frame_mbs_only_flag.get());
    auto fmof = std::make_shared<TreeNode<ParameterDescription>>("frame_mbs_only_flag");
    m_root->putChild(fmof, isps);

    if (!sps->frame_mbs_only_flag.get())
    {
        my_printf_flag("mb_adaptive_frame_field_flag", sps->mb_adaptive_frame_field_flag.get()); AddTreeItem(sps->mb_adaptive_frame_field_flag, fmof);
    }
    my_printf_flag("direct_8x8_inference_flag", sps->direct_8x8_inference_flag.get()); AddTreeItem(sps->direct_8x8_inference_flag,isps);
    my_printf_flag("frame_cropping_flag", sps->frame_cropping_flag.get());
    auto fcf = std::make_shared<TreeNode<ParameterDescription>>("frame_mbs_only_flag");
    m_root->putChild(fcf, isps);
    if (sps->frame_cropping_flag.get())
    {
        my_printf("frame_crop_left_offset: %d  (v bits)", sps->frame_crop_left_offset.get()); AddTreeItem(sps->frame_crop_left_offset,fcf);
        my_printf("frame_crop_right_offset: %d  (v bits)", sps->frame_crop_right_offset.get()); AddTreeItem(sps->frame_crop_right_offset,fcf);
        my_printf("frame_crop_top_offset: %d  (v bits)", sps->frame_crop_top_offset.get()); AddTreeItem(sps->frame_crop_right_offset,fcf);
        my_printf("frame_crop_bottom_offset: %d  (v bits)", sps->frame_crop_bottom_offset.get()); AddTreeItem(sps->frame_crop_right_offset, fcf);
    }
    my_printf_flag("vui_parameters_present_flag", sps->vui_parameters_present_flag.get());
    auto vppf = std::make_shared<TreeNode<ParameterDescription>>("vui_parameters_present_flag");
    m_root->putChild(vppf, isps);
    if (sps->vui_parameters_present_flag.get())
    {
        my_printf("vui_parameters()");
        auto vp = std::make_shared<TreeNode<ParameterDescription>>("vui_parameters()");
        m_root->putChild(vp, vppf);
        my_printf_flag("aspect_ratio_info_present_flag", sps->vui.aspect_ratio_info_present_flag.get());

        auto aripf = std::make_shared<TreeNode<ParameterDescription>>("aspect_ratio_info_present_flag");
        m_root->putChild(aripf, vp);
        if (sps->vui.aspect_ratio_info_present_flag.get())
        {
            my_printf("aspect_ratio_idc: %d  (8 bits)", sps->vui.aspect_ratio_idc); AddTreeItem(sps->vui.aspect_ratio_idc,aripf);
            if (sps->vui.aspect_ratio_idc == SAR_Extended)
            {
                my_printf("sar_width: %d   (16 bits)", sps->vui.sar_width.get());
                my_printf("sar_height: %d  (16 bits)", sps->vui.sar_height.get());
            }
        }
        my_printf_flag("overscan_info_present_flag", sps->vui.overscan_info_present_flag.get());
        auto oipf = std::make_shared<TreeNode<ParameterDescription>>("overscan_info_present_flag");
        m_root->putChild(oipf, vp);

        if (sps->vui.overscan_info_present_flag.get())
        {
            my_printf_flag("overscan_appropriate_flag", sps->vui.overscan_appropriate_flag.get()); AddTreeItem(sps->vui.overscan_appropriate_flag, oipf);
        }
        my_printf_flag("video_signal_type_present_flag", sps->vui.video_signal_type_present_flag.get());
        auto vstpf = std::make_shared<TreeNode<ParameterDescription>>("video_signal_type_present_flag");
        m_root->putChild(vstpf, vp);

        if (sps->vui.video_signal_type_present_flag.get())
        {
            my_printf("video_format: %d  (3 bits)", sps->vui.video_format.get()); AddTreeItem(sps->vui.video_format, vstpf);
            my_printf_flag("video_full_range_flag", sps->vui.video_full_range_flag.get()); AddTreeItem(sps->vui.video_full_range_flag, vstpf);
            my_printf_flag("colour_description_present_flag", sps->vui.colour_description_present_flag.get());
            auto cdpf = std::make_shared<TreeNode<ParameterDescription>>("colour_description_present_flag");
            m_root->putChild(cdpf, vstpf);

            if (sps->vui.colour_description_present_flag.get())
            {
                my_printf("colour_primaries: %d  (8 bits)", sps->vui.colour_primaries.get()); AddTreeItem(sps->vui.colour_primaries, cdpf);
                my_printf("transfer_characteristics: %d  (8 bits)", sps->vui.transfer_characteristics.get()); AddTreeItem(sps->vui.transfer_characteristics,cdpf);
                my_printf("matrix_coefficients: %d  (8 bits)", sps->vui.matrix_coefficients.get()); AddTreeItem(sps->vui.matrix_coefficients,cdpf);
            }
        }
        my_printf_flag("chroma_loc_info_present_flag", sps->vui.chroma_loc_info_present_flag.get());
        auto clipf = std::make_shared<TreeNode<ParameterDescription>>("chroma_loc_info_present_flag");
        m_root->putChild(clipf, vp);

        if (sps->vui.chroma_loc_info_present_flag.get())
        {
            my_printf("chroma_sample_loc_type_top_field: %d  (v bits)", sps->vui.chroma_sample_loc_type_top_field.get()); AddTreeItem(sps->vui.chroma_sample_loc_type_top_field, clipf);
            my_printf("chroma_sample_loc_type_bottom_field: %d  (v bits)", sps->vui.chroma_sample_loc_type_bottom_field.get()); AddTreeItem(sps->vui.chroma_sample_loc_type_bottom_field,clipf);
        }
        my_printf_flag("timing_info_present_flag", sps->vui.timing_info_present_flag.get());

        auto tipf = std::make_shared<TreeNode<ParameterDescription>>("timing_info_present_flag");
        m_root->putChild(tipf, vp);

        if (sps->vui.timing_info_present_flag.get())
        {
            my_printf("num_units_in_tick: %d  (32 bits)", sps->vui.num_units_in_tick.get()); AddTreeItem(sps->vui.num_units_in_tick, tipf);
            my_printf("time_scale: %d  (32 bits)", sps->vui.time_scale.get()); AddTreeItem(sps->vui.time_scale, tipf);
            my_printf_flag("fixed_frame_rate_flag", sps->vui.fixed_frame_rate_flag.get()); AddTreeItem(sps->vui.fixed_frame_rate_flag, tipf);
        }
        my_printf_flag("nal_hrd_parameters_present_flag", sps->vui.nal_hrd_parameters_present_flag.get());

        auto nhppf = std::make_shared<TreeNode<ParameterDescription>>("nal_hrd_parameters_present_flag");
        m_root->putChild(nhppf, vp);

        if (sps->vui.nal_hrd_parameters_present_flag.get())
        {
            my_printf("hrd_parameters()");
            auto hp = std::make_shared<TreeNode<ParameterDescription>>("hrd_parameters()");
            m_root->putChild(hp, nhppf);
            my_printf("cpb_cnt_minus1: %d  (v bits)", sps->hrd.cpb_cnt_minus1.get()); AddTreeItem(sps->hrd.cpb_cnt_minus1,hp);
            my_printf("bit_rate_scale: %d  (4 bits)", sps->hrd.bit_rate_scale.get()); AddTreeItem(sps->hrd.bit_rate_scale, hp);
            my_printf("cpb_size_scale: %d  (4 bits)", sps->hrd.cpb_size_scale.get()); AddTreeItem(sps->hrd.cpb_size_scale, hp);
            int SchedSelIdx;
            for (SchedSelIdx = 0; SchedSelIdx <= sps->hrd.cpb_cnt_minus1.get(); SchedSelIdx++)
            {
                my_printf("bit_rate_value_minus1[%d]: %d  (v bits)", SchedSelIdx, sps->hrd.bit_rate_value_minus1[SchedSelIdx]); // up to cpb_cnt_minus1, which is <= 31
                AddTreeItem(sps->hrd.bit_rate_value_minus1[SchedSelIdx],hp);
                my_printf("cpb_size_value_minus1[%d]: %d  (v bits)", SchedSelIdx, sps->hrd.cpb_size_value_minus1[SchedSelIdx]);
                AddTreeItem(sps->hrd.cpb_size_value_minus1[SchedSelIdx],hp);
                my_printf_flag2("cbr_flag", SchedSelIdx, sps->hrd.cbr_flag[SchedSelIdx]);
                AddTreeItem(sps->hrd.cbr_flag[SchedSelIdx], hp);
            }
            my_printf("initial_cpb_removal_delay_length_minus1: %d  (5 bits)", sps->hrd.initial_cpb_removal_delay_length_minus1.get()); AddTreeItem(sps->hrd.initial_cpb_removal_delay_length_minus1, nhppf);
            my_printf("cpb_removal_delay_length_minus1: %d  (5 bits)", sps->hrd.cpb_removal_delay_length_minus1.get()); AddTreeItem(sps->hrd.cpb_removal_delay_length_minus1,nhppf);
            my_printf("dpb_output_delay_length_minus1: %d  (5 bits)", sps->hrd.dpb_output_delay_length_minus1.get()); AddTreeItem(sps->hrd.dpb_output_delay_length_minus1,nhppf);
            my_printf("time_offset_length: %d  (5 bits)", sps->hrd.time_offset_length.get()); AddTreeItem(sps->hrd.time_offset_length, nhppf);
        }
        my_printf_flag("vcl_hrd_parameters_present_flag", sps->vui.vcl_hrd_parameters_present_flag.get());
        auto vhppf = std::make_shared<TreeNode<ParameterDescription>>("vcl_hrd_parameters_present_flag");
        m_root->putChild(vhppf, vp);

        if (sps->vui.vcl_hrd_parameters_present_flag.get())
        {
            my_printf("hrd_parameters()");
            auto hp = std::make_shared<TreeNode<ParameterDescription>>("hrd_parameters");
            m_root->putChild(vhppf, vhppf);

            my_printf("cpb_cnt_minus1: %d  (v bits)", sps->hrd.cpb_cnt_minus1.get()); AddTreeItem(sps->hrd.cpb_cnt_minus1,hp);
            my_printf("bit_rate_scale: %d  (4 bits)", sps->hrd.bit_rate_scale.get()); AddTreeItem(sps->hrd.bit_rate_scale,hp);
            my_printf("cpb_size_scale: %d  (4 bits)", sps->hrd.cpb_size_scale.get()); AddTreeItem(sps->hrd.cpb_size_scale,hp);
            int SchedSelIdx;
            for (SchedSelIdx = 0; SchedSelIdx <= sps->hrd.cpb_cnt_minus1.get(); SchedSelIdx++)
            {
                my_printf("bit_rate_value_minus1[%d]: %d  (v bits)", SchedSelIdx, sps->hrd.bit_rate_value_minus1[SchedSelIdx]); // up to cpb_cnt_minus1, which is <= 31
                AddTreeItem(sps->hrd.bit_rate_value_minus1[SchedSelIdx],hp);
                my_printf("cpb_size_value_minus1[%d]: %d  (v bits)", SchedSelIdx, sps->hrd.cpb_size_value_minus1[SchedSelIdx]);
                AddTreeItem(sps->hrd.cpb_size_value_minus1[SchedSelIdx],hp);
                my_printf_flag2("cbr_flag", SchedSelIdx, sps->hrd.cbr_flag[SchedSelIdx]); AddTreeItem(sps->hrd.cbr_flag[SchedSelIdx],hp);
            }
            my_printf("initial_cpb_removal_delay_length_minus1: %d  (5 bits)", sps->hrd.initial_cpb_removal_delay_length_minus1.get()); AddTreeItem(sps->hrd.initial_cpb_removal_delay_length_minus1, nhppf);
            my_printf("cpb_removal_delay_length_minus1: %d  (5 bits)", sps->hrd.cpb_removal_delay_length_minus1.get()); AddTreeItem(sps->hrd.cpb_removal_delay_length_minus1,nhppf);
            my_printf("dpb_output_delay_length_minus1: %d  (5 bits)", sps->hrd.dpb_output_delay_length_minus1.get()); AddTreeItem(sps->hrd.dpb_output_delay_length_minus1, nhppf);
            my_printf("time_offset_length: %d  (5 bits)", sps->hrd.time_offset_length.get()); AddTreeItem(sps->hrd.time_offset_length, nhppf);
        }
        if (sps->vui.nal_hrd_parameters_present_flag.get() || sps->vui.vcl_hrd_parameters_present_flag.get())
        {
            my_printf_flag("low_delay_hrd_flag", sps->vui.low_delay_hrd_flag.get()); AddTreeItem(sps->vui.low_delay_hrd_flag, vp); // tocheck
        }
        my_printf_flag("pic_struct_present_flag", sps->vui.pic_struct_present_flag.get()); AddTreeItem(sps->vui.pic_struct_present_flag,vp);
        my_printf_flag("bitstream_restriction_flag", sps->vui.bitstream_restriction_flag.get());
        auto brf = std::make_shared<TreeNode<ParameterDescription>>("bitstream_restriction_flag");
        m_root->putChild(brf, vp);
        if (sps->vui.bitstream_restriction_flag.get())
        {
            my_printf_flag("motion_vectors_over_pic_boundaries_flag", sps->vui.motion_vectors_over_pic_boundaries_flag.get()); AddTreeItem(sps->vui.motion_vectors_over_pic_boundaries_flag, brf);
            my_printf("max_bytes_per_pic_denom: %d  (v bits)", sps->vui.max_bytes_per_pic_denom.get()); AddTreeItem(sps->vui.max_bytes_per_pic_denom,brf);
            my_printf("max_bits_per_mb_denom: %d  (v bits)", sps->vui.max_bits_per_mb_denom.get()); AddTreeItem(sps->vui.max_bits_per_mb_denom,brf);
            my_printf("log2_max_mv_length_horizontal: %d  (v bits)", sps->vui.log2_max_mv_length_horizontal.get()); AddTreeItem(sps->vui.log2_max_mv_length_horizontal,brf);
            my_printf("log2_max_mv_length_vertical: %d  (v bits)", sps->vui.log2_max_mv_length_vertical.get()); AddTreeItem(sps->vui.log2_max_mv_length_vertical,brf);
            my_printf("num_reorder_frames: %d  (v bits)", sps->vui.num_reorder_frames.get()); AddTreeItem(sps->vui.num_reorder_frames,brf);
            my_printf("max_dec_frame_buffering: %d  (v bits)", sps->vui.max_dec_frame_buffering.get()); AddTreeItem(sps->vui.max_dec_frame_buffering, brf);
        }
    }
}

void NALParser::h264debugPPS(pps_t* pps, NALTreeNode& root)
{
    my_printf("pic_parameter_set_rbsp()");
    auto ipps = std::make_shared<TreeNode<ParameterDescription>>("pic_parameter_set_rbsp()");
    m_root->putChild(ipps, root);


    my_printf("pic_parameter_set_id: %d  (v bits)", pps->pic_parameter_set_id.get()); AddTreeItem(pps->pic_parameter_set_id, ipps);
    my_printf("seq_parameter_set_id: %d  (v bits)", pps->seq_parameter_set_id.get()); AddTreeItem(pps->seq_parameter_set_id, ipps);
    my_printf_flag("entropy_coding_mode_flag", pps->entropy_coding_mode_flag.get()); AddTreeItem(pps->entropy_coding_mode_flag , ipps);
    my_printf_flag("bottom_field_pic_order_in_frame_present_flag", pps->bottom_field_pic_order_in_frame_present_flag.get()); AddTreeItem(pps->bottom_field_pic_order_in_frame_present_flag , ipps);
    my_printf("num_slice_groups_minus1: %d  (v bits)", pps->num_slice_groups_minus1.get()); AddTreeItem(pps->num_slice_groups_minus1, ipps);
    if (pps->num_slice_groups_minus1.get() > 0)
    {
        my_printf("slice_group_map_type: %d  (v bits)", pps->slice_group_map_type.get()); AddTreeItem(pps->slice_group_map_type, ipps);
        if (pps->slice_group_map_type.get() == 0)
        {
            for (int i_group = 0; i_group <= pps->num_slice_groups_minus1.get(); i_group++){
                my_printf("run_length_minus1[%d]: %d  (v bits)", i_group, pps->run_length_minus1[i_group]); AddTreeItem(pps->run_length_minus1[i_group], ipps);
            }
               
        }
        else if (pps->slice_group_map_type.get() == 2)
        {
            for (int i_group = 0; i_group <= pps->num_slice_groups_minus1.get(); i_group++)
            {
                my_printf("top_left[%d]: %d  (v bits)", i_group, pps->top_left[i_group]); AddTreeItem(pps->top_left[i_group], ipps);
                my_printf("bottom_right[%d]: %d  (v bits)", i_group, pps->bottom_right[i_group]); AddTreeItem(pps->bottom_right[i_group], ipps);
            }
        }
        else if (pps->slice_group_map_type.get() == 3 ||
            pps->slice_group_map_type.get() == 4 ||
            pps->slice_group_map_type.get() == 5)
        {
            my_printf_flag("slice_group_change_direction_flag", pps->slice_group_change_direction_flag.get()); AddTreeItem(pps->slice_group_change_direction_flag, ipps);
            my_printf("slice_group_change_rate_minus1: %d  (v bits)", pps->slice_group_change_rate_minus1.get()); AddTreeItem(pps->slice_group_change_rate_minus1, ipps);
        }
        else if (pps->slice_group_map_type.get() == 6)
        {
            my_printf("pic_size_in_map_units_minus1: %d  (v bits)", pps->pic_size_in_map_units_minus1.get()); AddTreeItem(pps->pic_size_in_map_units_minus1, ipps);
            for (int i = 0; i <= pps->pic_size_in_map_units_minus1.get(); i++){
                my_printf("slice_group_id[%d]: %d  (%d bits)", i, pps->slice_group_id[i], pps->slice_group_id_bytes); AddTreeItem(pps->slice_group_id[i], ipps);
            }
               
        }
    }
    my_printf("num_ref_idx_l0_active_minus1: %d  (v bits)", pps->num_ref_idx_l0_active_minus1.get()); AddTreeItem(pps->num_ref_idx_l0_active_minus1, ipps);
    my_printf("num_ref_idx_l1_active_minus1: %d  (v bits)", pps->num_ref_idx_l1_active_minus1.get()); AddTreeItem(pps->num_ref_idx_l1_active_minus1, ipps);
    my_printf_flag("weighted_pred_flag", pps->weighted_pred_flag.get()); AddTreeItem(pps->weighted_pred_flag, ipps);
    const char* weighted_pre = NULL;
    if (pps->weighted_bipred_idc == 0)
    {
        weighted_pre = "Default";
    }
    if (pps->weighted_bipred_idc == 1)
    {
        weighted_pre = "Explicit";
    }
    if (pps->weighted_bipred_idc == 2)
    {
        weighted_pre = "Implicit";
    }
    my_printf("weighted_bipred_idc: %d  [%s]  (2 bits)", pps->weighted_bipred_idc.get(), weighted_pre); AddTreeItem(pps->weighted_bipred_idc, ipps);
    my_printf("pic_init_qp_minus26: %d  (v bits)", pps->pic_init_qp_minus26.get()); AddTreeItem(pps->pic_init_qp_minus26, ipps);
    my_printf("pic_init_qs_minus26: %d  (v bits)", pps->pic_init_qs_minus26.get()); AddTreeItem(pps->pic_init_qs_minus26,ipps);
    my_printf("chroma_qp_index_offset: %d  (v bits)", pps->chroma_qp_index_offset.get()); AddTreeItem(pps->chroma_qp_index_offset, ipps);
    my_printf_flag("deblocking_filter_control_present_flag", pps->deblocking_filter_control_present_flag.get()); AddTreeItem(pps->deblocking_filter_control_present_flag, ipps);
    my_printf_flag("constrained_intra_pred_flag", pps->constrained_intra_pred_flag.get()); AddTreeItem(pps->constrained_intra_pred_flag, ipps);
    my_printf_flag("redundant_pic_cnt_present_flag", pps->redundant_pic_cnt_present_flag.get()); AddTreeItem(pps->redundant_pic_cnt_present_flag, ipps);
    if (pps->_more_rbsp_data_present.get())
    {
        my_printf("more_rbsp_data()");
        auto imrdp = std::make_shared<TreeNode<ParameterDescription>>("more_rbsp_data()");
        m_root->putChild(imrdp, ipps);
        my_printf_flag("transform_8x8_mode_flag", pps->transform_8x8_mode_flag.get()); AddTreeItem(pps->transform_8x8_mode_flag, imrdp);
        my_printf_flag("pic_scaling_matrix_present_flag", pps->pic_scaling_matrix_present_flag.get());

        auto psmpf = std::make_shared<TreeNode<ParameterDescription>>("pic_scaling_matrix_present_flag");
        m_root->putChild(psmpf, imrdp);
 
        if (pps->pic_scaling_matrix_present_flag.get())
        {
            for (int i = 0; i < 6 + 2 * pps->transform_8x8_mode_flag.get(); i++)
            {
                my_printf_flag2("pic_scaling_list_present_flag", i, pps->pic_scaling_list_present_flag[i]);
                AddTreeItem(pps->pic_scaling_list_present_flag[i], psmpf);
                if (pps->pic_scaling_list_present_flag[i])
                {
                    if (i < 6)
                    {
                        my_printf("ScalingList4x4[%d]: %d  (v bits)", i, *(pps->ScalingList4x4[i]));
                        AddTreeItem(*(pps->ScalingList4x4[i]), psmpf);
                    }
                    else
                    {
                        my_printf("ScalingList4xScalingList8x84[%d]: %d  (v bits)", i, *(pps->ScalingList8x8[i]));
                        AddTreeItem(*(pps->ScalingList8x8[i]), psmpf);
                    }
                }
            }
        }
        my_printf("second_chroma_qp_index_offset: %d  (v bits)", pps->second_chroma_qp_index_offset.get()); AddTreeItem(pps->second_chroma_qp_index_offset, imrdp);
    }
    my_printf("rbsp_trailing_bits()"); AddTreeItem(0,ipps);

}

void NALParser::h264debugAUD(aud_t* aud, NALTreeNode& root)
{
    my_printf("access_unit_delimiter_rbsp()");
    auto iaud = std::make_shared<TreeNode<ParameterDescription>>("access_unit_delimiter_rbsp()");
    m_root->putChild(iaud, root);
    const char* primary_pic_type_name = NULL;
    switch (aud->primary_pic_type.get())
    {
    case AUD_PRIMARY_PIC_TYPE_I:       primary_pic_type_name = "I"; break;
    case AUD_PRIMARY_PIC_TYPE_IP:      primary_pic_type_name = "I, P"; break;
    case AUD_PRIMARY_PIC_TYPE_IPB:     primary_pic_type_name = "I, P, B"; break;
    case AUD_PRIMARY_PIC_TYPE_SI:      primary_pic_type_name = "SI"; break;
    case AUD_PRIMARY_PIC_TYPE_SISP:    primary_pic_type_name = "SI, SP"; break;
    case AUD_PRIMARY_PIC_TYPE_ISI:     primary_pic_type_name = "I, SI"; break;
    case AUD_PRIMARY_PIC_TYPE_ISIPSP:  primary_pic_type_name = "I, SI, P, SP"; break;
    case AUD_PRIMARY_PIC_TYPE_ISIPSPB: primary_pic_type_name = "I, SI, P, SP, B"; break;
    default: primary_pic_type_name = "Unknown"; break;
    }
    my_printf("primary_pic_type: %d (%s)  (3 bits)", aud->primary_pic_type.get(), primary_pic_type_name);
    AddTreeItem(aud->primary_pic_type,iaud);
}

void NALParser::h264debugSEI(h264_stream_t* h, NALTreeNode& root)
{
    sei_t** seis = h->seis;
    int num_seis = h->num_seis;
    int i = 0;

    my_printf("sei_rbsp()");
    auto isei = std::make_shared<TreeNode<ParameterDescription>>("sei_rbsp()");
    m_root->putChild(isei, root);

    my_printf("sei_message()");
    auto iisei = std::make_shared<TreeNode<ParameterDescription>>("sei_message()");
    m_root->putChild(iisei, isei);

    for (i = 0; i < num_seis; i++)
    {
        sei_t* s = seis[i];
        my_printf("payloadType: %d  (v bits)", s->payloadType); AddTreeItem(s->payloadType, iisei);
        my_printf("payloadSize: %d  (v bits)", s->payloadSize); AddTreeItem(s->payloadSize, iisei);
        my_printf("sei_payload()");
        auto sp = std::make_shared<TreeNode<ParameterDescription>>("sei_payload()");
        m_root->putChild(sp, iisei);
        switch (s->payloadType)
        {
        case SEI_TYPE_BUFFERING_PERIOD:
            my_printf("buffering_period()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_PIC_TIMING:
            my_printf("pic_timing()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_PAN_SCAN_RECT:
            my_printf("pan_scan_rect()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_FILLER_PAYLOAD:
            my_printf("filler_payload()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_USER_DATA_REGISTERED_ITU_T_T35:
            my_printf("user_data_registered_itu_t_t35()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_USER_DATA_UNREGISTERED:
        {
            char uuid[64] = { 0 };
            char tmp[8] = { 0 };
            for (int j = 0; j < 16; j++)
            {
                sprintf(tmp, "%X", s->payload[j]);
                strcat(uuid, tmp);
            }
            my_printf("uuid_iso_iec_11578: %s", uuid);

            auto udpb = std::make_shared<TreeNode<ParameterDescription>>("uuid_iso_iec_11578");
            m_root->putChild(udpb, sp);
   
            for (int j = 16; j < s->payloadSize && s->payload[j] != 0; j++)
            {
                my_printf("user_data_payload_byte: %d('%c')", s->payload[j], s->payload[j]);
                AddTreeItem(s->payload[j], sp);
            }
        }
        break;
        case SEI_TYPE_RECOVERY_POINT:
            my_printf("recovery_point()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_DEC_REF_PIC_MARKING_REPETITION:
            my_printf("Dec ref pic marking repetition()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_SPARE_PIC:
            my_printf("Spare pic()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_SCENE_INFO:
            my_printf("scene_info()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_SUB_SEQ_INFO:
            my_printf("Sub seq info()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_SUB_SEQ_LAYER_CHARACTERISTICS:
            my_printf("Sub seq layer characteristics()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_SUB_SEQ_CHARACTERISTICS:
            my_printf("Sub seq characteristics()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_FULL_FRAME_FREEZE:
            my_printf("Full frame freeze()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_FULL_FRAME_FREEZE_RELEASE:
            my_printf("Full frame freeze release()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_FULL_FRAME_SNAPSHOT:
            my_printf("picture_snapshot()"); AddTreeItem(0, sp);
            break;
        case SEI_TYPE_PROGRESSIVE_REFINEMENT_SEGMENT_START:
            my_printf("progressive_refinement_segment_start()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_PROGRESSIVE_REFINEMENT_SEGMENT_END:
            my_printf("progressive_refinement_segment_end()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_MOTION_CONSTRAINED_SLICE_GROUP_SET:
            my_printf("Motion constrained slice group set()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_FILM_GRAIN_CHARACTERISTICS:
            my_printf("film_grain_characteristics()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_DEBLOCKING_FILTER_DISPLAY_PREFERENCE:
            my_printf("Deblocking filter display preference()"); AddTreeItem(0,sp);
            break;
        case SEI_TYPE_STEREO_VIDEO_INFO:
            my_printf("Stereo video info()"); AddTreeItem(0,sp);
            break;
        default:
            my_printf("Unknown()"); AddTreeItem(0,sp);
            break;
        }
    }

    my_printf("rbsp_trailing_bits()"); AddTreeItem(0, isei);
}

