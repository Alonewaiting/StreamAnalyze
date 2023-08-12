#pragma once
enum class STREAM_TYPE {
    STREAM_NONE = 0,
    STREAM_H264,
    STREAM_H265
};
enum class SLICE_TYPE {
    SLICE_N = 0,
    SLICE_I,
    SLICE_B,
    SLICE_P
};
