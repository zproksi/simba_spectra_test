cmake_minimum_required(VERSION 3.19)
set(pname jsoncreator)

# Source files
set(SOURCE_FILES
    filemapper.cpp
    formatting.cpp
    jsoncreator.cpp
    stdafx.cpp
    timemeasurer.cpp
)
set(HEADER_FILES
    filemapper.h
    formatting.h
    json_ver1.h
    protocol_srtuctures.h
    timemeasurer.h
)

# Define the executable target
add_executable(${pname} ${SOURCE_FILES} ${HEADER_FILES})

# Precompiled header
target_precompile_headers(${pname} PRIVATE stdafx.h)
