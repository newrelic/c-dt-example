# C Distributed Tracing Example

This repository contains two example application, used to 
illustrate instrumenting an application with the New Relic 
C SDK. The two applications are a client app written in C 
and a server app written in C++. The client will send an 
HTTP get request to the server application with a distributed
trace payload. Either of these application can be replaced with
an application written in an other New Relic supported language
and instrumented with a New Relic agent. 

## The server application

### Building and running the server

The server will require the following environment variables and there are no defaults.

|       Var Name       | Description |
| -------------------- | ----------- |
| SERVER_NEW_RELIC_LICENSE_KEY | The New Relic License Key. | 
| SERVER_NEW_RELIC_HOST | New Relic collector host. If NULL will be set to the default of collector.newrelic.com |
| SERVER_AGENT_APP_NAME | The New Relic app name that will appear in the UI. |

Run the following commands to build the server application.

1. Clone and compile the C SDK, there are more instructions [here](https://github.com/newrelic/c-sdk).
    1. Use the `make dynamic` command to create the `libnewrelic.so`
1. Copy `libnewrelic.so` to the server/ directory.
1. Copy `libnewrelic.h` to this projects root directory.
1. `cmake .`
1. `make`
1. `./server`

## The client application

### Building and running the cerver

The client will require the following environment variables and there are no defaults.

|       Var Name       | Description |
| -------------------- | ----------- |
| CLIENT_NEW_RELIC_LICENSE_KEY | The New Relic License Key. | 
| CLIENT_NEW_RELIC_HOST | New Relic collector host. If NULL will be set to the default of collector.newrelic.com |
| CLIENT_NEW_RELIC_APP_NAME | The New Relic app name that will appear in the UI. |

Run the following commands to build the client application.

1. Clone and compile the C SDK, there are more instructions [here](https://github.com/newrelic/c-sdk).
1. Copy `libnewrelic.h` and `libnewrelic.a` to this projects root directory.
1. `make`
1. `./client.out`
