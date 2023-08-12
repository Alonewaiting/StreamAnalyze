/* 
 * h264bitstream - a library for reading and writing H.264 video
 * Copyright (C) 2005-2007 Auroras Entertainment, LLC
 * Copyright (C) 2008-2011 Avail-TVN
 * 
 * Written by Alex Izvorski <aizvorski@gmail.com> and Alex Giladi <alex.giladi@gmail.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _H264_STREAM_H
#define _H264_STREAM_H        1

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "bsDef.hpp"
#include "bs.h"
#include "h264_sei.h"

#include <vector>
using std::vector;

/**
   Sequence Parameter Set
   @see 7.3.2.1 Sequence parameter set RBSP syntax
   @see read_seq_parameter_set_rbsp
   @see write_seq_parameter_set_rbsp
   @see debug_sps
*/
typedef struct
{
    BIT(int) profile_idc;
    BIT(int) constraint_set0_flag;
    BIT(int) constraint_set1_flag;
    BIT(int) constraint_set2_flag;
    BIT(int) constraint_set3_flag;
    BIT(int) constraint_set4_flag;
    BIT(int) constraint_set5_flag;
    BIT(int) reserved_zero_2bits;
    BIT(int) level_idc;
    BIT(int) seq_parameter_set_id;
    BIT(int) chroma_format_idc;
    BIT(int) separate_colour_plane_flag;
    BIT(int) ChromaArrayType;
    BIT(int) bit_depth_luma_minus8;
    BIT(int) bit_depth_chroma_minus8;
    BIT(int) qpprime_y_zero_transform_bypass_flag;
    BIT(int) seq_scaling_matrix_present_flag;
      int seq_scaling_list_present_flag[8];
      int ScalingList4x4[6];
      int UseDefaultScalingMatrix4x4Flag[6];
      int ScalingList8x8[2];
      int UseDefaultScalingMatrix8x8Flag[2];
      BIT(int) log2_max_frame_num_minus4;
      BIT(int) pic_order_cnt_type;
      BIT(int) log2_max_pic_order_cnt_lsb_minus4;
      BIT(int) delta_pic_order_always_zero_flag;
      BIT(int) offset_for_non_ref_pic;
      BIT(int) offset_for_top_to_bottom_field;
      BIT(int) num_ref_frames_in_pic_order_cnt_cycle;
      int offset_for_ref_frame[256];
    BIT(int) max_num_ref_frames;
    BIT(int) gaps_in_frame_num_value_allowed_flag;
    BIT(int) pic_width_in_mbs_minus1;
    BIT(int) pic_height_in_map_units_minus1;
    BIT(int) frame_mbs_only_flag;
    BIT(int) mb_adaptive_frame_field_flag;
    BIT(int) direct_8x8_inference_flag;
    BIT(int) frame_cropping_flag;
      BIT(int) frame_crop_left_offset;
      BIT(int) frame_crop_right_offset;
      BIT(int) frame_crop_top_offset;
      BIT(int) frame_crop_bottom_offset;
    BIT(int) vui_parameters_present_flag;
    
    struct
    {
        BIT(int) aspect_ratio_info_present_flag;
          BIT(int) aspect_ratio_idc;
            BIT(int) sar_width;
            BIT(int) sar_height;
        BIT(int) overscan_info_present_flag;
          BIT(int) overscan_appropriate_flag;
        BIT(int) video_signal_type_present_flag;
          BIT(int) video_format;
          BIT(int) video_full_range_flag;
          BIT(int) colour_description_present_flag;
            BIT(int) colour_primaries;
            BIT(int) transfer_characteristics;
            BIT(int) matrix_coefficients;
        BIT(int) chroma_loc_info_present_flag;
          BIT(int) chroma_sample_loc_type_top_field;
          BIT(int) chroma_sample_loc_type_bottom_field;
        BIT(int) timing_info_present_flag;
          BIT(int) num_units_in_tick;
          BIT(int) time_scale;
          BIT(int) fixed_frame_rate_flag;
        BIT(int) nal_hrd_parameters_present_flag;
        BIT(int) vcl_hrd_parameters_present_flag;
          BIT(int) low_delay_hrd_flag;
        BIT(int) pic_struct_present_flag;
        BIT(int) bitstream_restriction_flag;
          BIT(int) motion_vectors_over_pic_boundaries_flag;
          BIT(int) max_bytes_per_pic_denom;
          BIT(int) max_bits_per_mb_denom;
          BIT(int) log2_max_mv_length_horizontal;
          BIT(int) log2_max_mv_length_vertical;
          BIT(int) num_reorder_frames;
          BIT(int) max_dec_frame_buffering;
    } vui;

    struct
    {
        BIT(int) cpb_cnt_minus1;
        BIT(int) bit_rate_scale;
        BIT(int) cpb_size_scale;
          int bit_rate_value_minus1[32]; // up to cpb_cnt_minus1, which is <= 31
          int cpb_size_value_minus1[32];
          int cbr_flag[32];
        BIT(int) initial_cpb_removal_delay_length_minus1;
        BIT(int) cpb_removal_delay_length_minus1;
        BIT(int) dpb_output_delay_length_minus1;
        BIT(int) time_offset_length;
    } hrd;

} sps_t;


/**
   Picture Parameter Set
   @see 7.3.2.2 Picture parameter set RBSP syntax
   @see read_pic_parameter_set_rbsp
   @see write_pic_parameter_set_rbsp
   @see debug_pps
*/
typedef struct 
{
    BIT(int) pic_parameter_set_id;
    BIT(int) seq_parameter_set_id;
    BIT(int) entropy_coding_mode_flag;
    BIT(int) pic_order_present_flag; // 2005版本为此字段名 保留，不影响库本身write的编译，但实际不使用
    BIT(int) bottom_field_pic_order_in_frame_present_flag; // 2013版本为此字段名
    BIT(int) num_slice_groups_minus1;
    BIT(int) slice_group_map_type;
      int run_length_minus1[8]; // up to num_slice_groups_minus1, which is <= 7 in Baseline and Extended, 0 otheriwse
      int top_left[8];
      int bottom_right[8];
      BIT(int) slice_group_change_direction_flag;
      BIT(int) slice_group_change_rate_minus1;
      BIT(int) pic_size_in_map_units_minus1;
      BIT(int) slice_group_id_bytes;
      vector<BIT(int)> slice_group_id; // FIXME what size?
    BIT(int) num_ref_idx_l0_active_minus1;
    BIT(int) num_ref_idx_l1_active_minus1;
    BIT(int) weighted_pred_flag;
    BIT(int) weighted_bipred_idc;
    BIT(int) pic_init_qp_minus26;
    BIT(int) pic_init_qs_minus26;
    BIT(int) chroma_qp_index_offset;
    BIT(int) deblocking_filter_control_present_flag;
    BIT(int) constrained_intra_pred_flag;
    BIT(int) redundant_pic_cnt_present_flag;

    // see if we carry any of the optional headers
    BIT(int) _more_rbsp_data_present;

    BIT(int) transform_8x8_mode_flag;
    BIT(int) pic_scaling_matrix_present_flag;
       int pic_scaling_list_present_flag[8];
       int* ScalingList4x4[6];
       int UseDefaultScalingMatrix4x4Flag[6];
       int* ScalingList8x8[2];
       int UseDefaultScalingMatrix8x8Flag[2];
    BIT(int) second_chroma_qp_index_offset;
} pps_t;

// predictive weight table
typedef struct
{
    BIT(int) luma_log2_weight_denom;
    BIT(int) chroma_log2_weight_denom;
    int luma_weight_l0_flag[64];
    int luma_weight_l0[64];
    int luma_offset_l0[64];
    int chroma_weight_l0_flag[64];
    int chroma_weight_l0[64][2];
    int chroma_offset_l0[64][2];
    int luma_weight_l1_flag[64];
    int luma_weight_l1[64];
    int luma_offset_l1[64];
    int chroma_weight_l1_flag[64];
    int chroma_weight_l1[64][2];
    int chroma_offset_l1[64][2];
} pwt_t;

// ref pic list modification
typedef struct
{
    BIT(int) modification_of_pic_nums_idc;
    BIT(int) abs_diff_pic_num_minus1;
    BIT(int) long_term_pic_num;
} rplm_tt;

typedef struct
{
    BIT(int) ref_pic_list_modification_flag_l0;
    BIT(int) ref_pic_list_modification_flag_l1;

    vector<rplm_tt> rplm;
} rplm_t;

// decoded ref pic marking
typedef struct
{
    BIT(int) memory_management_control_operation;
    BIT(int) difference_of_pic_nums_minus1;
    BIT(int) long_term_pic_num;
    BIT(int) long_term_frame_idx;
    BIT(int) max_long_term_frame_idx_plus1;
} drpm_tt;
typedef struct
{
    BIT(int) no_output_of_prior_pics_flag;
    BIT(int) long_term_reference_flag;
    BIT(int) adaptive_ref_pic_marking_mode_flag;

    vector<drpm_tt> drpm;
} drpm_t;

/**
  Slice Header
  @see 7.3.3 Slice header syntax
  @see read_slice_header_rbsp
  @see write_slice_header_rbsp
  @see debug_slice_header_rbsp
*/
typedef struct
{
    BIT(int) read_slice_type; // see if we only read slice type and return

    BIT(int) first_mb_in_slice;
    BIT(int) slice_type;
    BIT(int) pic_parameter_set_id;
    BIT(int) colour_plane_id;
    BIT(int) frame_num_bytes;
    BIT(int) frame_num;
    BIT(int) field_pic_flag;
      BIT(int) bottom_field_flag;
    BIT(int) idr_pic_id;
    BIT(int) pic_order_cnt_lsb_bytes;
    BIT(int) pic_order_cnt_lsb;
    BIT(int) delta_pic_order_cnt_bottom;
    int delta_pic_order_cnt[ 2 ];
    BIT(int) redundant_pic_cnt;
    BIT(int) direct_spatial_mv_pred_flag;
    BIT(int) num_ref_idx_active_override_flag;
    BIT(int) num_ref_idx_l0_active_minus1;
    BIT(int) num_ref_idx_l1_active_minus1;
    BIT(int) cabac_init_idc;
    BIT(int) slice_qp_delta;
    BIT(int) sp_for_switch_flag;
    BIT(int) slice_qs_delta;
    BIT(int) disable_deblocking_filter_idc;
    BIT(int) slice_alpha_c0_offset_div2;
    BIT(int) slice_beta_offset_div2;
    BIT(int) slice_group_change_cycle_bytes;
    BIT(int) slice_group_change_cycle;

    pwt_t pwt;
    rplm_t rplm;
    drpm_t drpm;
} slice_header_t;


/**
   Access unit delimiter
   @see 7.3.1 NAL unit syntax
   @see read_nal_unit
   @see write_nal_unit
   @see debug_nal
*/
typedef struct
{
    BIT(int) primary_pic_type;
} aud_t;

/**
   Network Abstraction Layer (NAL) unit
   @see 7.3.1 NAL unit syntax
   @see read_nal_unit
   @see write_nal_unit
   @see debug_nal
*/
typedef struct
{
    BIT(int) forbidden_zero_bit;
    BIT(int) nal_ref_idc;
    BIT(int) nal_unit_type;
    void* parsed; // FIXME
    BIT(int) sizeof_parsed;

    //uint8_t* rbsp_buf;
    //int rbsp_size;
} nal_t;

typedef struct
{
    BIT(int) _is_initialized;
    BIT(int) sps_id;
    BIT(int) initial_cpb_removal_delay;
    BIT(int) initial_cpb_delay_offset;
} sei_buffering_t;

typedef struct
{
    BIT(int) clock_timestamp_flag;
        BIT(int) ct_type;
        BIT(int) nuit_field_based_flag;
        BIT(int) counting_type;
        BIT(int) full_timestamp_flag;
        BIT(int) discontinuity_flag;
        BIT(int) cnt_dropped_flag;
        BIT(int) n_frames;

        BIT(int) seconds_value;
        BIT(int) minutes_value;
        BIT(int) hours_value;

        BIT(int) seconds_flag;
        BIT(int) minutes_flag;
        BIT(int) hours_flag;

        BIT(int) time_offset;
} picture_timestamp_t;

typedef struct
{
  BIT(int) _is_initialized;
  BIT(int) cpb_removal_delay;
  BIT(int) dpb_output_delay;
  BIT(int) pic_struct;
  picture_timestamp_t clock_timestamps[3]; // 3 is the maximum possible value
} sei_picture_timing_t;


typedef struct
{
  BIT(int) rbsp_size;
  uint8_t* rbsp_buf;
} slice_data_rbsp_t;

typedef struct
{
    BIT(int) type;           // 0:h264 1:h265
    BIT(int) init;
    BIT(int) profile_idc;
    BIT(int) level_idc;
    BIT(int) tier_idc;
    BIT(int) width;
    BIT(int) height;
    BIT(int) crop_left;
    BIT(int) crop_right;
    BIT(int) crop_top;
    BIT(int) crop_bottom;
    float max_framerate;  // 由SPS计算得到的帧率，为0表示SPS中没有相应的字段计算
    BIT(int) chroma_format_idc;  // YUV颜色空间 0: monochrome 1:420 2:422 3:444
    BIT(int) encoding_type;  // 为1表示CABAC 0表示CAVLC
    BIT(int) bit_depth_luma;
    BIT(int) bit_depth_chroma;
} videoinfo_t;


/**
   H264 stream
   Contains data structures for all NAL types that can be handled by this library.  
   When reading, data is read into those, and when writing it is written from those.  
   The reason why they are all contained in one place is that some of them depend on others, we need to 
   have all of them available to read or write correctly.
 */
typedef struct
{
    nal_t* nal;
    sps_t* sps;
    pps_t* pps;
    aud_t* aud;
    sei_t* sei; //This is a TEMP pointer at whats in h->seis...    
    int num_seis;
    slice_header_t* sh;
    slice_data_rbsp_t* slice_data;

    sps_t* sps_table[32];
    pps_t* pps_table[256];
    sei_t** seis;
    videoinfo_t* info;
} h264_stream_t;

h264_stream_t* h264_new();
void h264_free(h264_stream_t* h);

int find_nal_unit(uint8_t* buf, int size, int* nal_start, int* nal_end);

int rbsp_to_nal(const uint8_t* rbsp_buf, const int* rbsp_size, uint8_t* nal_buf, int* nal_size);
int nal_to_rbsp(const int nal_header_size, const uint8_t* nal_buf, int* nal_size, uint8_t* rbsp_buf, int* rbsp_size);

int read_nal_unit(h264_stream_t* h, uint8_t* buf, int size);
int peek_nal_unit(h264_stream_t* h, uint8_t* buf, int size);

void read_seq_parameter_set_rbsp(h264_stream_t* h, bs_t* b);
void read_scaling_list(bs_t* b, int* scalingList, int sizeOfScalingList, int useDefaultScalingMatrixFlag );
void read_vui_parameters(h264_stream_t* h, bs_t* b);
void read_hrd_parameters(h264_stream_t* h, bs_t* b);

void read_pic_parameter_set_rbsp(h264_stream_t* h, bs_t* b);

void read_sei_rbsp(h264_stream_t* h, bs_t* b);
void read_sei_message(h264_stream_t* h, bs_t* b);
void read_access_unit_delimiter_rbsp(h264_stream_t* h, bs_t* b);
void read_end_of_seq_rbsp(h264_stream_t* h, bs_t* b);
void read_end_of_stream_rbsp(h264_stream_t* h, bs_t* b);
void read_filler_data_rbsp(h264_stream_t* h, bs_t* b);

void read_slice_layer_rbsp(h264_stream_t* h, bs_t* b);
void read_rbsp_slice_trailing_bits(h264_stream_t* h, bs_t* b);
void read_rbsp_trailing_bits(h264_stream_t* h, bs_t* b);
void read_slice_header(h264_stream_t* h, bs_t* b);
void read_ref_pic_list_modification(h264_stream_t* h, bs_t* b);
void read_pred_weight_table(h264_stream_t* h, bs_t* b);
void read_dec_ref_pic_marking(h264_stream_t* h, bs_t* b);

int more_rbsp_trailing_data(h264_stream_t* h, bs_t* b);

int is_slice_type(int slice_type, int cmp_type);

int write_nal_unit(h264_stream_t* h, uint8_t* buf, int size);

void write_seq_parameter_set_rbsp(h264_stream_t* h, bs_t* b);
void write_scaling_list(bs_t* b, int* scalingList, int sizeOfScalingList, int useDefaultScalingMatrixFlag );
void write_vui_parameters(h264_stream_t* h, bs_t* b);
void write_hrd_parameters(h264_stream_t* h, bs_t* b);

void write_pic_parameter_set_rbsp(h264_stream_t* h, bs_t* b);

void write_sei_rbsp(h264_stream_t* h, bs_t* b);
void write_sei_message(h264_stream_t* h, bs_t* b);
void write_access_unit_delimiter_rbsp(h264_stream_t* h, bs_t* b);
void write_end_of_seq_rbsp(h264_stream_t* h, bs_t* b);
void write_end_of_stream_rbsp(h264_stream_t* h, bs_t* b);
void write_filler_data_rbsp(h264_stream_t* h, bs_t* b);

void write_slice_layer_rbsp(h264_stream_t* h, bs_t* b);
void write_rbsp_slice_trailing_bits(h264_stream_t* h, bs_t* b);
void write_rbsp_trailing_bits(h264_stream_t* h, bs_t* b);
void write_slice_header(h264_stream_t* h, bs_t* b);
void write_ref_pic_list_modification(h264_stream_t* h, bs_t* b);
void write_pred_weight_table(h264_stream_t* h, bs_t* b);
void write_dec_ref_pic_marking(h264_stream_t* h, bs_t* b);

void debug_sps(sps_t* sps);
void debug_pps(pps_t* pps);
void debug_slice_header(slice_header_t* sh);
void debug_nal(h264_stream_t* h, nal_t* nal);

void debug_bytes(uint8_t* buf, int len);

void read_sei_payload( h264_stream_t* h, bs_t* b, int payloadType, int payloadSize);
void write_sei_payload( h264_stream_t* h, bs_t* b, int payloadType, int payloadSize);

//NAL ref idc codes
#define NAL_REF_IDC_PRIORITY_HIGHEST    3
#define NAL_REF_IDC_PRIORITY_HIGH       2
#define NAL_REF_IDC_PRIORITY_LOW        1
#define NAL_REF_IDC_PRIORITY_DISPOSABLE 0

//Table 7-1 NAL unit type codes
#define NAL_UNIT_TYPE_UNSPECIFIED                    0    // Unspecified
#define NAL_UNIT_TYPE_CODED_SLICE_NON_IDR            1    // Coded slice of a non-IDR picture
#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A   2    // Coded slice data partition A
#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B   3    // Coded slice data partition B
#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C   4    // Coded slice data partition C
#define NAL_UNIT_TYPE_CODED_SLICE_IDR                5    // Coded slice of an IDR picture
#define NAL_UNIT_TYPE_SEI                            6    // Supplemental enhancement information (SEI)
#define NAL_UNIT_TYPE_SPS                            7    // Sequence parameter set
#define NAL_UNIT_TYPE_PPS                            8    // Picture parameter set
#define NAL_UNIT_TYPE_AUD                            9    // Access unit delimiter
#define NAL_UNIT_TYPE_END_OF_SEQUENCE               10    // End of sequence
#define NAL_UNIT_TYPE_END_OF_STREAM                 11    // End of stream
#define NAL_UNIT_TYPE_FILLER                        12    // Filler data
#define NAL_UNIT_TYPE_SPS_EXT                       13    // Sequence parameter set extension
                                             // 14..18    // Reserved
#define NAL_UNIT_TYPE_CODED_SLICE_AUX               19    // Coded slice of an auxiliary coded picture without partitioning
                                             // 20..23    // Reserved
                                             // 24..31    // Unspecified

 

//7.4.3 Table 7-6. Name association to slice_type
#define SH_SLICE_TYPE_P        0        // P (P slice)
#define SH_SLICE_TYPE_B        1        // B (B slice)
#define SH_SLICE_TYPE_I        2        // I (I slice)
#define SH_SLICE_TYPE_SP       3        // SP (SP slice)
#define SH_SLICE_TYPE_SI       4        // SI (SI slice)
//as per footnote to Table 7-6, the *_ONLY slice types indicate that all other slices in that picture are of the same type
#define SH_SLICE_TYPE_P_ONLY    5        // P (P slice)
#define SH_SLICE_TYPE_B_ONLY    6        // B (B slice)
#define SH_SLICE_TYPE_I_ONLY    7        // I (I slice)
#define SH_SLICE_TYPE_SP_ONLY   8        // SP (SP slice)
#define SH_SLICE_TYPE_SI_ONLY   9        // SI (SI slice)

//Appendix E. Table E-1  Meaning of sample aspect ratio indicator
#define SAR_Unspecified  0           // Unspecified
#define SAR_1_1        1             //  1:1
#define SAR_12_11      2             // 12:11
#define SAR_10_11      3             // 10:11
#define SAR_16_11      4             // 16:11
#define SAR_40_33      5             // 40:33
#define SAR_24_11      6             // 24:11
#define SAR_20_11      7             // 20:11
#define SAR_32_11      8             // 32:11
#define SAR_80_33      9             // 80:33
#define SAR_18_11     10             // 18:11
#define SAR_15_11     11             // 15:11
#define SAR_64_33     12             // 64:33
#define SAR_160_99    13             // 160:99
                                     // 14..254           Reserved
#define SAR_Extended      255        // Extended_SAR

//7.4.3.1 Table 7-7 modification_of_pic_nums_idc operations for modification of reference picture lists
#define RPLR_IDC_ABS_DIFF_ADD       0
#define RPLR_IDC_ABS_DIFF_SUBTRACT  1
#define RPLR_IDC_LONG_TERM          2
#define RPLR_IDC_END                3

//7.4.3.3 Table 7-9 Memory management control operation (memory_management_control_operation) values
#define MMCO_END                     0
#define MMCO_SHORT_TERM_UNUSED       1
#define MMCO_LONG_TERM_UNUSED        2
#define MMCO_SHORT_TERM_TO_LONG_TERM 3
#define MMCO_LONG_TERM_MAX_INDEX     4
#define MMCO_ALL_UNUSED              5
#define MMCO_CURRENT_TO_LONG_TERM    6

//7.4.2.4 Table 7-5 Meaning of primary_pic_type
#define AUD_PRIMARY_PIC_TYPE_I       0                // I
#define AUD_PRIMARY_PIC_TYPE_IP      1                // I, P
#define AUD_PRIMARY_PIC_TYPE_IPB     2                // I, P, B
#define AUD_PRIMARY_PIC_TYPE_SI      3                // SI
#define AUD_PRIMARY_PIC_TYPE_SISP    4                // SI, SP
#define AUD_PRIMARY_PIC_TYPE_ISI     5                // I, SI
#define AUD_PRIMARY_PIC_TYPE_ISIPSP  6                // I, SI, P, SP
#define AUD_PRIMARY_PIC_TYPE_ISIPSPB 7                // I, SI, P, SP, B

#define H264_PROFILE_BASELINE  66
#define H264_PROFILE_MAIN      77
#define H264_PROFILE_EXTENDED  88
#define H264_PROFILE_HIGH     100

// file handle for debug output
extern FILE* h264_dbgfile;

#endif
