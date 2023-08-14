cmake_minimum_required(VERSION 3.3)
project(streamAnalyze)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_DIR "D:/Qt/6.4.3/msvc2019_64")
#预定义宏
add_definitions(-D _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)
add_definitions(-D __windows__)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)

option(OPENAL_DEMO "open demo" ON)

set(COMMON_PATH ${CMAKE_SOURCE_DIR}/common)
set(FORMAT_PATH ${CMAKE_SOURCE_DIR}/format)
set(PLAYER_PATH ${CMAKE_SOURCE_DIR}/player)
set(STREAM_ANALYZE_PATH ${CMAKE_SOURCE_DIR}/streamAnalyze)
set(STREAM_UI_PATH ${CMAKE_SOURCE_DIR}/streamUI)
set(UTILS_PATH ${CMAKE_SOURCE_DIR}/utils)
set(IMAGUI_PATH ${CMAKE_SOURCE_DIR}/imgui)
set(THIRD_PART_PATH ${CMAKE_SOURCE_DIR}/3dpart)

list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.4.3/msvc2019_64/lib/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/")
#src and lib
set(PROJECT_LIBS)
set(PROJECT_SRC_FILES)


macro(add_src_file)
    foreach(_source IN ITEMS ${ARGN})
        set(PROJECT_SRC_FILES ${PROJECT_SRC_FILES} ${_source})
    endforeach()
endmacro(mymacro)

macro(add_lib_file)
foreach(_source IN ITEMS ${ARGN})
        message("${_source}")
        set(PROJECT_LIBS ${PROJECT_LIBS} ${_source})
    endforeach()
endmacro()



function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)
 
function(my_add_executable)
    foreach(_source IN ITEMS ${ARGN})
        assign_source_group(${_source})
    endforeach()
    add_executable(${ARGV})
endfunction(my_add_executable)

#COMMON
include(${COMMON_PATH}/common.cmake)
include_directories(${COMMON_PATH})
add_src_file(${COMMON_FILE_SRC})

#stream analyze
include(${STREAM_ANALYZE_PATH}/streamAnalyze.cmake)
include_directories(${STREAM_ANALYZE_PATH})
add_src_file(${STREAM_ANALYZE_FILE_SRC})



if(${OPENAL_DEMO})
	set(NALPARSER_TEST 
		${PROJECT_SRC_FILES}
		${CMAKE_SOURCE_DIR}/example/test_nal.cpp
	)
	my_add_executable(test_nal ${NALPARSER_TEST})
	
	set(H264ANALYZE_TEST
		${PROJECT_SRC_FILES}
		${CMAKE_SOURCE_DIR}/example/test_h264_analyze.cpp
	)
	my_add_executable(test_h264_analyze ${H264ANALYZE_TEST})

	set(H265ANALYZE_TEST
		${PROJECT_SRC_FILES}
		${CMAKE_SOURCE_DIR}/example/test_h265_analyze.cpp
	)
	my_add_executable(test_h265_analyze ${H265ANALYZE_TEST})
	
endif()

#QT
find_package(Qt6 COMPONENTS Core Widgets Gui Qml Quick Multimedia REQUIRED)
#moc编译在这里进行，请把含有Q_OBJECT宏定义的头文件放到这里来

QT6_WRAP_CPP(MOC_Files
      ${STREAM_UI_PATH}/StreamAnalyzeWidget.h)

include(${STREAM_UI_PATH}/streamUI.cmake)
include_directories(${STREAM_UI_PATH})
set(STRAM_ANALYZE_APP
	${STREAM_UI_FILE_SRC}
	${PROJECT_SRC_FILES}
	${MOC_Files}
)

my_add_executable(stream_analyze ${STRAM_ANALYZE_APP})
target_link_libraries(stream_analyze PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::Multimedia Qt6::Widgets)
qt_import_qml_plugins(${CMAKE_PROJECT_NAME})



