#
# CMakeLists for the New Relic C Distributed Trace Example
#

# The CMakeLists presumes that libnewrelic.so is in the this directory.

set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required (VERSION 3.0)
find_package (Threads)

link_directories (../.)


add_executable (server server.cpp)
target_link_libraries (server libnewrelic.so ${CMAKE_THREAD_LIBS_INIT}
        pcre dl)
