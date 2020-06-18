set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required(VERSION 3.12)
project(mp3_tagger)
set(CMAKE_CXX_STANDARD 11)

set(Taglib_DIR = ../../../dependencies/taglib)
find_package(Boost 1.73 COMPONENTS program_options filesystem REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS -fPIC)

set(SOURCE_FILES src/main.cpp src/Parser.cpp include/Parser.h src/Tagger.cpp include/Tagger.h)
add_executable(mp3_tagger ${SOURCE_FILES})
target_link_libraries(mp3_tagger ${Boost_LIBRARIES} -L../../../dependencies/taglib/1.11.1/lib -L../../../dependencies/zlib/lib -ltag -lz)

