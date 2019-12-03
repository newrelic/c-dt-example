# C Distributed Tracing Example

**This is an unsupported example. This project is provided AS-IS WITHOUT
WARRANTY OR SUPPORT.**

This repository contains two example application, used to 
illustrate instrumenting an application with the New Relic 
C SDK. The two applications are a client app written in C 
and a server app written in C++. The client will send an 
HTTP get request to the server application with a distributed
trace payload. Either of these application can be replaced with
an application written in an other New Relic supported language
and instrumented with a New Relic agent. 

## Requirements

* gcc 4.9 or greater 
* CMake 3.0 or greater
* g++ 7.3.1 or greater
* curl 7.29 or greater

## The server application

### Building and running the server

The server will require the following environment variables.

|       Var Name       | Description | Required |
| -------------------- | ----------- | ------------- |
| NEW_RELIC_LICENSE_KEY | The New Relic License Key. | Required |
| NEW_RELIC_HOST | New Relic collector host. If NULL will be set to the default of collector.newrelic.com | Not required |
| SERVER_NEW_RELIC_APP_NAME | The New Relic app name that will appear in the UI. | Required |

Run the following commands to build the server application.

1. Clone and compile the C SDK, there are more instructions [here](https://github.com/newrelic/c-sdk).
    1. Use the `make dynamic` command to create the `libnewrelic.so`
1. Copy `libnewrelic.so` to the server/ directory.
    1. The `CMakeLists.txt` is setup for linux. To compile the server on a mac the following line will need to be changed 
    from: `target_link_libraries (server libnewrelic.so ${CMAKE_THREAD_LIBS_INIT}`
    to: `target_link_libraries (server ${CMAKE_SOURCE_DIR}/libnewrelic.so ${CMAKE_THREAD_LIBS_INIT}`
1. Copy `libnewrelic.h` to this projects root directory.
1. `cmake .`
1. `make`
1. Follow the directions in the [C SDK repo](https://github.com/newrelic/c-sdk/blob/master/GUIDE.md#getting-started) to start the New Relic daemon.
1. `./server`

## The client application

### Building and running the client

The client will require the following environment variables and there are no defaults.

|       Var Name       | Description | Required |
| -------------------- | ----------- | -------- | 
| NEW_RELIC_LICENSE_KEY | The New Relic License Key. | Required | 
| NEW_RELIC_HOST | New Relic collector host. If NULL will be set to the default of collector.newrelic.com | Not Required |
| CLIENT_NEW_RELIC_APP_NAME | The New Relic app name that will appear in the UI. | Required |

Run the following commands to build the client application.

1. Clone and compile the C SDK, there are more instructions [here](https://github.com/newrelic/c-sdk).
1. Copy `libnewrelic.h` and `libnewrelic.a` to this projects root directory.
1. `make`
1. Follow the directions in the [C SDK repo](https://github.com/newrelic/c-sdk/blob/master/GUIDE.md#getting-started) to start the New Relic daemon.
1. `./client.out`
