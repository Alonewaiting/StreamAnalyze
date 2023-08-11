cmake_minimum_required(VERSION 3.3)
project(streamAnalyze)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#预定义宏
add_definitions(-D _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING)
add_definitions(-D __windows__)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)


set(COMMON_PATH ${CMAKE_SOURCE_DIR}/common)
set(FORMAT_PATH ${CMAKE_SOURCE_DIR}/format)
set(PLAYER_PATH ${CMAKE_SOURCE_DIR}/player)
set(STREAM__ANALYZE_PATH ${CMAKE_SOURCE_DIR}/streamAnalyze)
set(STREAM_UI_PATH ${CMAKE_SOURCE_DIR}/streamUI)
set(UTILS_PATH ${CMAKE_SOURCE_DIR}/utils)
set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/imgui)

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
