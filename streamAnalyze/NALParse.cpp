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

// Ê÷ÐÎÏî
#define AddTreeItem(para,_item) \
   _item->m_element.m_pareterDesciption.push_back({m_outputInfo,para });


NALParser::NALParser(STREAM_TYPE streamType) {
    //just h264
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
}

void NALParser::h264debugAUD(aud_t* aud, NALTreeNode& root)
{
}

void NALParser::h264debugSEI(h264_stream_t* h, NALTreeNode& root)
{
}

