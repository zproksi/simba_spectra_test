cmake_minimum_required(VERSION 3.19)
set(pname jsoncreator)
project(${pname})

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "Configs" FORCE)

# Set the build type to Debug if not explicitly specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Initial COMPILER_FLAGS:" ${CMAKE_CXX_FLAGS})

#CMAKE_CXX_FLAGS_DEBUG
#CMAKE_CXX_FLAGS_RELEASE
#CMAKE_CXX_FLAGS_RELWITHDEBINFO
#CMAKE_CXX_FLAGS_MINSIZEREL

set(CMAKE_CXX_FLAGS "")
message(STATUS "CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_CXX_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
set(CMAKE_EXE_LINKER_FLAGS "")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC (GNU Compiler)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D_DEBUG -g -D_CONSOLE -Wno-unknown-pragmas -Wno-error=interference-size -Wno-error=int-in-bool-context -Wno-error=char-subscripts -Wall -Werror -fno-rtti -pthread")
        set(CMAKE_EXE_LINKER_FLAGS "-g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -DNDEBUG    -D_CONSOLE -Wno-unknown-pragmas -Wno-error=interference-size -Wno-error=int-in-bool-context -Wno-error=char-subscripts -Wall -Werror -fno-rtti -pthread")
        set(CMAKE_EXE_LINKER_FLAGS "-O3 -flto")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -DNDEBUG -g -D_CONSOLE -Wno-unknown-pragmas -Wno-error=interference-size -Wno-error=int-in-bool-context -Wno-error=char-subscripts -Wall -Werror -fno-rtti -pthread")
        set(CMAKE_EXE_LINKER_FLAGS "-g -O2 -flto")
    else()
        message(FATAL_ERROR "Unsupported compilation mode")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")  #Clang or AppleClang or anything with Clang
    # clang (MAC Compiler)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D_DEBUG -g -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -Wno-unqualified-std-cast-call -fno-rtti -pthread -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "-g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -DNDEBUG    -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -Wno-unqualified-std-cast-call -fno-rtti -pthread -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "-O3 -flto")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -DNDEBUG -g -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -Wno-unqualified-std-cast-call -fno-rtti -pthread -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "-g -O2 -flto")
    else()
        message(FATAL_ERROR "Unsupported compilation mode")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC (Microsoft Visual C++)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /D_CONSOLE /DDEBUG /D_DEBUG      /Zi /EHsc /W4 /WX /GR- /MDd")
        set(CMAKE_EXE_LINKER_FLAGS "/DEBUG:FULL /INCREMENTAL")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /D_CONSOLE         /DNDEBUG /Ob1     /EHsc /W4 /WX /GR- /MD")
        set(CMAKE_EXE_LINKER_FLAGS "            /OPT:REF /OPT:ICF /INCREMENTAL:NO")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /D_CONSOLE         /DNDEBUG /Ob1 /Zi /EHsc /W4 /WX /GR- /MD")
        set(CMAKE_EXE_LINKER_FLAGS "/DEBUG:FULL /OPT:REF /OPT:ICF /INCREMENTAL:NO")
    else()
        message(FATAL_ERROR "Unsupported compilation mode")
    endif()
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

message(STATUS "CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})

add_subdirectory(src${pname})

# Console commands for building on Windows with Visual Studio
# !!!!!!!!!!!!!
#    Use these commands in the Visual Studio Command Prompt
# !!!!!!!!!!!!!
# Navigate to the directory containing CMakeLists.txt
#   create folder 'build' and enter it
#  For visual studio 2022 option for cmake is "Visual Studio 17 2022"
#       for 32 bits compilation use:
#  cmake -G "Visual Studio 17 2022" -A Win32 ..
#       for 64 bits compilation use:
#  cmake -G "Visual Studio 17 2022" -A x64 ..
#       and execute to build
#    Release:
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build . --config Release
#    Debug:
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build . --config Debug
#

# Console commands for building on Linux with GCC
# Use these commands in the terminal
# Navigate to the directory containing CMakeLists.txt
# mkdir build
# cd build
#   to build Release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build . --config Release
#   to build Debug
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build . --config Debug
#
# as a result folder should contain version of the application
# to generate 32 bits version of linux application add -m32 flag
# to CMAKE_CXX_FLAGS above in "GNU|Clang" section
