cmake_minimum_required(VERSION 3.19)
set(pname jsoncreator)

# Source files
set(SOURCE_FILES
    filemapper.cpp
    "flags.cpp"
    formatting.cpp
    jsoncreator.cpp
    stdafx.cpp
    timemeasurer.cpp
    "formattingthread.cpp"
)
set(HEADER_FILES
    filemapper.h
    "flags.h"
    formatting.h
    json_ver1.h
    parsing_templates.h
    "protocol_structures.h"
    timemeasurer.h
    "tsafequeue.h"
    "formattingthread.h"
)

# Define the executable target
add_executable(${pname} ${SOURCE_FILES} ${HEADER_FILES})

# Precompiled header
target_precompile_headers(${pname} PRIVATE stdafx.h)
