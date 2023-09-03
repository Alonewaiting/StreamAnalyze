#include<iostream>
#include<fstream>
#include "FileStream.h"
#include "h265_stream.h"
#include "h265_sei.h"


using namespace std;

//h265_stream_t* h265New() {
//    h265_stream_t* h = (h265_stream_t*)calloc(1, sizeof(h265_stream_t));
//
//    h->nal = (h265_nal_t*)calloc(1, sizeof(h265_nal_t));
//
//    // initialize tables
//    for (int i = 0; i < 16; i++) { h->vps_table[i] = (h265_vps_t*)calloc(1, sizeof(h265_vps_t)); }
//    for (int i = 0; i < 32; i++) { h->sps_table[i] = (h265_sps_t*)calloc(1, sizeof(h265_sps_t)); }
//    for (int i = 0; i < 256; i++) { h->pps_table[i] = (h265_pps_t*)calloc(1, sizeof(h265_pps_t)); }
//
//    h->vps = h->vps_table[0];
//    h->sps = h->sps_table[0];
//    h->pps = h->pps_table[0];
//    h->aud = (h265_aud_t*)calloc(1, sizeof(h265_aud_t));
//    h->num_seis = 0;
//    h->seis = NULL;
//    h->sei = NULL;  //This is a TEMP pointer at whats in h->seis...
//    h->sh = (h265_slice_header_t*)calloc(1, sizeof(h265_slice_header_t));
//
//    h->info = (videoinfo_t*)calloc(1, sizeof(videoinfo_t));
//    h->info->type = 1;
//    return h;
//}
//void h265Free(h265_stream_t* h) {
//    free(h->nal);
//    for (int i = 0; i < 16; i++) { if (h->vps_table[i] != NULL) free(h->vps_table[i]); }
//    for (int i = 0; i < 32; i++) { if (h->sps_table[i] != NULL) free(h->sps_table[i]); }
//    for (int i = 0; i < 256; i++) { if (h->pps_table[i] != NULL) free(h->pps_table[i]); }
//
//    if (h->aud != NULL)
//    {
//        free(h->aud);
//    }
//
//    if (h->seis != NULL)
//    {
//        for (int i = 0; i < h->num_seis; i++)
//        {
//            h265_sei_t* sei = h->seis[i];
//            h265_sei_free(sei);
//        }
//        free(h->seis);
//    }
//    free(h->sh);
//    free(h->info);
//    free(h);
//}
//
//// E.2.1  VUI parameters syntax
//void h265Read_vui_parameters(vui_parameters_t* vui, bs_t* b, int maxNumSubLayersMinus1)
//{
//    vui->aspect_ratio_info_present_flag = bs_read_u1(b);
//    if (vui->aspect_ratio_info_present_flag)
//    {
//        vui->aspect_ratio_idc = bs_read_u8(b);
//        if (vui->aspect_ratio_idc == H265_SAR_Extended)
//        {
//            vui->sar_width = bs_read_u(b, 16);
//            vui->sar_height = bs_read_u(b, 16);
//        }
//    }
//    vui->overscan_info_present_flag = bs_read_u1(b);
//    if (vui->overscan_info_present_flag)
//    {
//        vui->overscan_appropriate_flag = bs_read_u1(b);
//    }
//    vui->video_signal_type_present_flag = bs_read_u1(b);
//    if (vui->video_signal_type_present_flag)
//    {
//        vui->video_format = bs_read_u(b, 3);
//        vui->video_full_range_flag = bs_read_u1(b);
//        vui->colour_description_present_flag = bs_read_u1(b);
//        if (vui->colour_description_present_flag)
//        {
//            vui->colour_primaries = bs_read_u8(b);
//            vui->transfer_characteristics = bs_read_u8(b);
//            vui->matrix_coeffs = bs_read_u8(b);
//        }
//    }
//    vui->chroma_loc_info_present_flag = bs_read_u1(b);
//    if (vui->chroma_loc_info_present_flag)
//    {
//        vui->chroma_sample_loc_type_top_field = bs_read_ue(b);
//        vui->chroma_sample_loc_type_bottom_field = bs_read_ue(b);
//    }
//    vui->neutral_chroma_indication_flag = bs_read_u1(b);
//    vui->field_seq_flag = bs_read_u1(b);
//    vui->frame_field_info_present_flag = bs_read_u1(b);
//    vui->default_display_window_flag = bs_read_u1(b);
//    if (vui->default_display_window_flag)
//    {
//        vui->def_disp_win_left_offset = bs_read_ue(b);
//        vui->def_disp_win_right_offset = bs_read_ue(b);
//        vui->def_disp_win_top_offset = bs_read_ue(b);
//        vui->def_disp_win_bottom_offset = bs_read_ue(b);
//    }
//    vui->vui_timing_info_present_flag = bs_read_u1(b);
//    if (vui->vui_timing_info_present_flag)
//    {
//        vui->vui_num_units_in_tick = bs_read_u(b, 32);
//        vui->vui_time_scale = bs_read_u(b, 32);
//        vui->vui_poc_proportional_to_timing_flag = bs_read_u1(b);
//        if (vui->vui_poc_proportional_to_timing_flag)
//        {
//            vui->vui_num_ticks_poc_diff_one_minus1 = bs_read_ue(b);
//        }
//        vui->vui_hrd_parameters_present_flag = bs_read_u1(b);
//        if (vui->vui_hrd_parameters_present_flag)
//        {
//            h265_read_hrd_parameters(&vui->hrd_parameters, b, 1, maxNumSubLayersMinus1);
//        }
//    }
//    vui->bitstream_restriction_flag = bs_read_u1(b);
//    if (vui->bitstream_restriction_flag)
//    {
//        vui->tiles_fixed_structure_flag = bs_read_u1(b);
//        vui->motion_vectors_over_pic_boundaries_flag = bs_read_u1(b);
//        vui->restricted_ref_pic_lists_flag = bs_read_u1(b);
//        vui->min_spatial_segmentation_idc = bs_read_ue(b);
//        vui->max_bytes_per_pic_denom = bs_read_ue(b);
//        vui->max_bits_per_min_cu_denom = bs_read_ue(b);
//        vui->log2_max_mv_length_horizontal = bs_read_ue(b);
//        vui->log2_max_mv_length_vertical = bs_read_ue(b);
//    }
//}
//
//void h265Read_ptl(profile_tier_level_t* ptl, bs_t* b, int profilePresentFlag, int max_sub_layers_minus1)
//{
//    int i = 0;
//    if (profilePresentFlag)
//    {
//        ptl->general_profile_space = bs_read_u(b, 2);
//        ptl->general_tier_flag = bs_read_u1(b);
//        ptl->general_profile_idc = bs_read_u(b, 5);
//        for (i = 0; i < 32; i++)
//        {
//            ptl->general_profile_compatibility_flag[i] = bs_read_u1(b);
//        }
//        ptl->general_progressive_source_flag = bs_read_u1(b);
//        ptl->general_interlaced_source_flag = bs_read_u1(b);
//        ptl->general_non_packed_constraint_flag = bs_read_u1(b);
//        ptl->general_frame_only_constraint_flag = bs_read_u1(b);
//        if (ptl->general_profile_idc == 4 || ptl->general_profile_compatibility_flag[4] ||
//            ptl->general_profile_idc == 5 || ptl->general_profile_compatibility_flag[5] ||
//            ptl->general_profile_idc == 6 || ptl->general_profile_compatibility_flag[6] ||
//            ptl->general_profile_idc == 7 || ptl->general_profile_compatibility_flag[7])
//        {
//            ptl->general_max_12bit_constraint_flag = bs_read_u1(b);
//            ptl->general_max_10bit_constraint_flag = bs_read_u1(b);
//            ptl->general_max_8bit_constraint_flag = bs_read_u1(b);
//            ptl->general_max_422chroma_constraint_flag = bs_read_u1(b);
//            ptl->general_max_420chroma_constraint_flag = bs_read_u1(b);
//            ptl->general_max_monochrome_constraint_flag = bs_read_u1(b);
//            ptl->general_intra_constraint_flag = bs_read_u1(b);
//            ptl->general_one_picture_only_constraint_flag = bs_read_u1(b);
//            ptl->general_lower_bit_rate_constraint_flag = bs_read_u1(b);
//            uint64_t tmp1 = bs_read_u(b, 32);
//            uint64_t tmp2 = bs_read_u(b, 2);
//            ptl->general_reserved_zero_34bits = tmp1 + tmp2;
//        }
//        else
//        {
//            uint64_t tmp1 = bs_read_u(b, 32);
//            uint64_t tmp2 = bs_read_u(b, 11);
//            ptl->general_reserved_zero_43bits = tmp1 + tmp2;
//        }
//        if ((ptl->general_profile_idc >= 1 && ptl->general_profile_idc <= 5) ||
//            ptl->general_profile_compatibility_flag[1] || ptl->general_profile_compatibility_flag[2] ||
//            ptl->general_profile_compatibility_flag[3] || ptl->general_profile_compatibility_flag[4] ||
//            ptl->general_profile_compatibility_flag[5])
//        {
//            ptl->general_inbld_flag = bs_read_u1(b);
//        }
//        else
//        {
//            ptl->general_reserved_zero_bit = bs_read_u1(b);
//        }
//    }
//    ptl->general_level_idc = bs_read_u8(b);
//    ptl->sub_layer_profile_present_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_level_present_flag.resize(max_sub_layers_minus1);
//    for (i = 0; i < max_sub_layers_minus1; i++)
//    {
//        ptl->sub_layer_profile_present_flag[i] = bs_read_u1(b);
//        ptl->sub_layer_level_present_flag[i] = bs_read_u1(b);
//    }
//    if (max_sub_layers_minus1 > 0)
//    {
//        for (i = max_sub_layers_minus1; i < 8; i++)
//        {
//            ptl->reserved_zero_2bits[i] = bs_read_u(b, 2);
//        }
//    }
//    ptl->sub_layer_profile_space.resize(max_sub_layers_minus1);
//    ptl->sub_layer_tier_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_profile_idc.resize(max_sub_layers_minus1);
//    ptl->sub_layer_profile_compatibility_flag.resize(max_sub_layers_minus1);
//    for (int j = 0; j < max_sub_layers_minus1; j++)
//    {
//        ptl->sub_layer_profile_compatibility_flag[j].resize(32);
//    }
//    ptl->sub_layer_progressive_source_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_interlaced_source_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_non_packed_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_frame_only_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_max_12bit_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_max_10bit_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_max_8bit_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_max_422chroma_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_max_420chroma_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_max_monochrome_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_intra_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_one_picture_only_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_lower_bit_rate_constraint_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_reserved_zero_34bits.resize(max_sub_layers_minus1);
//    ptl->sub_layer_reserved_zero_43bits.resize(max_sub_layers_minus1);
//    ptl->sub_layer_inbld_flag.resize(max_sub_layers_minus1);
//    ptl->sub_layer_reserved_zero_bit.resize(max_sub_layers_minus1);
//    ptl->sub_layer_level_idc.resize(max_sub_layers_minus1);
//    for (i = 0; i < max_sub_layers_minus1; i++)
//    {
//        if (ptl->sub_layer_profile_present_flag[i])
//        {
//            ptl->sub_layer_profile_space[i] = bs_read_u(b, 2);
//            ptl->sub_layer_tier_flag[i] = bs_read_u1(b);
//            ptl->sub_layer_profile_idc[i] = bs_read_u(b, 5);
//            for (int j = 0; j < 32; j++)
//            {
//                ptl->sub_layer_profile_compatibility_flag[i][j] = bs_read_u1(b);
//            }
//            ptl->sub_layer_progressive_source_flag[i] = bs_read_u1(b);
//            ptl->sub_layer_interlaced_source_flag[i] = bs_read_u1(b);
//            ptl->sub_layer_non_packed_constraint_flag[i] = bs_read_u1(b);
//            ptl->sub_layer_frame_only_constraint_flag[i] = bs_read_u1(b);
//            if (ptl->sub_layer_profile_idc[i] == 4 || ptl->sub_layer_profile_compatibility_flag[i][4] ||
//                ptl->sub_layer_profile_idc[i] == 5 || ptl->sub_layer_profile_compatibility_flag[i][5] ||
//                ptl->sub_layer_profile_idc[i] == 6 || ptl->sub_layer_profile_compatibility_flag[i][6] ||
//                ptl->sub_layer_profile_idc[i] == 7 || ptl->sub_layer_profile_compatibility_flag[i][7])
//            {
//                ptl->sub_layer_max_12bit_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_max_10bit_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_max_8bit_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_max_422chroma_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_max_420chroma_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_max_monochrome_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_intra_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_one_picture_only_constraint_flag[i] = bs_read_u1(b);
//                ptl->sub_layer_lower_bit_rate_constraint_flag[i] = bs_read_u1(b);
//                uint64_t tmp1 = bs_read_u(b, 32);
//                uint64_t tmp2 = bs_read_u(b, 2);
//                ptl->sub_layer_reserved_zero_34bits[i] = tmp1 + tmp2;
//            }
//            else
//            {
//                uint64_t tmp1 = bs_read_u(b, 32);
//                uint64_t tmp2 = bs_read_u(b, 12);
//                ptl->sub_layer_reserved_zero_43bits[i] = tmp1 + tmp2;
//            }
//            // to check
//            if ((ptl->sub_layer_profile_idc[i] >= 1 && ptl->sub_layer_profile_idc[i] <= 5) ||
//                ptl->sub_layer_profile_compatibility_flag[i][1] ||
//                ptl->sub_layer_profile_compatibility_flag[i][2] ||
//                ptl->sub_layer_profile_compatibility_flag[i][3] ||
//                ptl->sub_layer_profile_compatibility_flag[i][4] ||
//                ptl->sub_layer_profile_compatibility_flag[i][5])
//            {
//                ptl->sub_layer_inbld_flag[i] = bs_read_u1(b);
//            }
//            else
//            {
//                ptl->sub_layer_reserved_zero_bit[i] = bs_read_u1(b);
//            }
//        }
//        if (ptl->sub_layer_level_present_flag[i])
//        {
//            ptl->sub_layer_level_idc[i] = bs_read_u8(b);
//        }
//    }
//}
//// 7.3.4  Scaling list data syntax
//void h265Read_scaling_list(scaling_list_data_t* sld, bs_t* b)
//{
//    for (int sizeId = 0; sizeId < 4; sizeId++)
//    {
//        for (int matrixId = 0; matrixId < 6; matrixId += (sizeId == 3) ? 3 : 1)
//        {
//            sld->scaling_list_pred_mode_flag[sizeId][matrixId] = bs_read_u1(b);
//            if (!sld->scaling_list_pred_mode_flag[sizeId][matrixId])
//            {
//                sld->scaling_list_pred_matrix_id_delta[sizeId][matrixId] = bs_read_ue(b);
//            }
//            else
//            {
//                int nextCoef = 8;
//                int coefNum = min(64, (1 << (4 + (sizeId << 1))));
//                sld->coefNum = coefNum; // tmp store
//                if (sizeId > 1)
//                {
//                    sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId] = bs_read_se(b);
//                    nextCoef = sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId] + 8;
//                }
//                for (int i = 0; i < sld->coefNum; i++)
//                {
//                    int scaling_list_delta_coef = bs_read_se(b);
//                    nextCoef = (nextCoef + scaling_list_delta_coef + 256) % 256;
//                    sld->ScalingList[sizeId][matrixId][i] = nextCoef;
//                }
//            }
//        }
//    }
//}
//
//// st_ref_pic_set
//// 7.3.7  Short-term reference picture set syntax
//void h265Read_short_term_ref_pic_set(bs_t* b, h265_sps_t* sps, st_ref_pic_set_t* st, referencePictureSets_t* rps, int stRpsIdx)
//{
//    st->inter_ref_pic_set_prediction_flag = 0;
//    if (stRpsIdx != 0)
//    {
//        st->inter_ref_pic_set_prediction_flag = bs_read_u1(b);
//    }
//    if (st->inter_ref_pic_set_prediction_flag)
//    {
//        st->delta_idx_minus1 = 0;
//        if (stRpsIdx == sps->m_RPSList.size())
//        {
//            st->delta_idx_minus1 = bs_read_ue(b);
//        }
//        int rIdx = stRpsIdx - 1 - st->delta_idx_minus1;
//        referencePictureSets_t* rpsRef = &sps->m_RPSList[rIdx];
//
//        st->delta_rps_sign = bs_read_u1(b);
//        st->abs_delta_rps_minus1 = bs_read_ue(b);
//        int deltaRPS = (1 - 2 * st->delta_rps_sign) * (st->abs_delta_rps_minus1 + 1); // delta_RPS
//        st->used_by_curr_pic_flag.resize(rpsRef->m_numberOfPictures + 1);
//        st->use_delta_flag.resize(rpsRef->m_numberOfPictures + 1);
//        for (int j = 0; j <= rpsRef->m_numberOfPictures; j++)
//        {
//            st->used_by_curr_pic_flag[j] = bs_read_u1(b);
//            int refIdc = st->used_by_curr_pic_flag[j];
//            if (!st->used_by_curr_pic_flag[j])
//            {
//                st->use_delta_flag[j] = bs_read_u1(b);
//                refIdc = st->use_delta_flag[j] << 1; //second bit is "1" if refIdc is 2, "0" if refIdc = 0.
//            }
//            // todo furture
//            if (refIdc == 1 || refIdc == 2)
//            {
//
//            }
//        }
//    }
//    else
//    {
//        st->num_negative_pics = bs_read_ue(b);
//        st->num_positive_pics = bs_read_ue(b);
//
//        rps->m_numberOfNegativePictures = st->num_negative_pics;
//        rps->m_numberOfPositivePictures = st->num_positive_pics;
//
//        // to check...
//        st->delta_poc_s0_minus1.resize(st->num_negative_pics);
//        st->used_by_curr_pic_s0_flag.resize(st->num_negative_pics);
//        for (int i = 0; i < st->num_negative_pics; i++)
//        {
//            st->delta_poc_s0_minus1[i] = bs_read_ue(b);
//            st->used_by_curr_pic_s0_flag[i] = bs_read_u1(b);
//            rps->m_used[i] = st->used_by_curr_pic_s0_flag[i];
//        }
//        st->delta_poc_s1_minus1.resize(st->num_positive_pics);
//        st->used_by_curr_pic_s1_flag.resize(st->num_positive_pics);
//        for (int i = 0; i < st->num_positive_pics; i++)
//        {
//            st->delta_poc_s1_minus1[i] = bs_read_ue(b);
//            st->used_by_curr_pic_s1_flag[i] = bs_read_u1(b);
//            rps->m_used[i + st->num_negative_pics] = st->used_by_curr_pic_s1_flag[i];
//        }
//        rps->m_numberOfPictures = rps->m_numberOfNegativePictures + rps->m_numberOfPositivePictures;
//    }
//}
//// E.2.2  HRD parameters syntax
//void h265Read_hrd_parameters(hrd_parameters_t* hrd, bs_t* b, int commonInfPresentFlag, int maxNumSubLayersMinus1)
//{
//    if (commonInfPresentFlag)
//    {
//        hrd->nal_hrd_parameters_present_flag = bs_read_u1(b);
//        hrd->vcl_hrd_parameters_present_flag = bs_read_u1(b);
//        if (hrd->nal_hrd_parameters_present_flag ||
//            hrd->vcl_hrd_parameters_present_flag)
//        {
//            hrd->sub_pic_hrd_params_present_flag = bs_read_u1(b);
//            if (hrd->sub_pic_hrd_params_present_flag)
//            {
//                hrd->tick_divisor_minus2 = bs_read_u8(b);
//                hrd->du_cpb_removal_delay_increment_length_minus1 = bs_read_u(b, 5);
//                hrd->sub_pic_cpb_params_in_pic_timing_sei_flag = bs_read_u1(b);
//                hrd->dpb_output_delay_du_length_minus1 = bs_read_u(b, 5);
//            }
//            hrd->bit_rate_scale = bs_read_u(b, 4);
//            hrd->cpb_size_scale = bs_read_u(b, 4);
//            if (hrd->sub_pic_hrd_params_present_flag)
//            {
//                hrd->cpb_size_du_scale = bs_read_u(b, 4);
//            }
//            hrd->initial_cpb_removal_delay_length_minus1 = bs_read_u(b, 5);
//            hrd->au_cpb_removal_delay_length_minus1 = bs_read_u(b, 5);
//            hrd->dpb_output_delay_length_minus1 = bs_read_u(b, 5);
//        }
//    }
//    hrd->fixed_pic_rate_general_flag.resize(maxNumSubLayersMinus1 + 1);
//    hrd->fixed_pic_rate_within_cvs_flag.resize(maxNumSubLayersMinus1 + 1);
//    hrd->elemental_duration_in_tc_minus1.resize(maxNumSubLayersMinus1 + 1);
//    hrd->low_delay_hrd_flag.resize(maxNumSubLayersMinus1 + 1);
//    hrd->cpb_cnt_minus1.resize(maxNumSubLayersMinus1 + 1);
//    for (int i = 0; i <= maxNumSubLayersMinus1; i++)
//    {
//        hrd->fixed_pic_rate_general_flag[i] = bs_read_u1(b);
//        if (!hrd->fixed_pic_rate_general_flag[i])
//        {
//            hrd->fixed_pic_rate_within_cvs_flag[i] = bs_read_u1(b);
//        }
//        if (hrd->fixed_pic_rate_within_cvs_flag[i])
//        {
//            hrd->elemental_duration_in_tc_minus1[i] = bs_read_ue(b);
//        }
//        else
//        {
//            hrd->low_delay_hrd_flag[i] = bs_read_u1(b);
//        }
//        if (!hrd->low_delay_hrd_flag[i])
//        {
//            hrd->cpb_cnt_minus1[i] = bs_read_u1(b);
//        }
//        if (hrd->nal_hrd_parameters_present_flag)
//        {
//            h265_read_sub_layer_hrd_parameters(&(hrd->sub_layer_hrd_parameters), b, hrd->sub_pic_hrd_params_present_flag, hrd->cpb_cnt_minus1[i]);
//        }
//        if (hrd->vcl_hrd_parameters_present_flag)
//        {
//            h265_read_sub_layer_hrd_parameters(&(hrd->sub_layer_hrd_parameters_v), b, hrd->sub_pic_hrd_params_present_flag, hrd->cpb_cnt_minus1[i]);
//        }
//    }
//}
//
//
//void h265Read_sps_rbsp(h265_stream_t* h, bs_t* b) {
//    // NOTE 不能直接赋值给sps，因为还未知是哪一个sps。
//
//    int sps_video_parameter_set_id = 0;
//    int sps_max_sub_layers_minus1 = 0;
//    int sps_temporal_id_nesting_flag = 0;
//    int sps_seq_parameter_set_id = 0;
//    profile_tier_level_t profile_tier_level;
//
//    sps_video_parameter_set_id = bs_read_u(b, 4);
//    sps_max_sub_layers_minus1 = bs_read_u(b, 3);
//    sps_temporal_id_nesting_flag = bs_read_u1(b);
//
//    // profile tier level...
//    memset(&profile_tier_level, '\0', sizeof(profile_tier_level_t));
//
//    h265Read_ptl(&profile_tier_level, b, 1, sps_max_sub_layers_minus1);
//
//    sps_seq_parameter_set_id = bs_read_ue(b);
//    // 选择正确的sps表
//    h->sps = h->sps_table[sps_seq_parameter_set_id];
//    h265_sps_t* sps = h->sps;
//    memset(sps, 0, sizeof(h265_sps_t));
//
//    sps->sps_video_parameter_set_id = sps_video_parameter_set_id;
//    sps->sps_max_sub_layers_minus1 = sps_max_sub_layers_minus1;
//    sps->sps_temporal_id_nesting_flag = sps_temporal_id_nesting_flag;
//
//    memcpy(&(sps->ptl), &profile_tier_level, sizeof(profile_tier_level_t)); // ptl
//
//    sps->sps_seq_parameter_set_id = sps_seq_parameter_set_id;
//    sps->chroma_format_idc = bs_read_ue(b);
//    h->info->chroma_format_idc = sps->chroma_format_idc;
//    if (sps->chroma_format_idc == 3)
//    {
//        sps->separate_colour_plane_flag = bs_read_u1(b);
//    }
//    sps->pic_width_in_luma_samples = bs_read_ue(b);
//    sps->pic_height_in_luma_samples = bs_read_ue(b);
//
//    h->info->width = sps->pic_width_in_luma_samples;
//    h->info->height = sps->pic_height_in_luma_samples;
//
//    sps->conformance_window_flag = bs_read_u1(b);
//    if (sps->conformance_window_flag)
//    {
//        sps->conf_win_left_offset = bs_read_ue(b);
//        sps->conf_win_right_offset = bs_read_ue(b);
//        sps->conf_win_top_offset = bs_read_ue(b);
//        sps->conf_win_bottom_offset = bs_read_ue(b);
//
//        // calc width & height again...
//        h->info->crop_left = sps->conf_win_left_offset;
//        h->info->crop_right = sps->conf_win_right_offset;
//        h->info->crop_top = sps->conf_win_top_offset;
//        h->info->crop_bottom = sps->conf_win_bottom_offset;
//
//        // 根据Table6-1及7.4.3.2.1重新计算宽、高
//        // 注意：手册里加1，实际上不用
//        // 参考：https://github.com/mbunkus/mkvtoolnix/issues/1152
//        int sub_width_c = ((1 == sps->chroma_format_idc) || (2 == sps->chroma_format_idc)) && (0 == sps->separate_colour_plane_flag) ? 2 : 1;
//        int sub_height_c = (1 == sps->chroma_format_idc) && (0 == sps->separate_colour_plane_flag) ? 2 : 1;
//        h->info->width -= (sub_width_c * sps->conf_win_right_offset + sub_width_c * sps->conf_win_left_offset);
//        h->info->height -= (sub_height_c * sps->conf_win_bottom_offset + sub_height_c * sps->conf_win_top_offset);
//    }
//
//    sps->bit_depth_luma_minus8 = bs_read_ue(b);
//    sps->bit_depth_chroma_minus8 = bs_read_ue(b);
//
//    // bit depth
//    h->info->bit_depth_luma = sps->bit_depth_luma_minus8 + 8;
//    h->info->bit_depth_chroma = sps->bit_depth_chroma_minus8 + 8;
//
//    sps->log2_max_pic_order_cnt_lsb_minus4 = bs_read_ue(b);
//
//    sps->sps_sub_layer_ordering_info_present_flag = bs_read_u1(b);
//    for (int i = (sps->sps_sub_layer_ordering_info_present_flag ? 0 : sps->sps_max_sub_layers_minus1);
//        i <= sps->sps_max_sub_layers_minus1; i++)
//    {
//        sps->sps_max_dec_pic_buffering_minus1[i] = bs_read_ue(b);
//        sps->sps_max_num_reorder_pics[i] = bs_read_ue(b);
//        sps->sps_max_latency_increase_plus1[i] = bs_read_ue(b);
//    }
//
//    sps->log2_min_luma_coding_block_size_minus3 = bs_read_ue(b);
//    sps->log2_diff_max_min_luma_coding_block_size = bs_read_ue(b);
//    sps->log2_min_luma_transform_block_size_minus2 = bs_read_ue(b);
//    sps->log2_diff_max_min_luma_transform_block_size = bs_read_ue(b);
//    sps->max_transform_hierarchy_depth_inter = bs_read_ue(b);
//    sps->max_transform_hierarchy_depth_intra = bs_read_ue(b);
//
//    sps->scaling_list_enabled_flag = bs_read_u1(b);
//    if (sps->scaling_list_enabled_flag)
//    {
//        sps->sps_scaling_list_data_present_flag = bs_read_u1(b);
//        if (sps->sps_scaling_list_data_present_flag)
//        {
//            // scaling_list_data()
//            h265Read_scaling_list(&(sps->scaling_list_data), b);
//        }
//    }
//
//    sps->amp_enabled_flag = bs_read_u1(b);
//    sps->sample_adaptive_offset_enabled_flag = bs_read_u1(b);
//    sps->pcm_enabled_flag = bs_read_u1(b);
//    if (sps->pcm_enabled_flag)
//    {
//        sps->pcm_sample_bit_depth_luma_minus1 = bs_read_u(b, 4);
//        sps->pcm_sample_bit_depth_chroma_minus1 = bs_read_u(b, 4);
//        sps->log2_min_pcm_luma_coding_block_size_minus3 = bs_read_ue(b);
//        sps->log2_diff_max_min_pcm_luma_coding_block_size = bs_read_ue(b);
//        sps->pcm_loop_filter_disabled_flag = bs_read_u1(b);
//    }
//
//    sps->num_short_term_ref_pic_sets = bs_read_ue(b);
//    // 根据num_short_term_ref_pic_sets创建数组
//    sps->st_ref_pic_set.resize(sps->num_short_term_ref_pic_sets);
//    sps->m_RPSList.resize(sps->num_short_term_ref_pic_sets); // 确定一共有多少个RPS列表
//    referencePictureSets_t* rps = NULL;
//    st_ref_pic_set_t* st = NULL;
//    for (int i = 0; i < sps->num_short_term_ref_pic_sets; i++)
//    {
//        st = &sps->st_ref_pic_set[i];
//        rps = &sps->m_RPSList[i];
//        h265Read_short_term_ref_pic_set(b, sps, st, rps, i);
//    }
//
//    sps->long_term_ref_pics_present_flag = bs_read_u1(b);
//    if (sps->long_term_ref_pics_present_flag)
//    {
//        sps->num_long_term_ref_pics_sps = bs_read_ue(b);
//        sps->lt_ref_pic_poc_lsb_sps.resize(sps->num_long_term_ref_pics_sps);
//        sps->used_by_curr_pic_lt_sps_flag.resize(sps->num_long_term_ref_pics_sps);
//        for (int i = 0; i < sps->num_long_term_ref_pics_sps; i++)
//        {
//            sps->lt_ref_pic_poc_lsb_sps_bytes = sps->log2_max_pic_order_cnt_lsb_minus4 + 4;
//            sps->lt_ref_pic_poc_lsb_sps[i] = bs_read_u(b, sps->log2_max_pic_order_cnt_lsb_minus4 + 4); // u(v)
//            sps->used_by_curr_pic_lt_sps_flag[i] = bs_read_u1(b);
//        }
//    }
//
//    sps->sps_temporal_mvp_enabled_flag = bs_read_u1(b);
//    sps->strong_intra_smoothing_enabled_flag = bs_read_u1(b);
//    sps->vui_parameters_present_flag = bs_read_u1(b);
//    if (sps->vui_parameters_present_flag)
//    {
//        h265Read_vui_parameters(&(sps->vui), b, sps->sps_max_sub_layers_minus1);
//        // calc fps
//        if (sps->vui.vui_num_units_in_tick != 0)
//            h->info->max_framerate = (float)(sps->vui.vui_time_scale) / (float)(sps->vui.vui_num_units_in_tick);
//    }
//
//    sps->sps_extension_present_flag = bs_read_u1(b);
//    if (sps->sps_extension_present_flag)
//    {
//        sps->sps_range_extension_flag = bs_read_u1(b);
//        sps->sps_multilayer_extension_flag = bs_read_u1(b);
//        sps->sps_3d_extension_flag = bs_read_u1(b);
//        sps->sps_extension_5bits = bs_read_u(b, 5);
//    }
//
//    if (sps->sps_range_extension_flag)
//    {
//        sps->sps_range_extension.transform_skip_rotation_enabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.transform_skip_context_enabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.implicit_rdpcm_enabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.explicit_rdpcm_enabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.extended_precision_processing_flag = bs_read_u1(b);
//        sps->sps_range_extension.intra_smoothing_disabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.high_precision_offsets_enabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.persistent_rice_adaptation_enabled_flag = bs_read_u1(b);
//        sps->sps_range_extension.cabac_bypass_alignment_enabled_flag = bs_read_u1(b);
//    }
//    if (sps->sps_multilayer_extension_flag)
//    {
//        // sps_multilayer_extension()
//        sps->inter_view_mv_vert_constraint_flag = bs_read_u1(b);
//    }
//    if (sps->sps_3d_extension_flag)
//    {
//        // todo sps_3d_extension( )
//    }
//    if (sps->sps_extension_5bits)
//    {
//        while (h265_more_rbsp_trailing_data(b))
//        {
//            int sps_extension_data_flag = bs_read_u1(b);
//        }
//    }
//    h265_read_rbsp_trailing_bits(b);
//}
//
//void h265Read_vps_rbsp(h265_stream_t* h, bs_t* b) {
//    int i = 0;
//    int j = 0;
//
//    int vps_video_parameter_set_id = bs_read_u(b, 4);
//    // 选择正确的sps表
//    h->vps = h->vps_table[vps_video_parameter_set_id];
//    h265_vps_t* vps = h->vps;
//    memset(vps, 0, sizeof(h265_vps_t));
//
//    vps->vps_video_parameter_set_id = vps_video_parameter_set_id;
//    vps->vps_base_layer_internal_flag = bs_read_u1(b);
//    vps->vps_base_layer_available_flag = bs_read_u1(b);
//    vps->vps_max_layers_minus1 = bs_read_u(b, 6);
//    vps->vps_max_sub_layers_minus1 = bs_read_u(b, 3);
//    vps->vps_temporal_id_nesting_flag = bs_read_u1(b);
//    vps->vps_reserved_0xffff_16bits = bs_read_u(b, 16);
//
//    // profile tier level...
//    h265Read_ptl(&vps->ptl, b, 1, vps->vps_max_sub_layers_minus1);
//    h->info->profile_idc = vps->ptl.general_profile_idc;
//    h->info->level_idc = vps->ptl.general_level_idc;
//    h->info->tier_idc = vps->ptl.general_tier_flag;
//
//    vps->vps_sub_layer_ordering_info_present_flag = bs_read_u1(b);
//    for (i = (vps->vps_sub_layer_ordering_info_present_flag ? 0 : vps->vps_max_sub_layers_minus1);
//        i <= vps->vps_max_sub_layers_minus1; i++)
//    {
//        vps->vps_max_dec_pic_buffering_minus1[i] = bs_read_ue(b);
//        vps->vps_max_num_reorder_pics[i] = bs_read_ue(b);
//        vps->vps_max_latency_increase_plus1[i] = bs_read_ue(b);
//    }
//    vps->vps_max_layer_id = bs_read_u(b, 6);
//    vps->vps_num_layer_sets_minus1 = bs_read_ue(b);
//    vps->layer_id_included_flag.resize(vps->vps_num_layer_sets_minus1 + 1);
//    for (unsigned int k = 0; k < vps->layer_id_included_flag.size(); k++)
//    {
//        vps->layer_id_included_flag[k].resize(vps->vps_max_layer_id);
//    }
//    for (i = 1; i <= vps->vps_num_layer_sets_minus1; i++)
//    {
//        vps->layer_id_included_flag[i].resize(vps->vps_num_layer_sets_minus1 + 1);
//    }
//    for (i = 1; i <= vps->vps_num_layer_sets_minus1; i++)
//    {
//        for (j = 0; j <= vps->vps_max_layer_id; j++)
//        {
//            vps->layer_id_included_flag[i][j] = bs_read_u1(b);
//        }
//    }
//    vps->vps_timing_info_present_flag = bs_read_u1(b);
//    if (vps->vps_timing_info_present_flag)
//    {
//        vps->vps_num_units_in_tick = bs_read_u(b, 32);
//        vps->vps_time_scale = bs_read_u(b, 32);
//        vps->vps_poc_proportional_to_timing_flag = bs_read_u1(b);
//        if (vps->vps_poc_proportional_to_timing_flag)
//        {
//            vps->vps_num_ticks_poc_diff_one_minus1 = bs_read_ue(b);
//        }
//        vps->vps_num_hrd_parameters = bs_read_ue(b);
//        vps->hrd_layer_set_idx.resize(vps->vps_num_hrd_parameters);
//        vps->cprms_present_flag.resize(vps->vps_num_hrd_parameters);
//        vps->hrd_layer_set_idx.resize(vps->vps_num_hrd_parameters);
//        vps->cprms_present_flag.resize(vps->vps_num_hrd_parameters);
//        for (i = 0; i < vps->vps_num_hrd_parameters; i++)
//        {
//            vps->hrd_layer_set_idx[i] = bs_read_ue(b);
//            if (i > 0)
//            {
//                vps->cprms_present_flag[i] = bs_read_u1(b);
//            }
//            //  hrd_parameters()
//            h265Read_hrd_parameters(&(vps->hrd_parameters), b, vps->cprms_present_flag[i], vps->vps_max_sub_layers_minus1);
//        }
//    }
//    vps->vps_extension_flag = bs_read_u1(b);
//    if (vps->vps_extension_flag)
//    {
//        while (h265_more_rbsp_trailing_data(b))
//        {
//            int sps_extension_data_flag = bs_read_u1(b);
//        }
//    }
//    h265_read_rbsp_trailing_bits(b);
//}
//void h265Read_pps_rbsp(h265_stream_t* h, bs_t* b) {
//    int pps_pic_parameter_set_id = bs_read_ue(b); // get id
//
//    h265_pps_t* pps = h->pps = h->pps_table[pps_pic_parameter_set_id];
//
//    memset(pps, 0, sizeof(h265_pps_t));
//
//    pps->pps_pic_parameter_set_id = pps_pic_parameter_set_id;
//    pps->pps_seq_parameter_set_id = bs_read_ue(b);
//    pps->dependent_slice_segments_enabled_flag = bs_read_u1(b);
//    pps->output_flag_present_flag = bs_read_u1(b);
//    pps->num_extra_slice_header_bits = bs_read_u(b, 3);
//    pps->sign_data_hiding_enabled_flag = bs_read_u1(b);
//    pps->cabac_init_present_flag = bs_read_u1(b);
//    pps->num_ref_idx_l0_default_active_minus1 = bs_read_ue(b);
//    pps->num_ref_idx_l1_default_active_minus1 = bs_read_ue(b);
//    pps->init_qp_minus26 = bs_read_se(b);
//    pps->constrained_intra_pred_flag = bs_read_u1(b);
//    pps->transform_skip_enabled_flag = bs_read_u1(b);
//    pps->cu_qp_delta_enabled_flag = bs_read_u1(b);
//    if (pps->cu_qp_delta_enabled_flag)
//    {
//        pps->diff_cu_qp_delta_depth = bs_read_ue(b);
//    }
//
//    pps->pps_cb_qp_offset = bs_read_se(b);
//    pps->pps_cr_qp_offset = bs_read_se(b);
//    pps->pps_slice_chroma_qp_offsets_present_flag = bs_read_u1(b);
//    pps->weighted_pred_flag = bs_read_u1(b);
//    pps->weighted_bipred_flag = bs_read_u1(b);
//    pps->transquant_bypass_enabled_flag = bs_read_u1(b);
//    pps->tiles_enabled_flag = bs_read_u1(b);
//    pps->entropy_coding_sync_enabled_flag = bs_read_u1(b);
//    h->info->encoding_type = pps->entropy_coding_sync_enabled_flag;
//
//    if (pps->tiles_enabled_flag)
//    {
//        pps->num_tile_columns_minus1 = bs_read_ue(b);
//        pps->num_tile_rows_minus1 = bs_read_ue(b);
//        pps->uniform_spacing_flag = bs_read_u1(b);
//        if (!pps->uniform_spacing_flag)
//        {
//            pps->column_width_minus1.resize(pps->num_tile_columns_minus1);
//            pps->row_height_minus1.resize(pps->num_tile_rows_minus1);
//            for (int i = 0; i < pps->num_tile_columns_minus1; i++)
//            {
//                pps->column_width_minus1[i] = bs_read_ue(b);
//            }
//            for (int i = 0; i < pps->num_tile_rows_minus1; i++)
//            {
//                pps->row_height_minus1[i] = bs_read_ue(b);
//            }
//        }
//        pps->loop_filter_across_tiles_enabled_flag = bs_read_u1(b);
//    }
//
//    pps->pps_loop_filter_across_slices_enabled_flag = bs_read_u1(b);
//    pps->deblocking_filter_control_present_flag = bs_read_u1(b);
//    if (pps->deblocking_filter_control_present_flag)
//    {
//        pps->deblocking_filter_override_enabled_flag = bs_read_u1(b);
//        pps->pps_deblocking_filter_disabled_flag = bs_read_u1(b);
//        if (pps->pps_deblocking_filter_disabled_flag)
//        {
//            pps->pps_beta_offset_div2 = bs_read_se(b);
//            pps->pps_tc_offset_div2 = bs_read_se(b);
//        }
//    }
//
//    pps->pps_scaling_list_data_present_flag = bs_read_u1(b);
//    if (pps->pps_scaling_list_data_present_flag)
//    {
//        // scaling_list_data()
//        h265Read_scaling_list(&(pps->scaling_list_data), b);
//    }
//
//    pps->lists_modification_present_flag = bs_read_u1(b);
//    pps->log2_parallel_merge_level_minus2 = bs_read_ue(b);
//    pps->slice_segment_header_extension_present_flag = bs_read_u1(b);
//    pps->pps_extension_present_flag = bs_read_u1(b);
//    if (pps->pps_extension_present_flag)
//    {
//        pps->pps_range_extension_flag = bs_read_u1(b);
//        pps->pps_multilayer_extension_flag = bs_read_u1(b);
//        pps->pps_3d_extension_flag = bs_read_u1(b);
//        pps->pps_extension_5bits = bs_read_u(b, 5);
//    }
//    if (pps->pps_range_extension_flag)
//    {
//        if (pps->transform_skip_enabled_flag)
//        {
//            pps->pps_range_extension.log2_max_transform_skip_block_size_minus2 = bs_read_ue(b);
//        }
//        pps->pps_range_extension.cross_component_prediction_enabled_flag = bs_read_u1(b);
//        pps->pps_range_extension.chroma_qp_offset_list_enabled_flag = bs_read_u1(b);
//        if (pps->pps_range_extension.chroma_qp_offset_list_enabled_flag)
//        {
//            pps->pps_range_extension.diff_cu_chroma_qp_offset_depth = bs_read_ue(b);
//            pps->pps_range_extension.chroma_qp_offset_list_len_minus1 = bs_read_ue(b);
//            pps->pps_range_extension.cb_qp_offset_list.resize(pps->pps_range_extension.chroma_qp_offset_list_len_minus1);
//            pps->pps_range_extension.cr_qp_offset_list.resize(pps->pps_range_extension.chroma_qp_offset_list_len_minus1);
//            for (int i = 0; i < pps->pps_range_extension.chroma_qp_offset_list_len_minus1; i++)
//            {
//                pps->pps_range_extension.cb_qp_offset_list[i] = bs_read_se(b);
//                pps->pps_range_extension.cr_qp_offset_list[i] = bs_read_se(b);
//            }
//        }
//        pps->pps_range_extension.log2_sao_offset_scale_luma = bs_read_ue(b);
//        pps->pps_range_extension.log2_sao_offset_scale_chroma = bs_read_ue(b);
//    }
//    if (pps->pps_multilayer_extension_flag)
//    {
//        // todo sps_multilayer_extension( )
//    }
//    if (pps->pps_3d_extension_flag)
//    {
//        // todo sps_3d_extension( )
//    }
//    if (pps->pps_extension_5bits)
//    {
//        while (h265_more_rbsp_trailing_data(b))
//        {
//            int pps_extension_data_flag = bs_read_u1(b);
//        }
//    }
//    h265_read_rbsp_trailing_bits(b);
//}
//
//int h265ReadNalUnit(h265_stream_t* h, uint8_t* buf, int size) {
//    h265_nal_t* nal = h->nal;
//
//    bs_t* b = bs_new(buf, size);
//    // nal header
//    nal->forbidden_zero_bit = bs_read_f(b, 1);
//    nal->nal_unit_type = bs_read_u(b, 6);
//    nal->nuh_layer_id = bs_read_u(b, 6);
//    nal->nuh_temporal_id_plus1 = bs_read_u(b, 3);
//    nal->parsed = NULL;
//    nal->sizeof_parsed = 0;
//    bs_free(b);
//
//    int nal_size = size;
//    int rbsp_size = size;
//    uint8_t* rbsp_buf = (uint8_t*)malloc(rbsp_size);
//
//    int rc = nal_to_rbsp(2, buf, &nal_size, rbsp_buf, &rbsp_size);
//
//    if (rc < 0) { free(rbsp_buf); return -1; } // handle conversion error
//
//    b = bs_new(rbsp_buf, rbsp_size);
//
//    // nal data
//    switch (nal->nal_unit_type)
//    {
//    case NAL_UNIT_VPS:
//        h265Read_vps_rbsp(h, b);
//        break;
//    case NAL_UNIT_SPS:
//        h265Read_sps_rbsp(h, b);
//        nal->parsed = h->sps;
//        nal->sizeof_parsed = sizeof(h265_sps_t);
//        break;
//    case NAL_UNIT_PPS:
//        h265Read_pps_rbsp(h, b);
//        nal->parsed = h->pps;
//        nal->sizeof_parsed = sizeof(h265_pps_t);
//        break;
//
//    default:
//        // here comes the reserved/unspecified/ignored stuff
//        nal->parsed = NULL;
//        nal->sizeof_parsed = 0;
//        return 0;
//    }
//
//    if (bs_overrun(b)) { bs_free(b); free(rbsp_buf); return -1; }
//
//    bs_free(b);
//    free(rbsp_buf);
//
//    return nal_size;
//}
//
#define BS_READ_WRITE_U1(bin, para) \
    para = bs_read_u1(bin);  \
    bs_write_u1(bout, para); 

#define BS_READ_WRITE_U(bin,bitsize, para) \
    para = bs_read_u(bin, bitsize);  \
    bs_write_u(bout, bitsize, para);

#define BS_READ_WRITE_U8(bin, para) \
    para = bs_read_u8(bin);  \
    bs_write_u8(bout, para);

#define BS_READ_WRITE_UE(bin,para) \
    para = bs_read_ue(bin);  \
    bs_write_ue(bout, para);

#define BS_READ_WRITE_F(bin,bitsize,para) \
    para = bs_read_f(bin, bitsize);  \
    bs_write_u(bout, bitsize, para);

#define BS_READ_WRITE_SE(bin,para) \
    para = bs_read_se(bin);  \
    bs_write_se(bout, para);

void h265Read_hrd_parameters(hrd_parameters_t* hrd, bs_t* b, int commonInfPresentFlag, int maxNumSubLayersMinus1, bs_t* bout);


//while (!bs_byte_aligned(b))
//{
//    int rbsp_alignment_zero_bit = 0;
//    BS_READ_WRITE_U1(b, rbsp_alignment_zero_bit);
//}
//while (!bs_eof(b))
//{
//    int rbsp_alignment_zero_bit = 0;
//    BS_READ_WRITE_U8(b, rbsp_alignment_zero_bit);
//}
//while (!bs_byte_aligned(bout))
//{
//    int rbsp_alignment_zero_bit = 0;
//    bs_write_u1(bout, 0);
//}
//return;

h265_stream_t* h265New() {
    h265_stream_t* h = (h265_stream_t*)calloc(1, sizeof(h265_stream_t));

    h->nal = (h265_nal_t*)calloc(1, sizeof(h265_nal_t));

    // initialize tables
    for (int i = 0; i < 16; i++) { h->vps_table[i] = (h265_vps_t*)calloc(1, sizeof(h265_vps_t)); }
    for (int i = 0; i < 32; i++) { h->sps_table[i] = (h265_sps_t*)calloc(1, sizeof(h265_sps_t)); }
    for (int i = 0; i < 256; i++) { h->pps_table[i] = (h265_pps_t*)calloc(1, sizeof(h265_pps_t)); }

    h->vps = h->vps_table[0];
    h->sps = h->sps_table[0];
    h->pps = h->pps_table[0];
    h->aud = (h265_aud_t*)calloc(1, sizeof(h265_aud_t));
    h->num_seis = 0;
    h->seis = NULL;
    h->sei = NULL;  //This is a TEMP pointer at whats in h->seis...
    h->sh = (h265_slice_header_t*)calloc(1, sizeof(h265_slice_header_t));

    h->info = (videoinfo_t*)calloc(1, sizeof(videoinfo_t));
    h->info->type = 1;
    return h;
}
void h265Free(h265_stream_t* h) {
    free(h->nal);
    for (int i = 0; i < 16; i++) { if (h->vps_table[i] != NULL) free(h->vps_table[i]); }
    for (int i = 0; i < 32; i++) { if (h->sps_table[i] != NULL) free(h->sps_table[i]); }
    for (int i = 0; i < 256; i++) { if (h->pps_table[i] != NULL) free(h->pps_table[i]); }

    if (h->aud != NULL)
    {
        free(h->aud);
    }

    if (h->seis != NULL)
    {
        for (int i = 0; i < h->num_seis; i++)
        {
            h265_sei_t* sei = h->seis[i];
            h265_sei_free(sei);
        }
        free(h->seis);
    }
    free(h->sh);
    free(h->info);
    free(h);
}
void h265Read_rbsp_trailing_bits(bs_t* b, bs_t* bout);
//7.3.2.10 RBSP slice trailing bits syntax
// 与h.264略有不同
void h265Read_rbsp_slice_trailing_bits(bs_t* b, bs_t* bout)
{
    h265Read_rbsp_trailing_bits(b,bout);
    while (h265_more_rbsp_trailing_data(b))
    {
        int cabac_zero_word = 0;
        BS_READ_WRITE_F(b, 16,cabac_zero_word)
    }
}

//7.3.2.11 RBSP trailing bits syntax
void h265Read_rbsp_trailing_bits(bs_t* b,bs_t* bout)
{
    int rbsp_stop_one_bit = 0;
    BS_READ_WRITE_U1(b, rbsp_stop_one_bit);
  

    while (!bs_byte_aligned(b))
    {
        int rbsp_alignment_zero_bit = 0;
        BS_READ_WRITE_U1(b, rbsp_alignment_zero_bit);
    }
}

// E.2.1  VUI parameters syntax
void h265Read_vui_parameters(vui_parameters_t* vui, bs_t* b, int maxNumSubLayersMinus1, bs_t* bout)
{
    BS_READ_WRITE_U1(b, vui->aspect_ratio_info_present_flag);

    if (vui->aspect_ratio_info_present_flag)
    {
        BS_READ_WRITE_U8(b, vui->aspect_ratio_idc);

        if (vui->aspect_ratio_idc == H265_SAR_Extended)
        {
            BS_READ_WRITE_U(b, 16, vui->sar_width);

            BS_READ_WRITE_U(b, 16, vui->sar_height);

        }
    }
    BS_READ_WRITE_U1(b, vui->overscan_info_present_flag);

    if (vui->overscan_info_present_flag)
    {
        BS_READ_WRITE_U1(b, vui->overscan_appropriate_flag);

    }
    BS_READ_WRITE_U1(b, vui->video_signal_type_present_flag);

    if (vui->video_signal_type_present_flag)
    {
        BS_READ_WRITE_U(b, 3, vui->video_format);
        vui->video_full_range_flag = bs_read_u1(b);
        vui->video_full_range_flag = 1;
        bs_write_u1(bout, vui->video_full_range_flag);
        
        //BS_READ_WRITE_U1(b, vui->video_full_range_flag);

        BS_READ_WRITE_U1(b, vui->colour_description_present_flag);

        if (vui->colour_description_present_flag)
        {
            BS_READ_WRITE_U8(b, vui->colour_primaries);

            BS_READ_WRITE_U8(b, vui->transfer_characteristics);

            BS_READ_WRITE_U8(b, vui->matrix_coeffs);

        }
    }
    BS_READ_WRITE_U1(b, vui->chroma_loc_info_present_flag);

    if (vui->chroma_loc_info_present_flag)
    {
        BS_READ_WRITE_UE(b, vui->chroma_sample_loc_type_top_field);

        BS_READ_WRITE_UE(b, vui->chroma_sample_loc_type_bottom_field);

    }
    BS_READ_WRITE_U1(b, vui->neutral_chroma_indication_flag);

    BS_READ_WRITE_U1(b, vui->field_seq_flag);

    BS_READ_WRITE_U1(b, vui->frame_field_info_present_flag);

    BS_READ_WRITE_U1(b, vui->default_display_window_flag);

    if (vui->default_display_window_flag)
    {
        BS_READ_WRITE_UE(b, vui->def_disp_win_left_offset);

        BS_READ_WRITE_UE(b, vui->def_disp_win_right_offset);

        BS_READ_WRITE_UE(b, vui->def_disp_win_top_offset);

        BS_READ_WRITE_UE(b, vui->def_disp_win_bottom_offset);

    }
    BS_READ_WRITE_U1(b, vui->vui_timing_info_present_flag);

    if (vui->vui_timing_info_present_flag)
    {
        BS_READ_WRITE_U(b, 32, vui->vui_num_units_in_tick);

        BS_READ_WRITE_U(b, 32, vui->vui_time_scale);

        BS_READ_WRITE_U1(b, vui->vui_poc_proportional_to_timing_flag);

        if (vui->vui_poc_proportional_to_timing_flag)
        {
            BS_READ_WRITE_UE(b, vui->vui_num_ticks_poc_diff_one_minus1);

        }
        BS_READ_WRITE_U1(b, vui->vui_hrd_parameters_present_flag);

        if (vui->vui_hrd_parameters_present_flag)
        {
            h265Read_hrd_parameters(&vui->hrd_parameters, b, 1, maxNumSubLayersMinus1,bout);
        }
    }
    BS_READ_WRITE_U1(b, vui->bitstream_restriction_flag);

    if (vui->bitstream_restriction_flag)
    {
        BS_READ_WRITE_U1(b, vui->tiles_fixed_structure_flag);

        BS_READ_WRITE_U1(b, vui->motion_vectors_over_pic_boundaries_flag);

        BS_READ_WRITE_U1(b, vui->restricted_ref_pic_lists_flag);

        BS_READ_WRITE_UE(b, vui->min_spatial_segmentation_idc);

        BS_READ_WRITE_UE(b, vui->max_bytes_per_pic_denom);

        BS_READ_WRITE_UE(b, vui->max_bits_per_min_cu_denom);

        BS_READ_WRITE_UE(b, vui->log2_max_mv_length_horizontal);

        BS_READ_WRITE_UE(b, vui->log2_max_mv_length_vertical);

    }
}

void h265Read_ptl(profile_tier_level_t* ptl, bs_t* b, int profilePresentFlag, int max_sub_layers_minus1, bs_t* bout)
{
    int i = 0;
    if (profilePresentFlag)
    {
        BS_READ_WRITE_U(b, 2, ptl->general_profile_space);

        BS_READ_WRITE_U1(b, ptl->general_tier_flag);

        BS_READ_WRITE_U(b, 5, ptl->general_profile_idc);

        for (i = 0; i < 32; i++)
        {
            BS_READ_WRITE_U1(b, ptl->general_profile_compatibility_flag[i]);

        }
        BS_READ_WRITE_U1(b, ptl->general_progressive_source_flag);

        BS_READ_WRITE_U1(b, ptl->general_interlaced_source_flag);

        BS_READ_WRITE_U1(b, ptl->general_non_packed_constraint_flag);

        BS_READ_WRITE_U1(b, ptl->general_frame_only_constraint_flag);

        if (ptl->general_profile_idc == 4 || ptl->general_profile_compatibility_flag[4] ||
            ptl->general_profile_idc == 5 || ptl->general_profile_compatibility_flag[5] ||
            ptl->general_profile_idc == 6 || ptl->general_profile_compatibility_flag[6] ||
            ptl->general_profile_idc == 7 || ptl->general_profile_compatibility_flag[7])
        {
            BS_READ_WRITE_U1(b, ptl->general_max_12bit_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_max_10bit_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_max_8bit_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_max_422chroma_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_max_420chroma_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_max_monochrome_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_intra_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_one_picture_only_constraint_flag);

            BS_READ_WRITE_U1(b, ptl->general_lower_bit_rate_constraint_flag);
            uint64_t tmp1 = 0;
            uint64_t tmp2 = 0;
            BS_READ_WRITE_U(b, 32, tmp1);

            BS_READ_WRITE_U(b, 2, tmp2);

            ptl->general_reserved_zero_34bits = tmp1 + tmp2;
        }
        else
        {
            uint64_t tmp1 = 0;
            uint64_t tmp2 = 0;
            BS_READ_WRITE_U(b, 32, tmp1);

            BS_READ_WRITE_U(b, 11, tmp2);

            ptl->general_reserved_zero_43bits = tmp1 + tmp2;
        }
        if ((ptl->general_profile_idc >= 1 && ptl->general_profile_idc <= 5) ||
            ptl->general_profile_compatibility_flag[1] || ptl->general_profile_compatibility_flag[2] ||
            ptl->general_profile_compatibility_flag[3] || ptl->general_profile_compatibility_flag[4] ||
            ptl->general_profile_compatibility_flag[5])
        {
            BS_READ_WRITE_U1(b, ptl->general_inbld_flag);

        }
        else
        {
            BS_READ_WRITE_U1(b, ptl->general_reserved_zero_bit);

        }
    }
    BS_READ_WRITE_U8(b, ptl->general_level_idc);

    ptl->sub_layer_profile_present_flag.resize(max_sub_layers_minus1);
    ptl->sub_layer_level_present_flag.resize(max_sub_layers_minus1);
    for (i = 0; i < max_sub_layers_minus1; i++)
    {
        BS_READ_WRITE_U1(b, ptl->sub_layer_profile_present_flag[i]);

        BS_READ_WRITE_U1(b, ptl->sub_layer_level_present_flag[i]);

    }
    if (max_sub_layers_minus1 > 0)
    {
        for (i = max_sub_layers_minus1; i < 8; i++)
        {
            BS_READ_WRITE_U(b, 2, ptl->reserved_zero_2bits[i]);

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
            BS_READ_WRITE_U(b, 2, ptl->sub_layer_profile_space[i]);

            BS_READ_WRITE_U1(b, ptl->sub_layer_tier_flag[i]);

            BS_READ_WRITE_U(b, 5, ptl->sub_layer_profile_idc[i]);

            for (int j = 0; j < 32; j++)
            {
                BS_READ_WRITE_U1(b, ptl->sub_layer_profile_compatibility_flag[i][j]);

            }
            BS_READ_WRITE_U1(b, ptl->sub_layer_progressive_source_flag[i]);

            BS_READ_WRITE_U1(b, ptl->sub_layer_interlaced_source_flag[i]);

            BS_READ_WRITE_U1(b, ptl->sub_layer_non_packed_constraint_flag[i]);

            BS_READ_WRITE_U1(b, ptl->sub_layer_frame_only_constraint_flag[i]);

            if (ptl->sub_layer_profile_idc[i] == 4 || ptl->sub_layer_profile_compatibility_flag[i][4] ||
                ptl->sub_layer_profile_idc[i] == 5 || ptl->sub_layer_profile_compatibility_flag[i][5] ||
                ptl->sub_layer_profile_idc[i] == 6 || ptl->sub_layer_profile_compatibility_flag[i][6] ||
                ptl->sub_layer_profile_idc[i] == 7 || ptl->sub_layer_profile_compatibility_flag[i][7])
            {
                BS_READ_WRITE_U1(b, ptl->sub_layer_max_12bit_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_max_10bit_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_max_8bit_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_max_422chroma_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_max_420chroma_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_max_monochrome_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_intra_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_one_picture_only_constraint_flag[i]);

                BS_READ_WRITE_U1(b, ptl->sub_layer_lower_bit_rate_constraint_flag[i]);
                uint64_t tmp1 = 0;
                uint64_t tmp2 = 0;
                BS_READ_WRITE_U(b, 32, tmp1);

                BS_READ_WRITE_U(b, 2, tmp2);

                ptl->sub_layer_reserved_zero_34bits[i] = tmp1 + tmp2;
            }
            else
            {
                uint64_t tmp1 = 0;
                uint64_t tmp2 = 0;
                BS_READ_WRITE_U(b, 32, tmp1);

                BS_READ_WRITE_U(b, 12, tmp2);

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
                BS_READ_WRITE_U1(b, ptl->sub_layer_inbld_flag[i]);

            }
            else
            {
                BS_READ_WRITE_U1(b, ptl->sub_layer_reserved_zero_bit[i]);

            }
        }
        if (ptl->sub_layer_level_present_flag[i])
        {
            BS_READ_WRITE_U8(b, ptl->sub_layer_level_idc[i]);

        }
    }
}
// 7.3.4  Scaling list data syntax
void h265Read_scaling_list(scaling_list_data_t* sld, bs_t* b,bs_t* bout)
{
    for (int sizeId = 0; sizeId < 4; sizeId++)
    {
        for (int matrixId = 0; matrixId < 6; matrixId += (sizeId == 3) ? 3 : 1)
        {
            BS_READ_WRITE_U1(b, sld->scaling_list_pred_mode_flag[sizeId][matrixId]);

            if (!sld->scaling_list_pred_mode_flag[sizeId][matrixId])
            {
                BS_READ_WRITE_UE(b, sld->scaling_list_pred_matrix_id_delta[sizeId][matrixId]);

            }
            else
            {
                int nextCoef = 8;
                int coefNum = min(64, (1 << (4 + (sizeId << 1))));
                sld->coefNum = coefNum; // tmp store
                if (sizeId > 1)
                {
                    BS_READ_WRITE_SE(b, sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId]);

                    nextCoef = sld->scaling_list_dc_coef_minus8[sizeId - 2][matrixId] + 8;
                }
                for (int i = 0; i < sld->coefNum; i++)
                {
                    int scaling_list_delta_coef = 0;
                    BS_READ_WRITE_SE(b, scaling_list_delta_coef);

                    nextCoef = (nextCoef + scaling_list_delta_coef + 256) % 256;
                    sld->ScalingList[sizeId][matrixId][i] = nextCoef;
                }
            }
        }
    }
}

// st_ref_pic_set
// 7.3.7  Short-term reference picture set syntax
void h265Read_short_term_ref_pic_set(bs_t* b, h265_sps_t* sps, st_ref_pic_set_t* st, referencePictureSets_t* rps, int stRpsIdx,bs_t* bout)
{
    st->inter_ref_pic_set_prediction_flag = 0;
    if (stRpsIdx != 0)
    {
        BS_READ_WRITE_U1(b, st->inter_ref_pic_set_prediction_flag);

    }
    if (st->inter_ref_pic_set_prediction_flag)
    {
        st->delta_idx_minus1 = 0;
        if (stRpsIdx == sps->m_RPSList.size())
        {
            BS_READ_WRITE_UE(b, st->delta_idx_minus1);

        }
        int rIdx = stRpsIdx - 1 - st->delta_idx_minus1;
        referencePictureSets_t* rpsRef = &sps->m_RPSList[rIdx];

        BS_READ_WRITE_U1(b, st->delta_rps_sign);

        BS_READ_WRITE_UE(b, st->abs_delta_rps_minus1);

        int deltaRPS = (1 - 2 * st->delta_rps_sign) * (st->abs_delta_rps_minus1 + 1); // delta_RPS
        st->used_by_curr_pic_flag.resize(rpsRef->m_numberOfPictures + 1);
        st->use_delta_flag.resize(rpsRef->m_numberOfPictures + 1);
        for (int j = 0; j <= rpsRef->m_numberOfPictures; j++)
        {
            BS_READ_WRITE_U1(b, st->used_by_curr_pic_flag[j]);

            int refIdc = st->used_by_curr_pic_flag[j];
            if (!st->used_by_curr_pic_flag[j])
            {
                BS_READ_WRITE_U1(b, st->use_delta_flag[j]);

                refIdc = st->use_delta_flag[j] << 1; //second bit is "1" if refIdc is 2, "0" if refIdc = 0.
            }
            // todo furture
            if (refIdc == 1 || refIdc == 2)
            {

            }
        }
    }
    else
    {
        BS_READ_WRITE_UE(b, st->num_negative_pics);

        BS_READ_WRITE_UE(b, st->num_positive_pics);


        rps->m_numberOfNegativePictures = st->num_negative_pics;
        rps->m_numberOfPositivePictures = st->num_positive_pics;

        // to check...
        st->delta_poc_s0_minus1.resize(st->num_negative_pics);
        st->used_by_curr_pic_s0_flag.resize(st->num_negative_pics);
        for (int i = 0; i < st->num_negative_pics; i++)
        {
            BS_READ_WRITE_UE(b, st->delta_poc_s0_minus1[i]);

            BS_READ_WRITE_U1(b, st->used_by_curr_pic_s0_flag[i]);

            rps->m_used[i] = st->used_by_curr_pic_s0_flag[i];
        }
        st->delta_poc_s1_minus1.resize(st->num_positive_pics);
        st->used_by_curr_pic_s1_flag.resize(st->num_positive_pics);
        for (int i = 0; i < st->num_positive_pics; i++)
        {
            BS_READ_WRITE_UE(b, st->delta_poc_s1_minus1[i]);

            BS_READ_WRITE_U1(b, st->used_by_curr_pic_s1_flag[i]);

            rps->m_used[i + st->num_negative_pics] = st->used_by_curr_pic_s1_flag[i];
        }
        rps->m_numberOfPictures = rps->m_numberOfNegativePictures + rps->m_numberOfPositivePictures;
    }
}
void h265Read_sub_layer_hrd_parameters(sub_layer_hrd_parameters_t* subhrd, bs_t* b, int sub_pic_hrd_params_present_flag, int CpbCnt,bs_t* bout)
{
    subhrd->bit_rate_value_minus1.resize(CpbCnt + 1);
    subhrd->cpb_size_value_minus1.resize(CpbCnt + 1);
    subhrd->cpb_size_du_value_minus1.resize(CpbCnt + 1);
    subhrd->bit_rate_du_value_minus1.resize(CpbCnt + 1);
    subhrd->cbr_flag.resize(CpbCnt + 1);
    for (int i = 0; i <= CpbCnt; i++)
    {
        BS_READ_WRITE_UE(b, subhrd->bit_rate_value_minus1[i]);

        BS_READ_WRITE_UE(b, subhrd->cpb_size_value_minus1[i]);

        if (sub_pic_hrd_params_present_flag)
        {
            BS_READ_WRITE_UE(b, subhrd->cpb_size_du_value_minus1[i]);

            BS_READ_WRITE_UE(b, subhrd->bit_rate_du_value_minus1[i]);

        }
        BS_READ_WRITE_U1(b, subhrd->cbr_flag[i]);

    }
}
// E.2.2  HRD parameters syntax
void h265Read_hrd_parameters(hrd_parameters_t* hrd, bs_t* b, int commonInfPresentFlag, int maxNumSubLayersMinus1,bs_t* bout)
{
    if (commonInfPresentFlag)
    {
        BS_READ_WRITE_U1(b, hrd->nal_hrd_parameters_present_flag);

        BS_READ_WRITE_U1(b, hrd->vcl_hrd_parameters_present_flag);

        if (hrd->nal_hrd_parameters_present_flag ||
            hrd->vcl_hrd_parameters_present_flag)
        {
            BS_READ_WRITE_U1(b, hrd->sub_pic_hrd_params_present_flag);

            if (hrd->sub_pic_hrd_params_present_flag)
            {
                BS_READ_WRITE_U8(b, hrd->tick_divisor_minus2);

                BS_READ_WRITE_U(b, 5, hrd->du_cpb_removal_delay_increment_length_minus1);

                BS_READ_WRITE_U1(b, hrd->sub_pic_cpb_params_in_pic_timing_sei_flag);

                BS_READ_WRITE_U(b, 5, hrd->dpb_output_delay_du_length_minus1);

            }
            BS_READ_WRITE_U(b, 4, hrd->bit_rate_scale);

            BS_READ_WRITE_U(b, 4, hrd->cpb_size_scale);

            if (hrd->sub_pic_hrd_params_present_flag)
            {
                BS_READ_WRITE_U(b, 4, hrd->cpb_size_du_scale);

            }
            BS_READ_WRITE_U(b, 5, hrd->initial_cpb_removal_delay_length_minus1);

            BS_READ_WRITE_U(b, 5, hrd->au_cpb_removal_delay_length_minus1);

            BS_READ_WRITE_U(b, 5, hrd->dpb_output_delay_length_minus1);

        }
    }
    hrd->fixed_pic_rate_general_flag.resize(maxNumSubLayersMinus1 + 1);
    hrd->fixed_pic_rate_within_cvs_flag.resize(maxNumSubLayersMinus1 + 1);
    hrd->elemental_duration_in_tc_minus1.resize(maxNumSubLayersMinus1 + 1);
    hrd->low_delay_hrd_flag.resize(maxNumSubLayersMinus1 + 1);
    hrd->cpb_cnt_minus1.resize(maxNumSubLayersMinus1 + 1);
    for (int i = 0; i <= maxNumSubLayersMinus1; i++)
    {
        BS_READ_WRITE_U1(b, hrd->fixed_pic_rate_general_flag[i]);

        if (!hrd->fixed_pic_rate_general_flag[i])
        {
            BS_READ_WRITE_U1(b, hrd->fixed_pic_rate_within_cvs_flag[i]);

        }
        if (hrd->fixed_pic_rate_within_cvs_flag[i])
        {
            BS_READ_WRITE_UE(b, hrd->elemental_duration_in_tc_minus1[i]);

        }
        else
        {
            BS_READ_WRITE_U1(b, hrd->low_delay_hrd_flag[i]);

        }
        if (!hrd->low_delay_hrd_flag[i])
        {
            BS_READ_WRITE_U1(b, hrd->cpb_cnt_minus1[i]);

        }
        if (hrd->nal_hrd_parameters_present_flag)
        {
            h265Read_sub_layer_hrd_parameters(&(hrd->sub_layer_hrd_parameters), b, hrd->sub_pic_hrd_params_present_flag, hrd->cpb_cnt_minus1[i],bout);
        }
        if (hrd->vcl_hrd_parameters_present_flag)
        {
            h265Read_sub_layer_hrd_parameters(&(hrd->sub_layer_hrd_parameters_v), b, hrd->sub_pic_hrd_params_present_flag, hrd->cpb_cnt_minus1[i],bout);
        }
    }
}


void h265Read_sps_rbsp(h265_stream_t* h, bs_t* b,bs_t* bout) {
    // NOTE 不能直接赋值给sps，因为还未知是哪一个sps。

    int sps_video_parameter_set_id = 0;
    int sps_max_sub_layers_minus1 = 0;
    int sps_temporal_id_nesting_flag = 0;
    int sps_seq_parameter_set_id = 0;
    profile_tier_level_t profile_tier_level;

    BS_READ_WRITE_U(b, 4, sps_video_parameter_set_id);

    BS_READ_WRITE_U(b, 3, sps_max_sub_layers_minus1);

    BS_READ_WRITE_U1(b, sps_temporal_id_nesting_flag);


    // profile tier level...
    memset(&profile_tier_level, '\0', sizeof(profile_tier_level_t));

    h265Read_ptl(&profile_tier_level, b, 1, sps_max_sub_layers_minus1,bout);

    //change sps_seq_parameter_set_id to 1
    sps_seq_parameter_set_id = bs_read_ue(b);
    bs_write_ue(bout,1);
    //BS_READ_WRITE_UE(b, sps_seq_parameter_set_id);
    sps_seq_parameter_set_id = 1;


    //// 选择正确的sps表
    h->sps = h->sps_table[sps_seq_parameter_set_id];
    h265_sps_t* sps = h->sps;
    memset(sps, 0, sizeof(h265_sps_t));

    sps->sps_video_parameter_set_id = sps_video_parameter_set_id;
    sps->sps_max_sub_layers_minus1 = sps_max_sub_layers_minus1;
    sps->sps_temporal_id_nesting_flag = sps_temporal_id_nesting_flag;

    memcpy(&(sps->ptl), &profile_tier_level, sizeof(profile_tier_level_t)); // ptl

    sps->sps_seq_parameter_set_id = sps_seq_parameter_set_id;
    BS_READ_WRITE_UE(b, sps->chroma_format_idc);

    h->info->chroma_format_idc = sps->chroma_format_idc;
    if (sps->chroma_format_idc == 3)
    {
        BS_READ_WRITE_U1(b, sps->separate_colour_plane_flag);

    }
    BS_READ_WRITE_UE(b, sps->pic_width_in_luma_samples);

    BS_READ_WRITE_UE(b, sps->pic_height_in_luma_samples);


    h->info->width = sps->pic_width_in_luma_samples;
    h->info->height = sps->pic_height_in_luma_samples;

    BS_READ_WRITE_U1(b, sps->conformance_window_flag);

    if (sps->conformance_window_flag)
    {
        BS_READ_WRITE_UE(b, sps->conf_win_left_offset);

        BS_READ_WRITE_UE(b, sps->conf_win_right_offset);

        BS_READ_WRITE_UE(b, sps->conf_win_top_offset);

        BS_READ_WRITE_UE(b, sps->conf_win_bottom_offset);


        // calc width & height again...
        h->info->crop_left = sps->conf_win_left_offset;
        h->info->crop_right = sps->conf_win_right_offset;
        h->info->crop_top = sps->conf_win_top_offset;
        h->info->crop_bottom = sps->conf_win_bottom_offset;

        // 根据Table6-1及7.4.3.2.1重新计算宽、高
        // 注意：手册里加1，实际上不用
        // 参考：https://github.com/mbunkus/mkvtoolnix/issues/1152
        int sub_width_c = ((1 == sps->chroma_format_idc) || (2 == sps->chroma_format_idc)) && (0 == sps->separate_colour_plane_flag) ? 2 : 1;
        int sub_height_c = (1 == sps->chroma_format_idc) && (0 == sps->separate_colour_plane_flag) ? 2 : 1;
        h->info->width -= (sub_width_c * sps->conf_win_right_offset + sub_width_c * sps->conf_win_left_offset);
        h->info->height -= (sub_height_c * sps->conf_win_bottom_offset + sub_height_c * sps->conf_win_top_offset);
    }

    BS_READ_WRITE_UE(b, sps->bit_depth_luma_minus8);

    BS_READ_WRITE_UE(b, sps->bit_depth_chroma_minus8);


    // bit depth
    h->info->bit_depth_luma = sps->bit_depth_luma_minus8 + 8;
    h->info->bit_depth_chroma = sps->bit_depth_chroma_minus8 + 8;

    BS_READ_WRITE_UE(b, sps->log2_max_pic_order_cnt_lsb_minus4);


    BS_READ_WRITE_U1(b, sps->sps_sub_layer_ordering_info_present_flag);

    for (int i = (sps->sps_sub_layer_ordering_info_present_flag ? 0 : sps->sps_max_sub_layers_minus1);
        i <= sps->sps_max_sub_layers_minus1; i++)
    {
        BS_READ_WRITE_UE(b, sps->sps_max_dec_pic_buffering_minus1[i]);

        BS_READ_WRITE_UE(b, sps->sps_max_num_reorder_pics[i]);

        BS_READ_WRITE_UE(b, sps->sps_max_latency_increase_plus1[i]);

    }

    BS_READ_WRITE_UE(b, sps->log2_min_luma_coding_block_size_minus3);

    BS_READ_WRITE_UE(b, sps->log2_diff_max_min_luma_coding_block_size);

    BS_READ_WRITE_UE(b, sps->log2_min_luma_transform_block_size_minus2);

    BS_READ_WRITE_UE(b, sps->log2_diff_max_min_luma_transform_block_size);

    BS_READ_WRITE_UE(b, sps->max_transform_hierarchy_depth_inter);

    BS_READ_WRITE_UE(b, sps->max_transform_hierarchy_depth_intra);


    BS_READ_WRITE_U1(b, sps->scaling_list_enabled_flag);

    if (sps->scaling_list_enabled_flag)
    {
        BS_READ_WRITE_U1(b, sps->sps_scaling_list_data_present_flag);

        if (sps->sps_scaling_list_data_present_flag)
        {
            // scaling_list_data()
            h265Read_scaling_list(&(sps->scaling_list_data), b,bout);
        }
    }

    BS_READ_WRITE_U1(b, sps->amp_enabled_flag);

    BS_READ_WRITE_U1(b, sps->sample_adaptive_offset_enabled_flag);

    BS_READ_WRITE_U1(b, sps->pcm_enabled_flag);

    if (sps->pcm_enabled_flag)
    {
        BS_READ_WRITE_U(b, 4, sps->pcm_sample_bit_depth_luma_minus1);

        BS_READ_WRITE_U(b, 4, sps->pcm_sample_bit_depth_chroma_minus1);

        BS_READ_WRITE_UE(b, sps->log2_min_pcm_luma_coding_block_size_minus3);

        BS_READ_WRITE_UE(b, sps->log2_diff_max_min_pcm_luma_coding_block_size);

        BS_READ_WRITE_U1(b, sps->pcm_loop_filter_disabled_flag);

    }

    BS_READ_WRITE_UE(b, sps->num_short_term_ref_pic_sets);

    // 根据num_short_term_ref_pic_sets创建数组
    sps->st_ref_pic_set.resize(sps->num_short_term_ref_pic_sets);
    sps->m_RPSList.resize(sps->num_short_term_ref_pic_sets); // 确定一共有多少个RPS列表
    referencePictureSets_t* rps = NULL;
    st_ref_pic_set_t* st = NULL;
    for (int i = 0; i < sps->num_short_term_ref_pic_sets; i++)
    {
        st = &sps->st_ref_pic_set[i];
        rps = &sps->m_RPSList[i];
        h265Read_short_term_ref_pic_set(b, sps, st, rps, i,bout);
    }

    BS_READ_WRITE_U1(b, sps->long_term_ref_pics_present_flag);

    if (sps->long_term_ref_pics_present_flag)
    {
        BS_READ_WRITE_UE(b, sps->num_long_term_ref_pics_sps);

        sps->lt_ref_pic_poc_lsb_sps.resize(sps->num_long_term_ref_pics_sps);
        sps->used_by_curr_pic_lt_sps_flag.resize(sps->num_long_term_ref_pics_sps);
        for (int i = 0; i < sps->num_long_term_ref_pics_sps; i++)
        {
            sps->lt_ref_pic_poc_lsb_sps_bytes = sps->log2_max_pic_order_cnt_lsb_minus4 + 4;
            BS_READ_WRITE_U(b, sps->log2_max_pic_order_cnt_lsb_minus4 + 4, sps->lt_ref_pic_poc_lsb_sps[i]);

            BS_READ_WRITE_U1(b, sps->used_by_curr_pic_lt_sps_flag[i]);

        }
    }

    BS_READ_WRITE_U1(b, sps->sps_temporal_mvp_enabled_flag);

    BS_READ_WRITE_U1(b, sps->strong_intra_smoothing_enabled_flag);

    BS_READ_WRITE_U1(b, sps->vui_parameters_present_flag);

    if (sps->vui_parameters_present_flag)
    {
        h265Read_vui_parameters(&(sps->vui), b, sps->sps_max_sub_layers_minus1,bout);
        // fps
        if (sps->vui.vui_num_units_in_tick != 0)
            h->info->max_framerate = (float)(sps->vui.vui_time_scale) / (float)(sps->vui.vui_num_units_in_tick);
    }
    while (!bs_byte_aligned(b))
    {
        int rbsp_alignment_zero_bit = 0;
        BS_READ_WRITE_U1(b, rbsp_alignment_zero_bit);
    }
    while (!bs_eof(b))
    {
        int rbsp_alignment_zero_bit = 0;
        BS_READ_WRITE_U8(b, rbsp_alignment_zero_bit);
    }
    while (!bs_byte_aligned(bout))
    {
        int rbsp_alignment_zero_bit = 0;
        bs_write_u1(bout, 0);
    }
    return;
    //BS_READ_WRITE_U1(b, sps->sps_extension_present_flag);

    //if (sps->sps_extension_present_flag)
    //{
    //    BS_READ_WRITE_U1(b, sps->sps_range_extension_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_multilayer_extension_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_3d_extension_flag);

    //    BS_READ_WRITE_U(b, 5, sps->sps_extension_5bits);

    //}

    //if (sps->sps_range_extension_flag)
    //{
    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.transform_skip_rotation_enabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.transform_skip_context_enabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.implicit_rdpcm_enabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.explicit_rdpcm_enabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.extended_precision_processing_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.intra_smoothing_disabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.high_precision_offsets_enabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.persistent_rice_adaptation_enabled_flag);

    //    BS_READ_WRITE_U1(b, sps->sps_range_extension.cabac_bypass_alignment_enabled_flag);

    //}
    //if (sps->sps_multilayer_extension_flag)
    //{
    //    // sps_multilayer_extension()
    //    BS_READ_WRITE_U1(b, sps->inter_view_mv_vert_constraint_flag);

    //}
    //if (sps->sps_3d_extension_flag)
    //{
    //    // todo sps_3d_extension( )
    //}
    //if (sps->sps_extension_5bits)
    //{
    //    while (h265_more_rbsp_trailing_data(b))
    //    {
    //        int sps_extension_data_flag = 0;
    //        BS_READ_WRITE_U1(b, sps_extension_data_flag);

    //    }
    //}
    //h265Read_rbsp_trailing_bits(b,bout);
}

void h265Read_vps_rbsp(h265_stream_t* h, bs_t* b,bs_t* bout) {
    int i = 0;
    int j = 0;
    int vps_video_parameter_set_id = 0;
    BS_READ_WRITE_U(b, 4, vps_video_parameter_set_id);

    // 选择正确的sps表
    h->vps = h->vps_table[vps_video_parameter_set_id];
    h265_vps_t* vps = h->vps;
    memset(vps, 0, sizeof(h265_vps_t));

    vps->vps_video_parameter_set_id = vps_video_parameter_set_id;
    BS_READ_WRITE_U1(b, vps->vps_base_layer_internal_flag);

    BS_READ_WRITE_U1(b, vps->vps_base_layer_available_flag);

    BS_READ_WRITE_U(b, 6, vps->vps_max_layers_minus1);

    BS_READ_WRITE_U(b, 3, vps->vps_max_sub_layers_minus1);

    BS_READ_WRITE_U1(b, vps->vps_temporal_id_nesting_flag);

    BS_READ_WRITE_U(b, 16, vps->vps_reserved_0xffff_16bits);


    // profile tier level...
    h265Read_ptl(&vps->ptl, b, 1, vps->vps_max_sub_layers_minus1,bout);
    h->info->profile_idc = vps->ptl.general_profile_idc;
    h->info->level_idc = vps->ptl.general_level_idc;
    h->info->tier_idc = vps->ptl.general_tier_flag;

    BS_READ_WRITE_U1(b, vps->vps_sub_layer_ordering_info_present_flag);

    for (i = (vps->vps_sub_layer_ordering_info_present_flag ? 0 : vps->vps_max_sub_layers_minus1);
        i <= vps->vps_max_sub_layers_minus1; i++)
    {
        BS_READ_WRITE_UE(b, vps->vps_max_dec_pic_buffering_minus1[i]);

        BS_READ_WRITE_UE(b, vps->vps_max_num_reorder_pics[i]);

        BS_READ_WRITE_UE(b, vps->vps_max_latency_increase_plus1[i]);

    }
    BS_READ_WRITE_U(b, 6, vps->vps_max_layer_id);

    BS_READ_WRITE_UE(b, vps->vps_num_layer_sets_minus1);

    vps->layer_id_included_flag.resize(vps->vps_num_layer_sets_minus1 + 1);
    for (unsigned int k = 0; k < vps->layer_id_included_flag.size(); k++)
    {
        vps->layer_id_included_flag[k].resize(vps->vps_max_layer_id);
    }
    for (i = 1; i <= vps->vps_num_layer_sets_minus1; i++)
    {
        vps->layer_id_included_flag[i].resize(vps->vps_num_layer_sets_minus1 + 1);
    }
    for (i = 1; i <= vps->vps_num_layer_sets_minus1; i++)
    {
        for (j = 0; j <= vps->vps_max_layer_id; j++)
        {
            BS_READ_WRITE_U1(b, vps->layer_id_included_flag[i][j]);

        }
    }
    BS_READ_WRITE_U1(b, vps->vps_timing_info_present_flag);

    if (vps->vps_timing_info_present_flag)
    {
        BS_READ_WRITE_U(b, 32, vps->vps_num_units_in_tick);

        BS_READ_WRITE_U(b, 32, vps->vps_time_scale);

        BS_READ_WRITE_U1(b, vps->vps_poc_proportional_to_timing_flag);

        if (vps->vps_poc_proportional_to_timing_flag)
        {
            BS_READ_WRITE_UE(b, vps->vps_num_ticks_poc_diff_one_minus1);

        }
        BS_READ_WRITE_UE(b, vps->vps_num_hrd_parameters);

        vps->hrd_layer_set_idx.resize(vps->vps_num_hrd_parameters);
        vps->cprms_present_flag.resize(vps->vps_num_hrd_parameters);
        vps->hrd_layer_set_idx.resize(vps->vps_num_hrd_parameters);
        vps->cprms_present_flag.resize(vps->vps_num_hrd_parameters);
        for (i = 0; i < vps->vps_num_hrd_parameters; i++)
        {
            BS_READ_WRITE_UE(b, vps->hrd_layer_set_idx[i]);

            if (i > 0)
            {
                BS_READ_WRITE_U1(b, vps->cprms_present_flag[i]);

            }
            //  hrd_parameters()
            h265Read_hrd_parameters(&(vps->hrd_parameters), b, vps->cprms_present_flag[i], vps->vps_max_sub_layers_minus1,bout);
        }
    }
    vps->vps_extension_flag = bs_read_u1(b);
    vps->vps_extension_flag = 1;
    bs_write_u1(bout, vps->vps_extension_flag);
    
    //BS_READ_WRITE_U1(b, vps->vps_extension_flag);

    if (vps->vps_extension_flag)
    {
        bs_write_u1(bout, 1);
       /* while (h265_more_rbsp_trailing_data(b))
        {
            int sps_extension_data_flag = 0;
            BS_READ_WRITE_U1(b, sps_extension_data_flag);

        }*/
    }
    //h265Read_rbsp_trailing_bits(b,bout);
    while (!bs_byte_aligned(bout))
    {
        int rbsp_alignment_zero_bit = 0;
        bs_write_u1(bout, 0);
    }
    while (!bs_byte_aligned(b))
    {
        int rbsp_alignment_zero_bit = 0;
        bs_read_u1(b);
    }
    while (!bs_eof(b))
    {
        int rbsp_alignment_zero_bit = 0;
        BS_READ_WRITE_U8(b, rbsp_alignment_zero_bit);
    }
   
    return;
}
void h265Read_pps_rbsp(h265_stream_t* h, bs_t* b,bs_t*bout) {
    int pps_pic_parameter_set_id = 0;
    pps_pic_parameter_set_id = bs_read_ue(b);
    pps_pic_parameter_set_id = 1;
    bs_write_ue(bout, pps_pic_parameter_set_id);

    //BS_READ_WRITE_UE(b, pps_pic_parameter_set_id);


    h265_pps_t* pps = h->pps = h->pps_table[pps_pic_parameter_set_id];

    memset(pps, 0, sizeof(h265_pps_t));

    pps->pps_pic_parameter_set_id = pps_pic_parameter_set_id;
    pps->pps_seq_parameter_set_id = bs_read_ue(b);
    pps->pps_seq_parameter_set_id = 1;
    bs_write_ue(bout, pps->pps_seq_parameter_set_id);
    //BS_READ_WRITE_UE(b, pps->pps_seq_parameter_set_id);
    while (!bs_byte_aligned(b))
    {
        int rbsp_alignment_zero_bit = 0;
        BS_READ_WRITE_U1(b, rbsp_alignment_zero_bit);
    }
    while (!bs_eof(b))
    {
        int rbsp_alignment_zero_bit = 0;
        BS_READ_WRITE_U8(b, rbsp_alignment_zero_bit);
    }
    while (!bs_byte_aligned(bout))
    {
        int rbsp_alignment_zero_bit = 0;
        bs_write_u1(bout, 0);
    }
    return;



    BS_READ_WRITE_U1(b, pps->dependent_slice_segments_enabled_flag);

    BS_READ_WRITE_U1(b, pps->output_flag_present_flag);

    BS_READ_WRITE_U(b, 3, pps->num_extra_slice_header_bits);

    BS_READ_WRITE_U1(b, pps->sign_data_hiding_enabled_flag);

    BS_READ_WRITE_U1(b, pps->cabac_init_present_flag);

    BS_READ_WRITE_UE(b, pps->num_ref_idx_l0_default_active_minus1);

    BS_READ_WRITE_UE(b, pps->num_ref_idx_l1_default_active_minus1);

    BS_READ_WRITE_SE(b, pps->init_qp_minus26);

    BS_READ_WRITE_U1(b, pps->constrained_intra_pred_flag);

    BS_READ_WRITE_U1(b, pps->transform_skip_enabled_flag);

    BS_READ_WRITE_U1(b, pps->cu_qp_delta_enabled_flag);

    if (pps->cu_qp_delta_enabled_flag)
    {
        BS_READ_WRITE_UE(b, pps->diff_cu_qp_delta_depth);

    }

    BS_READ_WRITE_SE(b, pps->pps_cb_qp_offset);

    BS_READ_WRITE_SE(b, pps->pps_cr_qp_offset);

    BS_READ_WRITE_U1(b, pps->pps_slice_chroma_qp_offsets_present_flag);

    BS_READ_WRITE_U1(b, pps->weighted_pred_flag);

    BS_READ_WRITE_U1(b, pps->weighted_bipred_flag);

    BS_READ_WRITE_U1(b, pps->transquant_bypass_enabled_flag);

    BS_READ_WRITE_U1(b, pps->tiles_enabled_flag);

    BS_READ_WRITE_U1(b, pps->entropy_coding_sync_enabled_flag);

    h->info->encoding_type = pps->entropy_coding_sync_enabled_flag;

    if (pps->tiles_enabled_flag)
    {
        BS_READ_WRITE_UE(b, pps->num_tile_columns_minus1);

        BS_READ_WRITE_UE(b, pps->num_tile_rows_minus1);

        BS_READ_WRITE_U1(b, pps->uniform_spacing_flag);

        if (!pps->uniform_spacing_flag)
        {
            pps->column_width_minus1.resize(pps->num_tile_columns_minus1);
            pps->row_height_minus1.resize(pps->num_tile_rows_minus1);
            for (int i = 0; i < pps->num_tile_columns_minus1; i++)
            {
                BS_READ_WRITE_UE(b, pps->column_width_minus1[i]);

            }
            for (int i = 0; i < pps->num_tile_rows_minus1; i++)
            {
                BS_READ_WRITE_UE(b, pps->row_height_minus1[i]);

            }
        }
        BS_READ_WRITE_U1(b, pps->loop_filter_across_tiles_enabled_flag);

    }

    BS_READ_WRITE_U1(b, pps->pps_loop_filter_across_slices_enabled_flag);

    BS_READ_WRITE_U1(b, pps->deblocking_filter_control_present_flag);

    if (pps->deblocking_filter_control_present_flag)
    {
        BS_READ_WRITE_U1(b, pps->deblocking_filter_override_enabled_flag);

        BS_READ_WRITE_U1(b, pps->pps_deblocking_filter_disabled_flag);

        if (pps->pps_deblocking_filter_disabled_flag)
        {
            BS_READ_WRITE_SE(b, pps->pps_beta_offset_div2);

            BS_READ_WRITE_SE(b, pps->pps_tc_offset_div2);

        }
    }

    BS_READ_WRITE_U1(b, pps->pps_scaling_list_data_present_flag);

    if (pps->pps_scaling_list_data_present_flag)
    {
        // scaling_list_data()
        h265Read_scaling_list(&(pps->scaling_list_data), b,bout);
    }

    BS_READ_WRITE_U1(b, pps->lists_modification_present_flag);

    BS_READ_WRITE_UE(b, pps->log2_parallel_merge_level_minus2);

    BS_READ_WRITE_U1(b, pps->slice_segment_header_extension_present_flag);

    BS_READ_WRITE_U1(b, pps->pps_extension_present_flag);

    if (pps->pps_extension_present_flag)
    {
        BS_READ_WRITE_U1(b, pps->pps_range_extension_flag);

        BS_READ_WRITE_U1(b, pps->pps_multilayer_extension_flag);

        BS_READ_WRITE_U1(b, pps->pps_3d_extension_flag);

        BS_READ_WRITE_U(b, 5, pps->pps_extension_5bits);

    }
    if (pps->pps_range_extension_flag)
    {
        if (pps->transform_skip_enabled_flag)
        {
            BS_READ_WRITE_UE(b, pps->pps_range_extension.log2_max_transform_skip_block_size_minus2);

        }
        BS_READ_WRITE_U1(b, pps->pps_range_extension.cross_component_prediction_enabled_flag);

        BS_READ_WRITE_U1(b, pps->pps_range_extension.chroma_qp_offset_list_enabled_flag);

        if (pps->pps_range_extension.chroma_qp_offset_list_enabled_flag)
        {
            BS_READ_WRITE_UE(b, pps->pps_range_extension.diff_cu_chroma_qp_offset_depth);

            BS_READ_WRITE_UE(b, pps->pps_range_extension.chroma_qp_offset_list_len_minus1);

            pps->pps_range_extension.cb_qp_offset_list.resize(pps->pps_range_extension.chroma_qp_offset_list_len_minus1);
            pps->pps_range_extension.cr_qp_offset_list.resize(pps->pps_range_extension.chroma_qp_offset_list_len_minus1);
            for (int i = 0; i < pps->pps_range_extension.chroma_qp_offset_list_len_minus1; i++)
            {
                BS_READ_WRITE_SE(b, pps->pps_range_extension.cb_qp_offset_list[i]);

                BS_READ_WRITE_SE(b, pps->pps_range_extension.cr_qp_offset_list[i]);

            }
        }
        BS_READ_WRITE_UE(b, pps->pps_range_extension.log2_sao_offset_scale_luma);

        BS_READ_WRITE_UE(b, pps->pps_range_extension.log2_sao_offset_scale_chroma);

    }
    if (pps->pps_multilayer_extension_flag)
    {
        // todo sps_multilayer_extension( )
    }
    if (pps->pps_3d_extension_flag)
    {
        // todo sps_3d_extension( )
    }
    if (pps->pps_extension_5bits)
    {
        while (h265_more_rbsp_trailing_data(b))
        {
            int pps_extension_data_flag = 0;
            BS_READ_WRITE_U1(b, pps_extension_data_flag);

        }
    }
    h265Read_rbsp_trailing_bits(b,bout);
}

int h265ReadNalUnit(h265_stream_t* h, uint8_t* buf, int size,std::vector<uint8_t>& output) {
    h265_nal_t* nal = h->nal;
    uint8_t * outputbuf = new uint8_t[size * 2];
    bs_t* b = bs_new(buf, size);
    bs_t* bout = bs_new(outputbuf, size * 2);
    // nal header
    BS_READ_WRITE_F(b, 1, nal->forbidden_zero_bit);

    BS_READ_WRITE_U(b, 6, nal->nal_unit_type);

    BS_READ_WRITE_U(b, 6, nal->nuh_layer_id);

    BS_READ_WRITE_U(b, 3, nal->nuh_temporal_id_plus1);

    nal->parsed = NULL;
    nal->sizeof_parsed = 0;
    bs_free(b);

    int nal_size = size;
    int rbsp_size = size;
    uint8_t* rbsp_buf = (uint8_t*)malloc(rbsp_size);

    int rc = nal_to_rbsp(2, buf, &nal_size, rbsp_buf, &rbsp_size);

    if (rc < 0) { free(rbsp_buf); return -1; } // handle conversion error

    b = bs_new(rbsp_buf, rbsp_size);

    // nal data
    switch (nal->nal_unit_type)
    {
    case NAL_UNIT_VPS:
        h265Read_vps_rbsp(h, b,bout);
        break;
    case NAL_UNIT_SPS:
        h265Read_sps_rbsp(h, b,bout);
        nal->parsed = h->sps;
        nal->sizeof_parsed = sizeof(h265_sps_t);
        break;
    case NAL_UNIT_PPS:
        h265Read_pps_rbsp(h, b,bout);
        nal->parsed = h->pps;
        nal->sizeof_parsed = sizeof(h265_pps_t);
        break;

    default:
        // here comes the reserved/unspecified/ignored stuff
        nal->parsed = NULL;
        nal->sizeof_parsed = 0;
        return 0;
    }

    if (bs_overrun(b)) { bs_free(b); free(rbsp_buf); return -1; }
    while (!bs_eof(b))
    {
        int temp = 0;
        BS_READ_WRITE_U8(b,temp);
    }
    auto bspos = bs_pos(b);
    int input_output = bs_pos(bout);
    for (int i = 0; i < input_output; i++) {
        output.emplace_back(outputbuf[i]);
    }
    delete[] outputbuf;
     bs_free(b);
    free(rbsp_buf);
    bs_free(bout);

    return nal_size;
}
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


void changeNALUSPS(std::vector<uint8_t>& buffer, h265_stream_t* h) {
    std::vector<uint8_t> outputBuffer;

    h265ReadNalUnit(h, buffer.data(), buffer.size(), outputBuffer);
 
    toSODB(outputBuffer);
    buffer = outputBuffer;
}
void changeNALUPPS(std::vector<uint8_t>& buffer, h265_stream_t* h) {
    std::vector<uint8_t> outputBuffer;

    h265ReadNalUnit(h, buffer.data(), buffer.size(), outputBuffer);

    toSODB(outputBuffer);
    buffer = outputBuffer;
}
void changeNALUVPS(std::vector<uint8_t>& buffer, h265_stream_t* h) {
    std::vector<uint8_t> outputBuffer;

    h265ReadNalUnit(h, buffer.data(), buffer.size(), outputBuffer);

    toSODB(outputBuffer);
    buffer = outputBuffer;
}

int main() {
    std::cout << "hello world" << std::endl;
    FileStream alpha("D:/testData/HEVC-Alpah/hevc_alpha.h265");
    FileStream major("D:/testData/HEVC-Alpah/hevc.h265");
    std::ofstream fpOut("D:/testData/HEVC-Alpah/mergexx.h265", std::ios::out | std::ios::binary);
    h265_stream_t* h = h265New();

    int i = 0;
    while (true)
    {
        auto majorNALU = major.getNextNALUnit();
        if (majorNALU.isNULL()) {
            break;
        }
        auto majorData = majorNALU.getNALUnit();
        if (majorNALU.isNULL()) {
            break;
        }
        bool passSEI = false;
        switch (majorNALU.getNALUType()) {
            case NAL_UNIT_VPS:
                changeNALUVPS(majorData,h);
                break;
            case NAL_UNIT_PREFIX_SEI:
                passSEI = true;
                break;
            default:
                break;



        }
        if (!passSEI) {
            fpOut.write(reinterpret_cast<char*>(majorNALU.getStartCode().data()), majorNALU.getStartSize());
            fpOut.write(reinterpret_cast<char*>(majorData.data()), majorData.size());
        }
       
        auto alphaNALU = alpha.getNextNALUnit();
        if (alphaNALU.isNULL()) {
            break;
        }
        auto alphadata = alphaNALU.getNALUnit();
        changeNALU2Alpha(alphadata);
        switch (alphaNALU.getNALUType())
        {
        case NAL_UNIT_VPS:
            passSEI = true;
            break;
        case NAL_UNIT_PPS:
            changeNALUPPS(alphadata,h);
            break;
        case NAL_UNIT_SPS:
            changeNALUSPS(alphadata,h);
            break;
        case NAL_UNIT_PREFIX_SEI:
            passSEI= true;
            break;
        default:
            break;
        }
        if (!passSEI) {
            fpOut.write(reinterpret_cast<char*>(alphaNALU.getStartCode().data()), alphaNALU.getStartSize());
            fpOut.write(reinterpret_cast<char*>(alphadata.data()), alphadata.size());
        }

    }
    h265Free(h);
}
