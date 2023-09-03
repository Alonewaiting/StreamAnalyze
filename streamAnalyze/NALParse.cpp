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

// 树形项
#define AddTreeItemValueZero(_item) \
   _item->m_element.m_pareterDesciption.push_back({m_outputInfo,0 });

NALParser::NALParser(STREAM_TYPE streamType) {
    m_streamType = streamType;
    m_hH264 = nullptr;
    m_hH265 = nullptr;
    if (streamType == STREAM_TYPE::STREAM_H264) {
       
        m_hH264 = h264_new();
    }
    if (streamType == STREAM_TYPE::STREAM_H265) {
        m_hH265 = h265_new();
    }
    
}
NALParser::~NALParser() {
    release();
}
int NALParser::release(void) {
    if (m_hH264) {
        h264_free(m_hH264);
    }
    if (m_hH265) {
        h265_free(m_hH265);
    }
    return 0;
}
int NALParser::parseNALU(NALUnit& vNal) {
    if (m_streamType == STREAM_TYPE::STREAM_H264) {
        read_nal_unit(m_hH264, vNal.getNALUnit().data(), vNal.getNALUnitSize());
        switch (m_hH264->sh->slice_type.get())
        {
        case 0:
        case 5:
            vNal.setSliceType(SLICE_TYPE::SLICE_P);
            break;
        case 1:
        case 6:
            vNal.setSliceType(SLICE_TYPE::SLICE_B);
            break;
        case 2:
        case 7:
            vNal.setSliceType(SLICE_TYPE::SLICE_I);
            break;
        }
        h264debugNALU(m_hH264, m_hH264->nal);
    }
    else if(m_streamType == STREAM_TYPE::STREAM_H265) {
        h265_read_nal_unit(m_hH265, vNal.getNALUnit().data(), vNal.getNALUnitSize());
        vNal.setSliceType((SLICE_TYPE)m_hH265->sh->slice_type);
        h265debugNALU(m_hH265, m_hH265->nal);
    }
    return 0;
}

void NALParser::getVideoInfo(videoinfo_t* videoInfo)
{
    if (m_streamType == STREAM_TYPE::STREAM_H264) {
       memcpy(videoInfo, m_hH264->info, sizeof(videoinfo_t));
    }
    else if (m_streamType == STREAM_TYPE::STREAM_H265) {
        memcpy(videoInfo, m_hH265->info, sizeof(videoinfo_t));

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

void NALParser::h265debugNALU(h265_stream_t* h, h265_nal_t* nal)
{
    int my_nal_type = -1;

    const char* nal_unit_type_name = NULL;

   

    switch (nal->nal_unit_type)
    {
    case NAL_UNIT_VPS:
        nal_unit_type_name = "Video parameter set";
        my_nal_type = 0;
        break;
    case NAL_UNIT_SPS:
        nal_unit_type_name = "Sequence parameter set";
        my_nal_type = 1;
        break;
    case NAL_UNIT_PPS:
        nal_unit_type_name = "Picture parameter set";
        my_nal_type = 2;
        break;
    case NAL_UNIT_AUD:
        nal_unit_type_name = "Access unit delimiter";
        my_nal_type = 3;
        break;
    case NAL_UNIT_EOS:
        nal_unit_type_name = "End of sequence";
        break;
    case NAL_UNIT_EOB:
        nal_unit_type_name = "End of bitstream";
        break;
    case NAL_UNIT_FILLER_DATA:
        nal_unit_type_name = "Filler data";
        break;
    case NAL_UNIT_PREFIX_SEI:
    case NAL_UNIT_SUFFIX_SEI:
        nal_unit_type_name = "Supplemental enhancement information";
        my_nal_type = 4;
        break;
    case NAL_UNIT_CODED_SLICE_TRAIL_N:
    case NAL_UNIT_CODED_SLICE_TRAIL_R:
        nal_unit_type_name = "Coded slice segment of a non-TSA, non-STSA trailing picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_CODED_SLICE_TSA_N:
    case NAL_UNIT_CODED_SLICE_TSA_R:
        nal_unit_type_name = "Coded slice segment of a TSA picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_CODED_SLICE_STSA_N:
    case NAL_UNIT_CODED_SLICE_STSA_R:
        nal_unit_type_name = "Coded slice segment of an STSA picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_CODED_SLICE_RADL_N:
    case NAL_UNIT_CODED_SLICE_RADL_R:
        nal_unit_type_name = "Coded slice segment of a RADL picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_CODED_SLICE_RASL_N:
    case NAL_UNIT_CODED_SLICE_RASL_R:
        nal_unit_type_name = "Coded slice segment of a RASL picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_RESERVED_VCL_N10:
    case NAL_UNIT_RESERVED_VCL_N12:
    case NAL_UNIT_RESERVED_VCL_N14:
        nal_unit_type_name = "Reserved non-IRAP SLNR VCL NAL unit types";
        my_nal_type = 5;
        break;
    case NAL_UNIT_RESERVED_VCL_R11:
    case NAL_UNIT_RESERVED_VCL_R13:
    case NAL_UNIT_RESERVED_VCL_R15:
        nal_unit_type_name = "Reserved non-IRAP sub-layer reference VCL NAL unit types";
        break;
    case NAL_UNIT_CODED_SLICE_BLA_W_LP:
    case NAL_UNIT_CODED_SLICE_BLA_W_RADL:
    case NAL_UNIT_CODED_SLICE_BLA_N_LP:
        nal_unit_type_name = "Coded slice segment of a BLA picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
    case NAL_UNIT_CODED_SLICE_IDR_N_LP:
        nal_unit_type_name = "Coded slice segment of an IDR picture";
        my_nal_type = 5;
        break;
    case NAL_UNIT_CODED_SLICE_CRA:
        nal_unit_type_name = "Coded slice segment of a CRA picture";
        break;

    case NAL_UNIT_RESERVED_IRAP_VCL22:
    case NAL_UNIT_RESERVED_IRAP_VCL23:
        nal_unit_type_name = "Reserved IRAP VCL NAL unit types";
        break;
    case NAL_UNIT_RESERVED_VCL24:
    case NAL_UNIT_RESERVED_VCL25:
    case NAL_UNIT_RESERVED_VCL26:
    case NAL_UNIT_RESERVED_VCL27:
    case NAL_UNIT_RESERVED_VCL28:
    case NAL_UNIT_RESERVED_VCL29:
    case NAL_UNIT_RESERVED_VCL30:
    case NAL_UNIT_RESERVED_VCL31:
        nal_unit_type_name = "Reserved non-IRAP VCL NAL unit types";
        break;
    case NAL_UNIT_RESERVED_NVCL41:
    case NAL_UNIT_RESERVED_NVCL42:
    case NAL_UNIT_RESERVED_NVCL43:
    case NAL_UNIT_RESERVED_NVCL44:
    case NAL_UNIT_RESERVED_NVCL45:
    case NAL_UNIT_RESERVED_NVCL46:
    case NAL_UNIT_RESERVED_NVCL47:
        nal_unit_type_name = "Reserved";
        break;
    case NAL_UNIT_UNSPECIFIED_48:
    case NAL_UNIT_UNSPECIFIED_49:
    case NAL_UNIT_UNSPECIFIED_50:
    case NAL_UNIT_UNSPECIFIED_51:
    case NAL_UNIT_UNSPECIFIED_52:
    case NAL_UNIT_UNSPECIFIED_53:
    case NAL_UNIT_UNSPECIFIED_54:
    case NAL_UNIT_UNSPECIFIED_55:
    case NAL_UNIT_UNSPECIFIED_56:
    case NAL_UNIT_UNSPECIFIED_57:
    case NAL_UNIT_UNSPECIFIED_58:
    case NAL_UNIT_UNSPECIFIED_59:
    case NAL_UNIT_UNSPECIFIED_60:
    case NAL_UNIT_UNSPECIFIED_61:
    case NAL_UNIT_UNSPECIFIED_62:
    case NAL_UNIT_UNSPECIFIED_63:
        nal_unit_type_name = "Unspecified";
        break;
    default:
        nal_unit_type_name = "Unknown";
        break;
    }
    // 根节点
    this->m_root = std::make_shared<TreeList<ParameterDescription>>("NAL");
    auto rootNode = this->m_root->getRoot();

    // 根据手册，nal头是一个节点
    my_printf("nal_unit_header");
    auto subroot = std::make_shared<TreeNode<ParameterDescription>>("nal_unit_header");
    this->m_root->putChild(subroot, rootNode);

    my_printf("forbidden_zero_bit: %d (1 bit)", nal->forbidden_zero_bit); AddTreeItemValueZero(subroot);
    my_printf("nal_unit_type: %d (%s) (6 bits)", nal->nal_unit_type, nal_unit_type_name); AddTreeItemValueZero(subroot);
    my_printf("nuh_layer_id: %d (6 bits)", nal->nuh_layer_id); AddTreeItemValueZero(subroot);
    my_printf("nuh_temporal_id_plus1: %d (3 bits)", nal->nuh_temporal_id_plus1); AddTreeItemValueZero(subroot);

    // nal具体的内容也是一个节点
    if (my_nal_type == 0)
        h265DebugVps(h->vps, rootNode);
    else if (my_nal_type == 1)
        h265DebugSps(h->sps, rootNode);
    else if (my_nal_type == 2)
        h265DebugPps(h->pps, rootNode);
    else if (my_nal_type == 3)
        h265DebugAud(h->aud, rootNode);
    else if (my_nal_type == 4)
        h265DebugSeis(h, rootNode);
    else if (my_nal_type == 5)
        h265DebugSliceHeader(h, rootNode);
}

void NALParser::h265DebugPtl(profile_tier_level_t* ptl, int profilePresentFlag, int max_sub_layers_minus1, NALTreeNode& root)
{
}

void NALParser::h265DebugSubLayerHrdParameters(sub_layer_hrd_parameters_t* subhrd, int sub_pic_hrd_params_present_flag, int CpbCnt, int subLayerId, const char* p, NALTreeNode& root)
{
}

void NALParser::h265DebugHrdParameters(hrd_parameters_t* hrd, int commonInfPresentFlag, int maxNumSubLayersMinus1, NALTreeNode& root)
{
}

void NALParser::h265DebugVps(h265_vps_t* vps, NALTreeNode& root)
{
    int i, j;
    my_printf("video_parameter_set_rbsp()");
    auto ivps = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ivps, root);

    my_printf("vps_video_parameter_set_id: %d  (4 bits)", vps->vps_video_parameter_set_id); AddTreeItemValueZero(ivps);
    my_printf_flag("vps_base_layer_internal_flag", vps->vps_base_layer_internal_flag); AddTreeItemValueZero(ivps);
    my_printf_flag("vps_base_layer_available_flag", vps->vps_base_layer_available_flag); AddTreeItemValueZero(ivps);
    my_printf("vps_max_layers_minus1: %d  (6 bits)", vps->vps_max_layers_minus1); AddTreeItemValueZero(ivps);
    my_printf("vps_max_sub_layers_minus1: %d  (3 bits)", vps->vps_max_sub_layers_minus1); AddTreeItemValueZero(ivps);
    my_printf_flag("vps_temporal_id_nesting_flag", vps->vps_temporal_id_nesting_flag); AddTreeItemValueZero(ivps);
    my_printf("vps_reserved_0xffff_16bits: %d  (16 bits)", vps->vps_reserved_0xffff_16bits); AddTreeItemValueZero(ivps);
    // ptl
    my_printf("profile_tier_level()");
    auto iptl = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(iptl, ivps);

    h265DebugPtl(&vps->ptl, 1, vps->vps_max_layers_minus1, iptl);

    my_printf_flag("vps_sub_layer_ordering_info_present_flag", vps->vps_sub_layer_ordering_info_present_flag);
    AddTreeItemValueZero(ivps);

    if (vps->vps_sub_layer_ordering_info_present_flag)
    {
        my_printf("SubLayers");
        auto isub = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(isub, ivps);

        for (i = (vps->vps_sub_layer_ordering_info_present_flag ? 0 : vps->vps_max_sub_layers_minus1);
            i <= vps->vps_max_sub_layers_minus1; i++)
        {
            my_printf("vps_max_dec_pic_buffering_minus1[%d]: %d  (v bits)", i, vps->vps_max_dec_pic_buffering_minus1[i]); AddTreeItemValueZero(isub);
            my_printf("vps_max_num_reorder_pics[%d]: %d  (v bits)", i, vps->vps_max_num_reorder_pics[i]); AddTreeItemValueZero(isub);
            my_printf("vps_max_latency_increase_plus1[%d]: %d  (v bits)", i, vps->vps_max_latency_increase_plus1[i]); AddTreeItemValueZero(isub);
        }
    }

    my_printf("vps_max_layer_id: %d  (6 bits)", vps->vps_max_layer_id); AddTreeItemValueZero(ivps);
    my_printf("vps_num_layer_sets_minus1: %d  (v bits)", vps->vps_num_layer_sets_minus1); AddTreeItemValueZero(ivps);
    for (i = 1; i <= vps->vps_num_layer_sets_minus1; i++)
    {
        for (j = 0; j <= vps->vps_max_layer_id; j++)
        {
            my_printf_flag3("layer_id_included_flag", i, j, vps->layer_id_included_flag[i][j]); AddTreeItemValueZero(ivps);
        }
    }
    my_printf_flag("vps_timing_info_present_flag", vps->vps_timing_info_present_flag);
    auto tipf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(tipf, ivps);

    if (vps->vps_timing_info_present_flag)
    {
        my_printf("vps_num_units_in_tick: %d  (32 bits)", vps->vps_num_units_in_tick); AddTreeItemValueZero(tipf);
        my_printf("vps_time_scale: %d  (32 bits)", vps->vps_time_scale); AddTreeItemValueZero(tipf);
        my_printf_flag("vps_poc_proportional_to_timing_flag", vps->vps_poc_proportional_to_timing_flag);
        auto ppttf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(ppttf, tipf);


        if (vps->vps_poc_proportional_to_timing_flag)
        {
            my_printf("vps_num_ticks_poc_diff_one_minus1: %d  (v bits)", vps->vps_num_ticks_poc_diff_one_minus1); AddTreeItemValueZero(ppttf);
        }
        my_printf("vps_num_hrd_parameters: %d  (v bits)", vps->vps_num_hrd_parameters); AddTreeItemValueZero(tipf);
        for (i = 0; i < vps->vps_num_hrd_parameters; i++)
        {
            my_printf("hrd_layer_set_idx[%d]: %d  (v bits)", i, vps->hrd_layer_set_idx[i]); AddTreeItemValueZero(tipf);
            if (i > 0)
            {
                my_printf_flag2("cprms_present_flag", i, vps->cprms_present_flag[i]); AddTreeItemValueZero(tipf);
            }
            //  hrd_parameters()
            h265DebugHrdParameters(&(vps->hrd_parameters), vps->cprms_present_flag[i], vps->vps_max_sub_layers_minus1, tipf);
        }
    }
    my_printf_flag("vps_extension_flag", vps->vps_extension_flag); AddTreeItemValueZero(ivps);
    if (vps->vps_extension_flag)
    {
        // do nothing...
    }

    my_printf("rbsp_trailing_bits()"); AddTreeItemValueZero(ivps);
}

void NALParser::h265DebugScalingList(scaling_list_data_t* sld, NALTreeNode& root)
{
    my_printf("scaling_list_data()");
    auto isl = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(isl, root);

    for (int sizeId = 0; sizeId < 4; sizeId++)
    {
        for (int matrixId = 0; matrixId < 6; matrixId += (sizeId == 3) ? 3 : 1)
        {
            my_printf_flag3("scaling_list_pred_mode_flag", sizeId, matrixId, sld->scaling_list_pred_mode_flag[sizeId][matrixId]);
            AddTreeItemValueZero(isl);
            if (!sld->scaling_list_pred_mode_flag[sizeId][matrixId])
            {
                my_printf_flag3("scaling_list_pred_matrix_id_delta[%d][%d]: %d  (v bits)", sizeId, matrixId, sld->scaling_list_pred_matrix_id_delta[sizeId][matrixId]);
                AddTreeItemValueZero(isl);
            }
            else
            {
                if (sizeId > 1)
                {
                    my_printf("scaling_list_dc_coef_minus8[%d][%d]: %d  (v bits)", sizeId, matrixId, sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId]);
                    AddTreeItemValueZero(isl);
                }
                for (int i = 0; i < sld->coefNum; i++)
                {
                    my_printf("ScalingList[%d][%d][%d]: %d  (v bits)", sizeId, matrixId, i, sld->ScalingList[sizeId][matrixId][i]);
                    AddTreeItemValueZero(isl);
                }
            }
        }
    }
}

void NALParser::h265DebugShortTermRefPicSet(h265_sps_t* sps, st_ref_pic_set_t* st, referencePictureSets_t* rps, int stRpsIdx, NALTreeNode& root)
{
    my_printf("short_term_ref_pic_set( %d )", stRpsIdx);
    auto srps = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(srps, root);
    auto srpsii = srps;
    if (stRpsIdx != 0)
    {
        my_printf_flag("inter_ref_pic_set_prediction_flag", st->inter_ref_pic_set_prediction_flag); AddTreeItemValueZero(srps);
        auto srpsii = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);

    }
    if (st->inter_ref_pic_set_prediction_flag)
    {
        my_printf("delta_idx_minus1: %d (v bits)", st->delta_idx_minus1); AddTreeItemValueZero(srpsii);
        my_printf("delta_rps_sign: %d (1 bit)", st->delta_rps_sign); AddTreeItemValueZero(srpsii);
        my_printf("abs_delta_rps_minus1: %d (v bits)", st->abs_delta_rps_minus1); AddTreeItemValueZero(srpsii);
        int rIdx = stRpsIdx - 1 - st->delta_idx_minus1;
        referencePictureSets_t* rpsRef = &sps->m_RPSList[rIdx];
        for (int j = 0; j <= rpsRef->m_numberOfPictures; j++)
        {
            my_printf_flag2("used_by_curr_pic_flag", j, st->used_by_curr_pic_flag[j]); AddTreeItemValueZero(srpsii);
            if (!st->used_by_curr_pic_flag[j])
            {
                my_printf_flag2("use_delta_flag", j, st->use_delta_flag[j]); AddTreeItemValueZero(srpsii);
            }
        }
    }
    else
    {
        my_printf("num_negative_pics: %d (v bits)", st->num_negative_pics); AddTreeItemValueZero(srpsii);
        my_printf("num_positive_pics: %d (v bits)", st->num_positive_pics); AddTreeItemValueZero(srpsii);
        for (int i = 0; i < st->num_negative_pics; i++)
        {
            my_printf("delta_poc_s0_minus1[%d]: %d  (v bits)", i, st->delta_poc_s0_minus1[i]); AddTreeItemValueZero(srpsii);
            my_printf_flag2("used_by_curr_pic_s0_flag", i, st->used_by_curr_pic_s0_flag[i]); AddTreeItemValueZero(srpsii);
        }
        for (int i = 0; i < st->num_positive_pics; i++)
        {
            my_printf("delta_poc_s1_minus1[%d]: %d (v bits)", i, st->delta_poc_s1_minus1[i]); AddTreeItemValueZero(srpsii);
            my_printf_flag2("used_by_curr_pic_s1_flag", i, st->used_by_curr_pic_s1_flag[i]); AddTreeItemValueZero(srpsii);
        }
    }
}

void NALParser::h265DebugVuiParameters(vui_parameters_t* vui, int maxNumSubLayersMinus1, NALTreeNode& root)
{
    my_printf("vui_parameters()");
    auto ivp = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ivp, root);

    my_printf_flag("aspect_ratio_info_present_flag", vui->aspect_ratio_info_present_flag);
    auto aripf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(aripf, ivp);

    if (vui->aspect_ratio_info_present_flag)
    {
        my_printf("aspect_ratio_idc: %d  (8 bits)", vui->aspect_ratio_idc); AddTreeItemValueZero(aripf);
        if (vui->aspect_ratio_idc == H265_SAR_Extended)
        {
            my_printf("sar_width: %d   (16 bits)", vui->sar_width); AddTreeItemValueZero(aripf);
            my_printf("sar_height: %d  (16 bits)", vui->sar_height); AddTreeItemValueZero(aripf);
        }
    }
    my_printf_flag("overscan_info_present_flag", vui->overscan_info_present_flag);
    auto oipf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(oipf, ivp);


    if (vui->overscan_info_present_flag)
    {
        my_printf_flag("overscan_appropriate_flag", vui->overscan_appropriate_flag); AddTreeItemValueZero(oipf);
    }
    my_printf_flag("video_signal_type_present_flag", vui->video_signal_type_present_flag);

    auto vstpf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(vstpf, ivp);


    if (vui->video_signal_type_present_flag)
    {
        my_printf("video_format: %d  (3 bits)", vui->video_format); AddTreeItemValueZero(vstpf);
        my_printf_flag("video_full_range_flag", vui->video_full_range_flag); AddTreeItemValueZero(vstpf);
        my_printf_flag("colour_description_present_flag", vui->colour_description_present_flag);

        auto cdpf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(cdpf, vstpf);


        if (vui->colour_description_present_flag)
        {
            my_printf("colour_primaries: %d  (8 bits)", vui->colour_primaries); AddTreeItemValueZero(cdpf);
            my_printf("transfer_characteristics: %d  (8 bits)", vui->transfer_characteristics); AddTreeItemValueZero(cdpf);
            my_printf("matrix_coeffs: %d  (8 bits)", vui->matrix_coeffs); AddTreeItemValueZero(cdpf);
        }
    }
    my_printf_flag("chroma_loc_info_present_flag", vui->chroma_loc_info_present_flag);

    auto clipf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(clipf, ivp);
    if (vui->chroma_loc_info_present_flag)
    {
        my_printf("chroma_sample_loc_type_top_field: %d  (v bits)", vui->chroma_sample_loc_type_top_field);
        AddTreeItemValueZero(clipf);
        my_printf("chroma_sample_loc_type_bottom_field: %d  (v bits)", vui->chroma_sample_loc_type_bottom_field);
        AddTreeItemValueZero(clipf);
    }
    my_printf_flag("neutral_chroma_indication_flag", vui->neutral_chroma_indication_flag); AddTreeItemValueZero(ivp);
    my_printf_flag("field_seq_flag", vui->field_seq_flag); AddTreeItemValueZero(ivp);
    my_printf_flag("frame_field_info_present_flag", vui->frame_field_info_present_flag); AddTreeItemValueZero(ivp);
    my_printf_flag("default_display_window_flag", vui->default_display_window_flag);

    auto ddwf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ddwf, ivp);

    if (vui->default_display_window_flag)
    {
        my_printf("def_disp_win_left_offset: %d  (v bits)", vui->def_disp_win_left_offset); AddTreeItemValueZero(ddwf);
        my_printf("def_disp_win_right_offset: %d  (v bits)", vui->def_disp_win_right_offset); AddTreeItemValueZero(ddwf);
        my_printf("def_disp_win_top_offset: %d  (v bits)", vui->def_disp_win_top_offset); AddTreeItemValueZero(ddwf);
        my_printf("def_disp_win_bottom_offset: %d  (v bits)", vui->def_disp_win_bottom_offset); AddTreeItemValueZero(ddwf);
    }
    my_printf_flag("vui_timing_info_present_flag", vui->vui_timing_info_present_flag);

    auto vtipf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(vtipf, ivp);


    if (vui->vui_timing_info_present_flag)
    {
        my_printf("vui_num_units_in_tick: %u  (32 bits)", vui->vui_num_units_in_tick); AddTreeItemValueZero(vtipf);
        my_printf("vui_time_scale: %u  (32 bits)", vui->vui_time_scale); AddTreeItemValueZero(vtipf);
        my_printf_flag("vui_poc_proportional_to_timing_flag", vui->vui_poc_proportional_to_timing_flag);

        auto vppttf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(vppttf, vtipf);

        if (vui->vui_poc_proportional_to_timing_flag)
        {
            my_printf("vui_num_ticks_poc_diff_one_minus1: %d  (v bits)", vui->vui_num_ticks_poc_diff_one_minus1);
            AddTreeItemValueZero(vppttf);
        }
        my_printf_flag("vui_hrd_parameters_present_flag", vui->vui_hrd_parameters_present_flag);
        auto vhppf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(vhppf, vtipf);



        if (vui->vui_hrd_parameters_present_flag)
        {
            h265DebugHrdParameters(&vui->hrd_parameters, 1, maxNumSubLayersMinus1, vhppf);
        }
    }
    my_printf_flag("bitstream_restriction_flag", vui->bitstream_restriction_flag);

    auto brf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(brf, vtipf);

    if (vui->bitstream_restriction_flag)
    {
        my_printf_flag("tiles_fixed_structure_flag", vui->tiles_fixed_structure_flag); AddTreeItemValueZero(brf);
        my_printf_flag("motion_vectors_over_pic_boundaries_flag", vui->motion_vectors_over_pic_boundaries_flag); AddTreeItemValueZero(brf);
        my_printf_flag("restricted_ref_pic_lists_flag", vui->restricted_ref_pic_lists_flag); AddTreeItemValueZero(brf);
        my_printf("min_spatial_segmentation_idc: %d  (v bits)", vui->min_spatial_segmentation_idc); AddTreeItemValueZero(brf);
        my_printf("max_bytes_per_pic_denom: %d  (v bits)", vui->max_bytes_per_pic_denom); AddTreeItemValueZero(brf);
        my_printf("max_bits_per_min_cu_denom: %d  (v bits)", vui->max_bits_per_min_cu_denom); AddTreeItemValueZero(brf);
        my_printf("log2_max_mv_length_horizontal: %d  (v bits)", vui->log2_max_mv_length_horizontal); AddTreeItemValueZero(brf);
        my_printf("log2_max_mv_length_vertical: %d  (v bits)", vui->bitstream_restriction_flag); AddTreeItemValueZero(brf);
    }
}

void NALParser::h265DebugSps(h265_sps_t* sps, NALTreeNode& root)
{
    my_printf("seq_parameter_set_rbsp()");
    auto isps = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(isps, root);


    my_printf("sps_video_parameter_set_id: %d  (4 bits)", sps->sps_video_parameter_set_id); AddTreeItemValueZero(isps);
    my_printf("sps_max_sub_layers_minus1: %d  (4 bits)", sps->sps_max_sub_layers_minus1); AddTreeItemValueZero(isps);
    my_printf_flag("sps_temporal_id_nesting_flag", sps->sps_temporal_id_nesting_flag); AddTreeItemValueZero(isps);
    // ptl
    my_printf("profile_tier_level()");

    auto iptl = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(iptl, isps);

    h265DebugPtl(&sps->ptl, 1, sps->sps_max_sub_layers_minus1, iptl);

    my_printf("sps_seq_parameter_set_id: %d  (v bits)", sps->sps_seq_parameter_set_id); AddTreeItemValueZero(isps);
    my_printf("chroma_format_idc: %d  (v bits)", sps->chroma_format_idc); AddTreeItemValueZero(isps);
    if (sps->chroma_format_idc == 3)
    {
        my_printf_flag("separate_colour_plane_flag", sps->separate_colour_plane_flag); AddTreeItemValueZero(isps);
    }
    my_printf("pic_width_in_luma_samples: %d  (v bits)", sps->pic_width_in_luma_samples); AddTreeItemValueZero(isps);
    my_printf("pic_height_in_luma_samples: %d  (v bits)", sps->pic_height_in_luma_samples); AddTreeItemValueZero(isps);
    my_printf_flag("conformance_window_flag", sps->conformance_window_flag);

    auto cwf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(cwf, isps);


    if (sps->conformance_window_flag)
    {
        my_printf("conf_win_left_offset: %d  (v bits)", sps->conf_win_left_offset); AddTreeItemValueZero(cwf);
        my_printf("conf_win_right_offset: %d  (v bits)", sps->conf_win_right_offset); AddTreeItemValueZero(cwf);
        my_printf("conf_win_top_offset: %d  (v bits)", sps->conf_win_top_offset); AddTreeItemValueZero(cwf);
        my_printf("conf_win_bottom_offset: %d  (v bits)", sps->conf_win_bottom_offset); AddTreeItemValueZero(cwf);
    }
    my_printf("bit_depth_luma_minus8: %d  (v bits)", sps->bit_depth_luma_minus8); AddTreeItemValueZero(isps);
    my_printf("bit_depth_chroma_minus8: %d  (v bits)", sps->bit_depth_chroma_minus8); AddTreeItemValueZero(isps);
    my_printf("log2_max_pic_order_cnt_lsb_minus4: %d  (v bits)", sps->log2_max_pic_order_cnt_lsb_minus4); AddTreeItemValueZero(isps);
    my_printf_flag("sps_sub_layer_ordering_info_present_flag", sps->sps_sub_layer_ordering_info_present_flag); AddTreeItemValueZero(isps);
    my_printf("SubLayers");

    auto isloripf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(isloripf, isps);


    for (int i = (sps->sps_sub_layer_ordering_info_present_flag ? 0 : sps->sps_max_sub_layers_minus1);
        i <= sps->sps_max_sub_layers_minus1; i++)
    {
        my_printf("sps_max_dec_pic_buffering_minus1[%d]: %d  (v bits)", i, sps->sps_max_dec_pic_buffering_minus1[i]); AddTreeItemValueZero(isloripf);
        my_printf("sps_max_num_reorder_pics[%d]: %d  (v bits)", i, sps->sps_max_num_reorder_pics[i]); AddTreeItemValueZero(isloripf);
        my_printf("sps_max_latency_increase_plus1[%d]: %d  (v bits)", i, sps->sps_max_latency_increase_plus1[i]); AddTreeItemValueZero(isloripf);
    }
    my_printf("log2_min_luma_coding_block_size_minus3: %d  (v bits)", sps->log2_min_luma_coding_block_size_minus3); AddTreeItemValueZero(isps);
    my_printf("log2_diff_max_min_luma_coding_block_size: %d  (v bits)", sps->log2_diff_max_min_luma_coding_block_size); AddTreeItemValueZero(isps);
    my_printf("log2_min_luma_transform_block_size_minus2: %d  (v bits)", sps->log2_min_luma_transform_block_size_minus2); AddTreeItemValueZero(isps);
    my_printf("log2_diff_max_min_luma_transform_block_size: %d  (v bits)", sps->log2_diff_max_min_luma_transform_block_size); AddTreeItemValueZero(isps);
    my_printf("max_transform_hierarchy_depth_inter: %d  (v bits)", sps->max_transform_hierarchy_depth_inter); AddTreeItemValueZero(isps);
    my_printf("max_transform_hierarchy_depth_intra: %d  (v bits)", sps->max_transform_hierarchy_depth_intra); AddTreeItemValueZero(isps);
    my_printf_flag("scaling_list_enabled_flag", sps->scaling_list_enabled_flag);

    auto slef = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(slef, isps);

    if (sps->scaling_list_enabled_flag)
    {
        my_printf_flag("sps_scaling_list_data_present_flag", sps->sps_scaling_list_data_present_flag);
        auto ssldpf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(ssldpf, slef);

        if (sps->sps_scaling_list_data_present_flag)
        {
            h265DebugScalingList(&sps->scaling_list_data, ssldpf);
        }
    }

    my_printf_flag("amp_enabled_flag", sps->amp_enabled_flag); AddTreeItemValueZero(isps);
    my_printf_flag("sample_adaptive_offset_enabled_flag", sps->sample_adaptive_offset_enabled_flag); AddTreeItemValueZero(isps);
    my_printf_flag("pcm_enabled_flag", sps->pcm_enabled_flag);

    auto pef = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(pef, isps);

    if (sps->pcm_enabled_flag)
    {
        my_printf("pcm_sample_bit_depth_luma_minus1: %d  (4 bits)", sps->pcm_sample_bit_depth_luma_minus1); AddTreeItemValueZero(pef);
        my_printf("pcm_sample_bit_depth_chroma_minus1: %d  (4 bits)", sps->pcm_sample_bit_depth_chroma_minus1); AddTreeItemValueZero(pef);
        my_printf("log2_min_pcm_luma_coding_block_size_minus3: %d  (v bits)", sps->log2_min_pcm_luma_coding_block_size_minus3); AddTreeItemValueZero(pef);
        my_printf("log2_diff_max_min_pcm_luma_coding_block_size: %d  (v bits)", sps->log2_diff_max_min_pcm_luma_coding_block_size); AddTreeItemValueZero(pef);
        my_printf_flag("pcm_loop_filter_disabled_flag", sps->pcm_loop_filter_disabled_flag); AddTreeItemValueZero(pef);
    }
    my_printf("num_short_term_ref_pic_sets: %d  (v bits)", sps->num_short_term_ref_pic_sets);
    referencePictureSets_t* rps = NULL;
    st_ref_pic_set_t* st = NULL;
    auto nstrps = isps;
    if (sps->num_short_term_ref_pic_sets > 0)
    {

        auto nstrps = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(nstrps, isps);

    }
    for (int i = 0; i < sps->num_short_term_ref_pic_sets; i++)
    {
        st = &sps->st_ref_pic_set[i];
        rps = &sps->m_RPSList[i];
        h265DebugShortTermRefPicSet(sps, st, rps, i, nstrps);
    }
    my_printf_flag("long_term_ref_pics_present_flag", sps->long_term_ref_pics_present_flag);
    auto ltrppf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ltrppf, isps);


    if (sps->long_term_ref_pics_present_flag)
    {
        my_printf("num_long_term_ref_pics_sps: %d  (v bits)", sps->num_long_term_ref_pics_sps);
        AddTreeItemValueZero(ltrppf);
        for (int i = 0; i < sps->num_long_term_ref_pics_sps; i++)
        {
            my_printf("lt_ref_pic_poc_lsb_sps[%d]: %d  (u %d bits)", i, sps->lt_ref_pic_poc_lsb_sps[i], sps->lt_ref_pic_poc_lsb_sps_bytes);
            AddTreeItemValueZero(ltrppf);
            my_printf_flag2("used_by_curr_pic_lt_sps_flag", i, sps->used_by_curr_pic_lt_sps_flag[i]);
            AddTreeItemValueZero(ltrppf);
        }
    }
    my_printf_flag("sps_temporal_mvp_enabled_flag", sps->sps_temporal_mvp_enabled_flag); AddTreeItemValueZero(isps);
    my_printf_flag("strong_intra_smoothing_enabled_flag", sps->strong_intra_smoothing_enabled_flag); AddTreeItemValueZero(isps);
    my_printf_flag("vui_parameters_present_flag", sps->vui_parameters_present_flag);

    auto vppf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(vppf, isps);

    if (sps->vui_parameters_present_flag)
    {
        // vui
        h265DebugVuiParameters(&sps->vui, sps->sps_max_sub_layers_minus1, vppf);
    }
    my_printf_flag("sps_extension_present_flag", sps->sps_extension_present_flag);

    auto sepf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(sepf, isps);

    if (sps->sps_extension_present_flag)
    {
        my_printf_flag("sps_range_extension_flag", sps->sps_range_extension_flag);

        auto sref = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(sref, sepf);

        if (sps->sps_range_extension_flag)
        {
            my_printf_flag("transform_skip_rotation_enabled_flag", sps->sps_range_extension.transform_skip_rotation_enabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("transform_skip_context_enabled_flag", sps->sps_range_extension.transform_skip_context_enabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("implicit_rdpcm_enabled_flag", sps->sps_range_extension.implicit_rdpcm_enabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("explicit_rdpcm_enabled_flag", sps->sps_range_extension.explicit_rdpcm_enabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("extended_precision_processing_flag", sps->sps_range_extension.extended_precision_processing_flag); AddTreeItemValueZero(sref);
            my_printf_flag("intra_smoothing_disabled_flag", sps->sps_range_extension.intra_smoothing_disabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("high_precision_offsets_enabled_flag", sps->sps_range_extension.high_precision_offsets_enabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("persistent_rice_adaptation_enabled_flag", sps->sps_range_extension.persistent_rice_adaptation_enabled_flag); AddTreeItemValueZero(sref);
            my_printf_flag("cabac_bypass_alignment_enabled_flag", sps->sps_range_extension.cabac_bypass_alignment_enabled_flag); AddTreeItemValueZero(sref);
        }
        my_printf_flag("sps_multilayer_extension_flag", sps->sps_multilayer_extension_flag);
        auto smef = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(smef, sepf);


        if (sps->sps_multilayer_extension_flag)
        {
            my_printf_flag("inter_view_mv_vert_constraint_flag", sps->inter_view_mv_vert_constraint_flag);
            AddTreeItemValueZero(smef);
        }
        my_printf_flag("sps_3d_extension_flag", sps->sps_3d_extension_flag); AddTreeItemValueZero(sepf);
        my_printf("sps_extension_5bits: %d  (5 bits)", sps->sps_extension_5bits); AddTreeItemValueZero(sepf);
    }


    // todo sps_3d_extension_flag

    my_printf("rbsp_trailing_bits()"); AddTreeItemValueZero(isps);

}

void NALParser::h265DebugPps(h265_pps_t* pps, NALTreeNode& root)
{
    my_printf("pic_parameter_set_rbsp()");
    auto ipps = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ipps, root);

    my_printf("pps_pic_parameter_set_id: %d  (v bits)", pps->pps_pic_parameter_set_id); AddTreeItemValueZero(ipps);
    my_printf("pps_seq_parameter_set_id: %d  (v bits)", pps->pps_seq_parameter_set_id); AddTreeItemValueZero(ipps);
    my_printf_flag("dependent_slice_segments_enabled_flag", pps->dependent_slice_segments_enabled_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("output_flag_present_flag", pps->output_flag_present_flag); AddTreeItemValueZero(ipps);
    my_printf("num_extra_slice_header_bits: %d  (3 bits)", pps->num_extra_slice_header_bits); AddTreeItemValueZero(ipps);
    my_printf_flag("sign_data_hiding_enabled_flag", pps->sign_data_hiding_enabled_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("cabac_init_present_flag", pps->cabac_init_present_flag); AddTreeItemValueZero(ipps);
    my_printf("num_ref_idx_l0_default_active_minus1: %d  (v bits)", pps->num_ref_idx_l0_default_active_minus1); AddTreeItemValueZero(ipps);
    my_printf("num_ref_idx_l1_default_active_minus1: %d  (v bits)", pps->num_ref_idx_l1_default_active_minus1); AddTreeItemValueZero(ipps);
    my_printf("init_qp_minus26: %d  (v bits)", pps->init_qp_minus26); AddTreeItemValueZero(ipps);
    my_printf_flag("constrained_intra_pred_flag", pps->constrained_intra_pred_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("transform_skip_enabled_flag", pps->transform_skip_enabled_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("cu_qp_delta_enabled_flag", pps->cu_qp_delta_enabled_flag);

    auto cqdef = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(cqdef, ipps);

    if (pps->cu_qp_delta_enabled_flag)
    {
        my_printf("diff_cu_qp_delta_depth: %d  (v bits)", pps->diff_cu_qp_delta_depth);
        AddTreeItemValueZero(cqdef);
    }
    my_printf("pps_cb_qp_offset: %d  (v bits)", pps->pps_cb_qp_offset); AddTreeItemValueZero(ipps);
    my_printf("pps_cr_qp_offset: %d  (v bits)", pps->pps_cr_qp_offset); AddTreeItemValueZero(ipps);
    my_printf_flag("pps_slice_chroma_qp_offsets_present_flag", pps->pps_slice_chroma_qp_offsets_present_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("weighted_pred_flag", pps->weighted_pred_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("weighted_bipred_flag", pps->weighted_bipred_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("transquant_bypass_enabled_flag", pps->transquant_bypass_enabled_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("tiles_enabled_flag", pps->tiles_enabled_flag);

    auto tef = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(tef, ipps);

    if (pps->tiles_enabled_flag)
    {
        my_printf("num_tile_columns_minus1: %d  (v bits)", pps->num_tile_columns_minus1); AddTreeItemValueZero(tef);
        my_printf("num_tile_rows_minus1: %d  (v bits)", pps->num_tile_rows_minus1); AddTreeItemValueZero(tef);
        my_printf_flag("uniform_spacing_flag", pps->uniform_spacing_flag);

        auto usf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(usf, tef);

       
        if (!pps->uniform_spacing_flag)
        {
            for (int i = 0; i < pps->num_tile_columns_minus1; i++)
            {
                my_printf("column_width_minus1[%d]: %d  (v bits)", i, pps->column_width_minus1[i]); AddTreeItemValueZero(usf);
            }
            for (int i = 0; i < pps->num_tile_rows_minus1; i++)
            {
                my_printf("row_height_minus1[%d]: %d  (v bits)", i, pps->row_height_minus1[i]); AddTreeItemValueZero(usf);
            }
        }
        my_printf_flag("loop_filter_across_tiles_enabled_flag", pps->loop_filter_across_tiles_enabled_flag); // to check
        AddTreeItemValueZero(tef);
    }
    my_printf_flag("entropy_coding_sync_enabled_flag", pps->entropy_coding_sync_enabled_flag); AddTreeItemValueZero(ipps);

    my_printf_flag("pps_loop_filter_across_slices_enabled_flag", pps->pps_loop_filter_across_slices_enabled_flag); // to check
    AddTreeItemValueZero(ipps);
    my_printf_flag("deblocking_filter_control_present_flag", pps->deblocking_filter_control_present_flag);


    auto dfcpf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(dfcpf, ipps);


    if (pps->deblocking_filter_control_present_flag)
    {
        my_printf_flag("deblocking_filter_override_enabled_flag", pps->deblocking_filter_override_enabled_flag);
        AddTreeItemValueZero(dfcpf);
        my_printf_flag("pps_deblocking_filter_disabled_flag", pps->pps_deblocking_filter_disabled_flag);

        auto dfdf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(dfdf, dfcpf);

        if (pps->pps_deblocking_filter_disabled_flag)
        {
            my_printf("pps_beta_offset_div2: %d  (v bits)", pps->pps_beta_offset_div2); AddTreeItemValueZero(dfdf);
            my_printf("pps_tc_offset_div2: %d  (v bits)", pps->pps_tc_offset_div2); AddTreeItemValueZero(dfdf);
        }
    }
    my_printf_flag("pps_scaling_list_data_present_flag", pps->pps_scaling_list_data_present_flag);

    auto sldf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(sldf, ipps);

    if (pps->pps_scaling_list_data_present_flag)
    {
        // scaling_list_data()
        h265DebugScalingList(&pps->scaling_list_data, sldf);
    }
    my_printf_flag("lists_modification_present_flag", pps->lists_modification_present_flag); AddTreeItemValueZero(ipps);
    my_printf("log2_parallel_merge_level_minus2: %d  (v bits)", pps->log2_parallel_merge_level_minus2); AddTreeItemValueZero(ipps);
    my_printf_flag("slice_segment_header_extension_present_flag", pps->slice_segment_header_extension_present_flag); AddTreeItemValueZero(ipps);
    my_printf_flag("pps_extension_present_flag", pps->pps_extension_present_flag);

    auto epf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(epf, ipps);

    if (pps->pps_extension_present_flag)
    {
        my_printf_flag("pps_range_extension_flag", pps->pps_range_extension_flag); AddTreeItemValueZero(epf);
        auto iref = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(iref, epf);


        if (pps->pps_range_extension_flag)
        {
            if (pps->transform_skip_enabled_flag)
            {
                my_printf("log2_max_transform_skip_block_size_minus2: %d  (v bits)", pps->pps_range_extension.log2_max_transform_skip_block_size_minus2);
                AddTreeItemValueZero(iref);
            }
            my_printf_flag("cross_component_prediction_enabled_flag", pps->pps_range_extension.cross_component_prediction_enabled_flag); AddTreeItemValueZero(iref);
            my_printf_flag("chroma_qp_offset_list_enabled_flag", pps->pps_range_extension.chroma_qp_offset_list_enabled_flag); AddTreeItemValueZero(iref);
            if (pps->pps_range_extension.chroma_qp_offset_list_enabled_flag)
            {
                my_printf("diff_cu_chroma_qp_offset_depth: %d  (v bits)", pps->pps_range_extension.diff_cu_chroma_qp_offset_depth); AddTreeItemValueZero(iref);
                my_printf("chroma_qp_offset_list_len_minus1: %d  (v bits)", pps->pps_range_extension.chroma_qp_offset_list_len_minus1); AddTreeItemValueZero(iref);
                for (int i = 0; i < pps->pps_range_extension.chroma_qp_offset_list_len_minus1; i++)
                {
                    my_printf("cb_qp_offset_list[%d]: %d  (v bits)", i, pps->pps_range_extension.cb_qp_offset_list[i]); AddTreeItemValueZero(iref);
                    my_printf("cr_qp_offset_list[%d]: %d  (v bits)", i, pps->pps_range_extension.cb_qp_offset_list[i]); AddTreeItemValueZero(iref);
                }
            }
            my_printf("log2_sao_offset_scale_luma: %d  (v bits)", pps->pps_range_extension.log2_sao_offset_scale_luma); AddTreeItemValueZero(iref);
            my_printf("log2_sao_offset_scale_chroma: %d  (v bits)", pps->pps_range_extension.log2_sao_offset_scale_chroma); AddTreeItemValueZero(iref);
        }
        my_printf_flag("pps_multilayer_extension_flag", pps->pps_multilayer_extension_flag); AddTreeItemValueZero(epf);
        my_printf_flag("pps_3d_extension_flag", pps->pps_3d_extension_flag); AddTreeItemValueZero(epf);
        my_printf("pps_extension_5bits: %d  (5 bits)", pps->pps_extension_5bits); AddTreeItemValueZero(epf);
    }

    if (pps->pps_multilayer_extension_flag)
    {
        // todo...
    }
    if (pps->pps_3d_extension_flag)
    {
        // todo...
    }

    my_printf("rbsp_trailing_bits()"); AddTreeItemValueZero(ipps);

}

void NALParser::h265DebugAud(h265_aud_t* aud, NALTreeNode& root)
{
    const char* pic_type = NULL;

    my_printf("access_unit_delimiter_rbsp()");
    auto iaud = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(iaud, root);


    switch (aud->pic_type)
    {
    case H265_AUD_PRIMARY_PIC_TYPE_I:    pic_type = "I"; break;
    case H265_AUD_PRIMARY_PIC_TYPE_IP:   pic_type = "P, I"; break;
    case H265_AUD_PRIMARY_PIC_TYPE_IPB:  pic_type = "B, P, I"; break;
    default: pic_type = "Unknown"; break;
    }
    my_printf("pic_type: %d ( %s )", aud->pic_type, pic_type); AddTreeItemValueZero(iaud);
    my_printf("rbsp_trailing_bits()"); AddTreeItemValueZero(root);
}

void NALParser::h265DebugSeis(h265_stream_t* h, NALTreeNode& root)
{
    h265_sei_t** seis = h->seis;
    int num_seis = h->num_seis;
    const char* sei_type_name = NULL;
    int i;

    my_printf("sei_rbsp()");
    auto isei = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(isei, root);

    my_printf("sei_message()");

    auto iisei = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(iisei, isei);

    for (i = 0; i < num_seis; i++)
    {
        h265_sei_t* s = seis[i];
        my_printf("payloadType: %d  (v bits)", s->payloadType); AddTreeItemValueZero(iisei);
        my_printf("payloadSize: %d  (v bits)", s->payloadSize); AddTreeItemValueZero(iisei);
        my_printf("sei_payload()");
        auto sp = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(sp, iisei);


        if (h->nal->nal_unit_type == NAL_UNIT_PREFIX_SEI)
        {
            switch (s->payloadType)
            {
            case 0:
                my_printf("buffering_period()"); AddTreeItemValueZero(sp);
                break;
            case 1:
                my_printf("pic_timing()"); AddTreeItemValueZero(sp);
                break;
            case 2:
                my_printf("pan_scan_rect()"); AddTreeItemValueZero(sp);
                break;
            case 3:
                my_printf("filler_payload()"); AddTreeItemValueZero(sp);
                break;
            case 4:
                my_printf("user_data_registered_itu_t_t35()"); AddTreeItemValueZero(sp);
                break;
            case 5:
            {
                my_printf("user_data_unregistered()"); AddTreeItemValueZero(sp);
                char uuid[64] = { 0 };
                char tmp[8] = { 0 };
                for (int j = 0; j < 16; j++)
                {
                    sprintf(tmp, "%X", s->payload[j]);
                    strcat(uuid, tmp);
                }
                my_printf("uuid_iso_iec_11578: %s", uuid);

                auto udpb = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
                this->m_root->putChild(udpb, sp);

             
                for (int j = 16; j < s->payloadSize; j++)
                {
                    my_printf("user_data_payload_byte: %d('%c')", s->payload[j], s->payload[j]);
                    AddTreeItemValueZero(sp);
                }
            }
            break;
            case 6:
                my_printf("recovery_point()"); AddTreeItemValueZero(sp);
                break;
            case 9:
                my_printf("scene_info()"); AddTreeItemValueZero(sp);
                break;
            case 15:
                my_printf("picture_snapshot()"); AddTreeItemValueZero(sp);
                break;
            case 16:
                my_printf("progressive_refinement_segment_start()"); AddTreeItemValueZero(sp);
                break;
            case 17:
                my_printf("progressive_refinement_segment_end()"); AddTreeItemValueZero(sp);
                break;
            default:
                my_printf("reserved_sei_message()"); AddTreeItemValueZero(sp);
                break;
            }
        }
        else if (h->nal->nal_unit_type == NAL_UNIT_SUFFIX_SEI)
        {
            switch (s->payloadType)
            {
            case 3:
                my_printf("filler_payload()"); AddTreeItemValueZero(sp);
                break;
            case 4:
                my_printf("user_data_registered_itu_t_t35()"); AddTreeItemValueZero(sp);
                break;
            case 5:
                my_printf("user_data_unregistered()"); AddTreeItemValueZero(sp);
                break;
            case 17:
                my_printf("progressive_refinement_segment_end()"); AddTreeItemValueZero(sp);
                break;
            case 22:
                my_printf("post_filter_hint()"); AddTreeItemValueZero(sp);
                break;
            case 132:
                my_printf("decoded_picture_hash()"); AddTreeItemValueZero(sp);
                break;
            case 16:
                my_printf("progressive_refinement_segment_start()"); AddTreeItemValueZero(sp);
                break;
            default:
                my_printf("reserved_sei_message()"); AddTreeItemValueZero(sp);
                break;
            }
        }
    }

    my_printf("rbsp_trailing_bits()"); AddTreeItemValueZero(isei);


}

void NALParser::h265DebugRefPicListsModification(h265_slice_header_t* hrd, NALTreeNode& root)
{
    my_printf("ref_pic_list_modification_flag_l0: %d", hrd->ref_pic_lists_modification.ref_pic_list_modification_flag_l0);
    auto modf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(modf, root);
    if (hrd->ref_pic_lists_modification.ref_pic_list_modification_flag_l0)
    {
        for (int i = 0; i <= hrd->num_ref_idx_l0_active_minus1; i++)
            my_printf("list_entry_l0[%d]: %u", i, hrd->ref_pic_lists_modification.list_entry_l0[i]); AddTreeItemValueZero(modf);
    }
    if (hrd->slice_type == H265_SH_SLICE_TYPE_B)
    {
        my_printf("ref_pic_list_modification_flag_l1: %d", hrd->ref_pic_lists_modification.ref_pic_list_modification_flag_l1); AddTreeItemValueZero(modf);
        for (int i = 0; i <= hrd->num_ref_idx_l1_active_minus1; i++)
            my_printf("list_entry_l1[%d]: %u", i, hrd->ref_pic_lists_modification.list_entry_l1[i]); AddTreeItemValueZero(modf);
    }
}

void NALParser::h265DebugPredWeightTable(h265_stream_t* h, NALTreeNode& root)
{
    pred_weight_table_t* pwt = &h->sh->pred_weight_table;
    h265_sps_t* sps = h->sps;

    my_printf("pred_weight_table()");

    auto ipwt = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ipwt, root);


    my_printf("luma_log2_weight_denom: %d  (v bits)", pwt->luma_log2_weight_denom); AddTreeItemValueZero(ipwt);
    if (h->sps->chroma_format_idc != 0)
    {
        my_printf("delta_chroma_log2_weight_denom: %d  (v bits)", pwt->delta_chroma_log2_weight_denom);
        AddTreeItemValueZero(ipwt);
    }

    auto nria = std::make_shared<TreeNode<ParameterDescription>>("NumRefIdxL0Active");

    if (h->sh->num_ref_idx_l0_active_minus1 >= 0)
    {
        my_printf("NumRefIdxL0Active");
        auto nria = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(nria, ipwt);
       
    }

    for (int i = 0; i <= h->sh->num_ref_idx_l0_active_minus1; i++)
    {
        my_printf_flag2("luma_weight_l0_flag", i, pwt->luma_weight_l0_flag[i]);
        AddTreeItemValueZero(nria);
    }
    if (h->sps->chroma_format_idc != 0)
    {
        for (int i = 0; i <= h->sh->num_ref_idx_l0_active_minus1; i++)
        {
            my_printf_flag2("chroma_weight_l0_flag", i, pwt->chroma_weight_l0_flag[i]);
            AddTreeItemValueZero(nria);
        }
    }


    for (int i = 0; i <= h->sh->num_ref_idx_l0_active_minus1; i++)
    {
        if (pwt->luma_weight_l0_flag[i])
        {
            my_printf("delta_luma_weight_l0[%d]: %d  (v bits)", i, pwt->delta_luma_weight_l0[i]); AddTreeItemValueZero(nria);
            my_printf("luma_offset_l0[%d]: %d  (v bits)", i, pwt->luma_offset_l0[i]); AddTreeItemValueZero(nria);
        }
        if (pwt->chroma_weight_l0_flag[i])
        {
            for (int j = 0; j < 2; j++)
            {
                my_printf("delta_chroma_weight_l0[%d][%d]: %d  (v bits)", i, j, pwt->delta_chroma_weight_l0[i][j]); AddTreeItemValueZero(nria);
                my_printf("delta_chroma_offset_l0[%d][%d]: %d  (v bits)", i, j, pwt->delta_chroma_offset_l0[i][j]); AddTreeItemValueZero(nria);
            }
        }
    }

    auto nria_l = std::make_shared<TreeNode<ParameterDescription>>("NumRefIdxL0Active");
    
    if ((h->sh->slice_type == H265_SH_SLICE_TYPE_B) && (h->sh->num_ref_idx_l1_active_minus1 >= 0))
    {
        my_printf("NumRefIdxL1Active");
        auto nria_l = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(nria_l, ipwt);

    }

    if (h->sh->slice_type == H265_SH_SLICE_TYPE_B)
    {
        for (int i = 0; i <= h->sh->num_ref_idx_l1_active_minus1; i++)
        {
            my_printf_flag2("luma_weight_l1_flag", i, pwt->luma_weight_l1_flag[i]);
            AddTreeItemValueZero(nria_l);
        }
        if (h->sps->chroma_format_idc != 0)
        {
            for (int i = 0; i <= h->sh->num_ref_idx_l1_active_minus1; i++)
            {
                my_printf_flag2("chroma_weight_l1_flag", i, pwt->chroma_weight_l1_flag[i]);
                AddTreeItemValueZero(nria_l);
            }
        }
        for (int i = 0; i <= h->sh->num_ref_idx_l1_active_minus1; i++)
        {
            if (pwt->luma_weight_l1_flag[i])
            {
                my_printf("delta_luma_weight_l1[%d]: %d  (v bits)", i, pwt->delta_luma_weight_l1[i]); AddTreeItemValueZero(nria_l);
                my_printf("luma_offset_l1[%d]: %d  (v bits)", i, pwt->luma_offset_l1[i]); AddTreeItemValueZero(nria_l);

            }
            if (pwt->chroma_weight_l1_flag[i])
            {
                for (int j = 0; j < 2; j++)
                {
                    my_printf("delta_chroma_weight_l1[%d][%d]: %d  (v bits)", i, j, pwt->delta_chroma_weight_l1[i][j]);
                    AddTreeItemValueZero(nria_l);
                    my_printf("delta_chroma_offset_l1[%d][%d]: %d  (v bits)", i, j, pwt->delta_chroma_offset_l1[i][j]);
                    AddTreeItemValueZero(nria_l);
                }
            }
        }
    }
}

void NALParser::h265DebugSliceHeader(h265_stream_t* h, NALTreeNode& root)
{
    h265_slice_header_t* hrd = h->sh;
    h265_sps_t* sps = NULL;
    h265_pps_t* pps = NULL;
    int nal_unit_type = h->nal->nal_unit_type;
    h->pps = h->pps_table[hrd->slice_pic_parameter_set_id];
    pps = h->pps;
    h->sps = h->sps_table[pps->pps_seq_parameter_set_id];
    sps = h->sps;
    if (pps == NULL || sps == NULL) return;

    my_printf("slice_segment_layer_rbsp()");

    auto islce = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(islce, root);


    my_printf("slice_segment_header()");
    auto ihrd = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
    this->m_root->putChild(ihrd, islce);


    my_printf_flag("first_slice_segment_in_pic_flag", hrd->first_slice_segment_in_pic_flag); AddTreeItemValueZero(ihrd);
    if (nal_unit_type >= NAL_UNIT_CODED_SLICE_BLA_W_LP && nal_unit_type <= NAL_UNIT_RESERVED_IRAP_VCL23)
    {
        my_printf_flag("no_output_of_prior_pics_flag", hrd->no_output_of_prior_pics_flag); AddTreeItemValueZero(ihrd);
    }
    my_printf("slice_pic_parameter_set_id: %d (v bits)", hrd->slice_pic_parameter_set_id); AddTreeItemValueZero(ihrd);
    if (!hrd->first_slice_segment_in_pic_flag)
    {
        if (pps->dependent_slice_segments_enabled_flag)
        {
            my_printf_flag("dependent_slice_segment_flag", hrd->dependent_slice_segment_flag); AddTreeItemValueZero(ihrd);
        }
        my_printf("slice_segment_address: %d (v %d bits)", hrd->slice_segment_address, hrd->slice_segment_address_bytes);
        AddTreeItemValueZero(ihrd);
    }

    if (!hrd->dependent_slice_segment_flag)
    {
        my_printf("dependent_slice_segment_flag");

        auto dssf = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(dssf, ihrd);

        for (int i = 0; i < pps->num_extra_slice_header_bits; i++)
        {
            my_printf("slice_reserved_flag[%d]: %d", i, hrd->slice_reserved_flag[i]); // todo
            AddTreeItemValueZero(dssf);
        }
        const char* slice_type_name;
        switch (hrd->slice_type)
        {
        case H265_SH_SLICE_TYPE_P:  slice_type_name = "P slice"; break;
        case H265_SH_SLICE_TYPE_B:  slice_type_name = "B slice"; break;
        case H265_SH_SLICE_TYPE_I:  slice_type_name = "I slice"; break;
        default:                    slice_type_name = "Unknown"; break;
        }
        my_printf("slice_type: %d (%s) (v bits)", hrd->slice_type, slice_type_name); AddTreeItemValueZero(dssf);
        if (pps->output_flag_present_flag)
        {
            my_printf_flag("pic_output_flag", hrd->pic_output_flag); AddTreeItemValueZero(dssf);
        }
        if (sps->separate_colour_plane_flag == 1)
        {
            my_printf("colour_plane_id: %d  (2 bits)", hrd->colour_plane_id); AddTreeItemValueZero(dssf);
        }
        if (nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL || nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP)
        {
            // do nothing...
        }
        else
        {
            my_printf("slice_pic_order_cnt_lsb: %d  (%d bits)", hrd->slice_pic_order_cnt_lsb, hrd->slice_pic_order_cnt_lsb_bytes); AddTreeItemValueZero(dssf);
            my_printf_flag("short_term_ref_pic_set_sps_flag", hrd->short_term_ref_pic_set_sps_flag); AddTreeItemValueZero(dssf);
            if (!hrd->short_term_ref_pic_set_sps_flag)
            {
                referencePictureSets_t* rps = &hrd->m_localRPS;
                h265DebugShortTermRefPicSet(sps, &hrd->st_ref_pic_set, rps, sps->num_short_term_ref_pic_sets, dssf);
            }
            else if (sps->num_short_term_ref_pic_sets > 1)
            {
                my_printf("short_term_ref_pic_set_idx: %d  (v %d bits)", hrd->short_term_ref_pic_set_idx, hrd->short_term_ref_pic_set_idx_bytes);
                AddTreeItemValueZero(dssf);
            }
            if (sps->long_term_ref_pics_present_flag)
            {
                if (sps->num_long_term_ref_pics_sps > 0)
                {
                    my_printf("num_long_term_sps: %d  (v bits)", hrd->num_long_term_sps); AddTreeItemValueZero(dssf);
                }
                my_printf("num_long_term_pics: %d  (v bits)", hrd->num_long_term_pics); AddTreeItemValueZero(dssf);
                for (int i = 0; i < (int)hrd->lt_idx_sps.size(); i++)
                {
                    if (i < hrd->num_long_term_sps)
                    {
                        if (sps->num_long_term_ref_pics_sps > 1)
                        {
                            my_printf("lt_idx_sps[%d]: %d  (v bits)", i, hrd->lt_idx_sps[i]); AddTreeItemValueZero(dssf);
                        }
                    }
                    else
                    {
                        my_printf("poc_lsb_lt[%d]: %d  (v bits)", i, hrd->poc_lsb_lt[i]); AddTreeItemValueZero(dssf);
                        my_printf_flag2("used_by_curr_pic_lt_flag", i, hrd->used_by_curr_pic_lt_flag[i]); AddTreeItemValueZero(dssf);
                    }
                    my_printf_flag2("delta_poc_msb_present_flag", i, hrd->delta_poc_msb_present_flag[i]); AddTreeItemValueZero(dssf);
                    if (hrd->delta_poc_msb_present_flag[i])
                    {
                        my_printf("delta_poc_msb_cycle_lt[%d]: %d  (v bits)", i, hrd->delta_poc_msb_cycle_lt[i]);
                        AddTreeItemValueZero(dssf);
                    }
                }
            }
            if (sps->sps_temporal_mvp_enabled_flag)
            {
                my_printf_flag("slice_temporal_mvp_enabled_flag", hrd->slice_temporal_mvp_enabled_flag); AddTreeItemValueZero(dssf);
            }
        }

        if (sps->sample_adaptive_offset_enabled_flag)
        {
            my_printf_flag("slice_sao_luma_flag", hrd->slice_sao_luma_flag); AddTreeItemValueZero(dssf);
            my_printf_flag("slice_sao_chroma_flag", hrd->slice_sao_chroma_flag); AddTreeItemValueZero(dssf);
        }
        if (hrd->slice_type == H265_SH_SLICE_TYPE_P || hrd->slice_type == H265_SH_SLICE_TYPE_B)
        {
            my_printf_flag("num_ref_idx_active_override_flag", hrd->num_ref_idx_active_override_flag);
            auto nriaof = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
            this->m_root->putChild(nriaof, dssf);

            if (hrd->num_ref_idx_active_override_flag)
            {
                my_printf("num_ref_idx_l0_active_minus1: %d  (v bits)", hrd->num_ref_idx_l0_active_minus1); AddTreeItemValueZero(dssf);
                if (hrd->slice_type == H265_SH_SLICE_TYPE_B)
                {
                    my_printf("num_ref_idx_l1_active_minus1: %d  (v bits)", hrd->num_ref_idx_l1_active_minus1); AddTreeItemValueZero(dssf);
                }
            }
            if (pps->lists_modification_present_flag)
            {
                h265DebugRefPicListsModification(hrd,dssf);
            }
            if (hrd->slice_type == H265_SH_SLICE_TYPE_B)
            {
                my_printf_flag("mvd_l1_zero_flag", hrd->mvd_l1_zero_flag); AddTreeItemValueZero(dssf);
            }
            if (pps->cabac_init_present_flag)
            {
                my_printf_flag("cabac_init_flag", hrd->cabac_init_flag); AddTreeItemValueZero(dssf);
            }
            if (hrd->slice_temporal_mvp_enabled_flag)
            {
                if (hrd->slice_type == H265_SH_SLICE_TYPE_B)
                {
                    my_printf_flag("collocated_from_l0_flag", hrd->collocated_from_l0_flag); AddTreeItemValueZero(dssf);
                }
                if ((hrd->collocated_from_l0_flag && hrd->num_ref_idx_l0_active_minus1 > 0) ||
                    (!hrd->collocated_from_l0_flag && hrd->num_ref_idx_l1_active_minus1 > 0))
                {
                    my_printf("collocated_ref_idx: %d  (v bits)", hrd->collocated_ref_idx); AddTreeItemValueZero(dssf);
                }
            }
            if ((pps->weighted_pred_flag && hrd->slice_type == H265_SH_SLICE_TYPE_P) ||
                (pps->weighted_bipred_flag && hrd->slice_type == H265_SH_SLICE_TYPE_B))
                h265DebugPredWeightTable(h, dssf);
            my_printf("five_minus_max_num_merge_cand: %d  (v bits)", hrd->five_minus_max_num_merge_cand); AddTreeItemValueZero(dssf);
        }
        my_printf("slice_qp_delta: %d  (v bits)", hrd->slice_qp_delta); AddTreeItemValueZero(dssf);
        if (pps->pps_slice_chroma_qp_offsets_present_flag)
        {
            my_printf("slice_cb_qp_offset: %d  (v bits)", hrd->slice_cb_qp_offset); AddTreeItemValueZero(dssf);
            my_printf("slice_cr_qp_offset: %d  (v bits)", hrd->slice_cr_qp_offset); AddTreeItemValueZero(dssf);
        }
        if (pps->pps_range_extension.chroma_qp_offset_list_enabled_flag)
        {
            my_printf_flag("cu_chroma_qp_offset_enabled_flag", hrd->cu_chroma_qp_offset_enabled_flag); AddTreeItemValueZero(dssf);
        }
        if (pps->deblocking_filter_override_enabled_flag)
        {
            my_printf_flag("deblocking_filter_override_flag", hrd->deblocking_filter_override_flag); AddTreeItemValueZero(dssf);
        }
        if (hrd->deblocking_filter_override_flag)
        {
            my_printf_flag("slice_deblocking_filter_disabled_flag", hrd->slice_deblocking_filter_disabled_flag);
            auto dfof = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
            this->m_root->putChild(dfof, dssf);

 
            if (!hrd->slice_deblocking_filter_disabled_flag)
            {
                my_printf("slice_beta_offset_div2: %d  (v bits)", hrd->slice_beta_offset_div2); AddTreeItemValueZero(dfof);
                my_printf("slice_tc_offset_div2: %d  (v bits)", hrd->slice_tc_offset_div2); AddTreeItemValueZero(dfof);
            }
        }
        if (pps->pps_loop_filter_across_slices_enabled_flag &&
            (hrd->slice_sao_luma_flag || hrd->slice_sao_chroma_flag ||
                !hrd->slice_deblocking_filter_disabled_flag))
        {
            my_printf_flag("slice_loop_filter_across_slices_enabled_flag", hrd->slice_loop_filter_across_slices_enabled_flag);
            AddTreeItemValueZero(dssf);
        }
    }
    if (pps->tiles_enabled_flag || pps->entropy_coding_sync_enabled_flag)
    {
        my_printf("num_entry_point_offsets: %d (v bits)", hrd->num_entry_point_offsets);

        auto inepo = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
        this->m_root->putChild(inepo, ihrd);

        if (hrd->num_entry_point_offsets > 0)
        {
            my_printf("offset_len_minus1: %d (v bits)", hrd->offset_len_minus1); AddTreeItemValueZero(inepo);
            my_printf("NumEntryPointOffsets");
            auto iinepo = std::make_shared<TreeNode<ParameterDescription>>(m_outputInfo);
            this->m_root->putChild(iinepo, ihrd);
            for (int i = 0; i < hrd->num_entry_point_offsets; i++)
            {
                my_printf("entry_point_offset_minus1[%d]: %d (%d bits)", i, hrd->entry_point_offset_minus1[i], hrd->entry_point_offset_minus1_bytes); // to add len
                AddTreeItemValueZero(iinepo);
            }
        }
    }
    if (pps->slice_segment_header_extension_present_flag)
    {
        my_printf("slice_segment_header_extension_length: %d (v bits)", hrd->slice_segment_header_extension_length);
        AddTreeItemValueZero(ihrd);
        for (int i = 0; i < hrd->slice_segment_header_extension_length; i++)
        {
            my_printf("slice_segment_header_extension_data_byte[%d]: %d  (8 bits)", i, hrd->slice_segment_header_extension_data_byte[i]);
            AddTreeItemValueZero(ihrd);
        }
    }
    // no need to debug...
    my_printf("slice_segment_data()");
    AddTreeItemValueZero(islce);
    my_printf("rbsp_slice_segment_trailing_bits()");
    AddTreeItemValueZero(islce);

}

