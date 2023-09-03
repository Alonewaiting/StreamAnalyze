#pragma once
enum class STREAM_TYPE {
    STREAM_NONE = 0,
    STREAM_H264,
    STREAM_H265
};
enum class SLICE_TYPE {
    SLICE_N = -1,
    SLICE_B = 0,
    SLICE_P = 1,
    SLICE_I = 2
};
