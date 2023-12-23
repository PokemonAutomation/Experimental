#set the minimum cmake version required
cmake_minimum_required(VERSION 3.16.0)

#set the name of the project
project(PLA-SeedFinder)

#enable c++17 or c++ 20 depending on compiler
if (MSVC)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#produce clang tidy file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_VERBOSE_MAKEFILE ON)

#Find threads library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#detect cpu arch
if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set (X86 TRUE)
else ()
    set (X86 FALSE)
endif ()

#add current directory to find tesseractPA.lib
#link_directories(${CMAKE_CURRENT_LIST_DIR})

if (X86)
    file(GLOB MAIN_SOURCES
    Source/Compiler.h
    Source/CpuFeatures.cpp
    Source/CpuFeatures.h
    Source/DynamicParallelizer.cpp
    Source/DynamicParallelizer.h
    Source/Kernels/SeedScan_Default.cpp
    Source/Kernels/SeedScan_x86_AVX2.cpp
    Source/Kernels/SeedScan_x86_AVX512.cpp
    Source/Kernels/SeedScan_x86_SSE41.cpp
    Source/Kernels/XoroShiro1_Default.h
    Source/Kernels/XoroShiro2_SSE2.h
    Source/Kernels/XoroShiro4_AVX2.h
    Source/Kernels/XoroShiro8_AVX512.h
    Source/Main.cpp
    Source/PLA-SeedFinder.cpp
    Source/PLA-SeedFinder.h
    Source/ReportCandidates.cpp
    Source/ReportCandidates.h
    Source/SeedScan.cpp
    Source/SeedScan.h
    Source/Tools.cpp
    Source/Tools.h
    )
else ()
    file(GLOB MAIN_SOURCES
        Source/DynamicParallelizer.cpp
        Source/Kernels/SeedScan_Default.cpp
        Source/Kernels/SeedScan_aarch64.cpp
        Source/Main.cpp
        Source/PLA-SeedFinder.cpp
        Source/ReportCandidates.cpp
        Source/SeedScan.cpp
        Source/Tools.cpp
    )
endif (X86)

if (WIN32)
    add_executable(PLA-SeedFinder WIN32 ${MAIN_SOURCES})
else ()
    add_executable(PLA-SeedFinder ${MAIN_SOURCES})
endif ()
set_target_properties(PLA-SeedFinder PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(PLA-SeedFinder)
target_link_libraries(PLA-SeedFinder Threads::Threads)

#add defines
target_compile_definitions(PLA-SeedFinder PRIVATE NOMINMAX)

#add include directory
target_include_directories(PLA-SeedFinder PRIVATE Source/)

#enable MP with MSVC (Build with Multiple Processes)
if (MSVC)
    add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory Assembly/)

    target_compile_options(PLA-SeedFinder PRIVATE /FAs /FaAssembly/ /MP /W4)
    target_compile_options(PLA-SeedFinder PRIVATE /wd5054)  # Deprecated enum arithemtic
    target_compile_options(PLA-SeedFinder PRIVATE /wd4505)  # unreferenced local function has been removed

    set(ARCH_FLAGS_09_Nehalem       /W4)    # Dummy parameter
    set(ARCH_FLAGS_13_Haswell       /arch:AVX2)
    set(ARCH_FLAGS_17_Skylake       /arch:AVX512)

    #   Run-time ISA dispatching
    target_compile_definitions(PLA-SeedFinder PRIVATE PA_AutoDispatch_08_Nehalem)
    target_compile_definitions(PLA-SeedFinder PRIVATE PA_AutoDispatch_13_Haswell)
    target_compile_definitions(PLA-SeedFinder PRIVATE PA_AutoDispatch_17_Skylake)
    
else()
    target_compile_options(PLA-SeedFinder PRIVATE -Wall -Wpedantic -O2)
    if (X86)
        target_compile_options(PLA-SeedFinder PRIVATE -msse4.2)

        set(ARCH_FLAGS_09_Nehalem       -march=nehalem)
        set(ARCH_FLAGS_13_Haswell       -march=haswell)
        set(ARCH_FLAGS_17_Skylake       -march=skylake-avx512)

        #   Run-time ISA dispatching
        target_compile_definitions(PLA-SeedFinder PRIVATE PA_AutoDispatch_08_Nehalem)
        target_compile_definitions(PLA-SeedFinder PRIVATE PA_AutoDispatch_13_Haswell)
        target_compile_definitions(PLA-SeedFinder PRIVATE PA_AutoDispatch_17_Skylake)
    endif (X86)
endif()


if(X86)
    #   Run-time CPU dispatching.
    if (ARCH_FLAGS_09_Nehalem)
    SET_SOURCE_FILES_PROPERTIES(
        Source/Kernels/SeedScan_x86_SSE41.cpp
        PROPERTIES COMPILE_FLAGS ${ARCH_FLAGS_09_Nehalem}
    )
    endif()
    if (ARCH_FLAGS_13_Haswell)
    SET_SOURCE_FILES_PROPERTIES(
        Source/Kernels/SeedScan_x86_AVX2.cpp
        PROPERTIES COMPILE_FLAGS ${ARCH_FLAGS_13_Haswell}
    )
    endif()
    if (ARCH_FLAGS_17_Skylake)
    SET_SOURCE_FILES_PROPERTIES(
        Source/Kernels/SeedScan_x86_AVX512.cpp
        PROPERTIES COMPILE_FLAGS ${ARCH_FLAGS_17_Skylake}
    )
    endif()
endif(X86)

if (WIN32)
    #copy needed dlls
    #file(COPY *.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(GLOB MY_DLLS
    "*.dll"
    )
    file(COPY ${MY_DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)
