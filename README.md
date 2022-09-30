# DaqGrimoire
DaqGrimoire is a header-only library providing utilities for reading data from Daqromancy sources. There are two types of Daqromancy sources, files and network tcp connections to a running instance of Daqromancy.
Daqromancy files are handled by the DYFileReader class, while network connections are handled by the DYClient class.

## Installation
As DaqGrimoire is a header-only library, the only thing you need to do is drop the headers into your project. A convience header, DaqGrimoire.h, is provided and includes all DaqGrimoire content.

As a convience, DaqGrimoire has a CMakeList.txt that will allow it to be included as an interface library in your CMake project. If you wish to use DYClient, be sure to define the GRIMOIRE_ASIO_INCLUDE_PATH in your
project, otherwise the header will not be made available via DaqGrimoire.h. See the dependencies section for more information.

## Dependencies
DaqGrimoire requires C++ 20.

DYFileReader relies only upon stl containers, and as such is easily included in most projects. 

DYClient depends upon the asio networking libraries. Install standalone asio (found [here](https://github.com/chriskohlhoff/asio)) somewhere in your project. Since DaqGrimoire is header only, you just need to make sure
that when you compile your code, the path to asio.hpp is added in your include directories. If using CMake to integrate DaqGrimoire, this can be done conveniently by defining the variable GRIMOIRE_ASIO_INCLUDE_PATH 
with your asio path in your CMake project. This will let the DaqGrimoire project know that you have asio and where it is. If you choose not to use the CMake method, be sure to define the macro DG_HAS_ASIO wherever
you include DaqGrimoire.h (the `#define DG_HAS_ASIO` should come before the `#include` for DaqGrimoire.h).

DYClient also uses std::thread, and your platform's specific threading implementation should also be linked in your project (i.e. on Linux systems pass the -pthread flag to the linker).

## About DYFileReader
DYFileReader is a wrapper around a std::shared_ptr of an std::ifstream. A pointer to an ifstream is used to make the reader movable (and usable in objects like std::vector). When creating a reader,
a path to the file is requested, and optionally the user can specify the size of the file buffer in data events. A data event in Daqromancy is 24 bytes. The default size of the buffer is 200000 events, or
4.8 MB. Most of the time, the perfomance of the reader can be optimized by either shrinking the buffer or increasing it. See the code for detailed documentation on the API.

NOTE: Daqromancy files are written as binary data. DYFileReader makes no assumptions about the endian-ness of the data in the file. If your data was generated on a big-endian machine, and you attempt to read the data
on a little-endian machine, or vice-versa, the reader will fail.

## About DYClient
DYClient is a wrapper around an asio socket and utilizes the asynchronous capabilities of the asio library. A DYClient is created by passing the ip address and port number of the active Daqromancy server. If your
client was accepted by the server, DYClient will then initiate a data read cycle. All of this work occurs on a thread owned by the DYClient. The data read from the server is then stored in a thread-safe queue. 
Your program can then poll the queue for data. The queue also provides functionality for blocking, but this is currently not implemented. See the code for detailed documentation on the API.

NOTE: DYClient does not currently have a cap on how much data can be stored in it's queue. As such it is imperative that the data is pulled off the queue at a comparable rate to the data being dropped into the queue.

## Current Status
Use at your own risk! The library has had little to no testing, so bugs surely abound!