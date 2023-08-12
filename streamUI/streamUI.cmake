if(MSVC)
set(IMGUI_SRC_FILS
	${IMAGUI_PATH}/imconfig.h
	${IMAGUI_PATH}/imgui.cpp
	${IMAGUI_PATH}/imgui.h
	${IMAGUI_PATH}/imgui_demo.cpp
	${IMAGUI_PATH}/imgui_draw.cpp
	${IMAGUI_PATH}/imgui_tables.cpp
	${IMAGUI_PATH}/imgui_widgets.cpp
	${IMAGUI_PATH}/imgui_internal.h
	${IMAGUI_PATH}/backends/imgui_impl_dx11.cpp
	${IMAGUI_PATH}/backends/imgui_impl_win32.cpp
	)
else()
set(IMGUI_SRC_FILS
	${IMAGUI_PATH}/imconfig.h
	${IMAGUI_PATH}/imgui.cpp
	${IMAGUI_PATH}/imgui.h
	${IMAGUI_PATH}/imgui_demo.cpp
	${IMAGUI_PATH}/imgui_draw.cpp
	${IMAGUI_PATH}/imgui_tables.cpp
	${IMAGUI_PATH}/imgui_widgets.cpp
	${IMAGUI_PATH}/imgui_internal.h
	)
endif()
include_directories(${IMGUI_PATH})
include_directories(${IMGUI_PATH}/backends)

set(STREAM_UI_FILE_SRC)
file(GLOB_RECURSE STREAM_UI_FILE_SRC ${STREAM_UI_PATH}/*.c ${STREAM_UI_PATH}/*.cc ${STREAM_UI_PATH}/*.cpp ${STREAM_UI_PATH}/*.hpp ${STREAM_UI_PATH}/*.ui)
set(QT_UI_FILE)
file(GLOB_RECURSE QT_UI_FILE ${STREAM_UI_PATH}/*.ui)

set(STREAM_UI_FILE_SRC
    ${STREAM_UI_FILE_SRC}

)