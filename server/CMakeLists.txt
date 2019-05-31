#
# CMakeLists for the New Relic C Distributed Trace Example
#

# The CMakeLists presumes that libnewrelic.so is in the this directory.

cmake_minimum_required (VERSION 3.0)
find_package (Threads)

add_definitions(-DUSE_NEWRELIC)

link_directories (../.)

add_executable (server server.cpp)
target_link_libraries (server libnewrelic.so ${CMAKE_THREAD_LIBS_INIT}
        pcre dl)
