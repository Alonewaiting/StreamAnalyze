
set(STREAM_ANALYZE_FILE_SRC)

#h264
set(STREAM_ANALYZE_FILE_H264)
include_directories(${STREAM_ANALYZE_PATH}/h264)
file(GLOB_RECURSE STREAM_ANALYZE_FILE_H264 ${STREAM_ANALYZE_PATH}/h264/*.c ${STREAM_ANALYZE_PATH}/h264/*.cc ${STREAM_ANALYZE_PATH}/h264/*.cpp ${STREAM_ANALYZE_PATH}/h264/*.hpp)


set(STREAM_ANALYZE_FILE_SRC 
    ${STREAM_ANALYZE_FILE_H264}
)