cmake_minimum_required(VERSION 3.20)
project(GISAID_Fasta_Extractor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(src)

include(FetchContent)

FetchContent_Declare(argparse GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG 40a3888f151256bba95c45860af381b2b73752e8) # v2.6
FetchContent_MakeAvailable(argparse)

target_link_libraries(${PROJECT_NAME} PRIVATE argparse::argparse)
