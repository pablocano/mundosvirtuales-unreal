cmake_minimum_required(VERSION 2.8) 

PROJECT (PlantCore)
set( CORE_SRC
    Src/Flat.cpp
    Src/Machine.cpp
    Src/MachinePart.cpp
)

set( CORE_INCLUDE
    Src/Flat.h
    Src/Machine.h
    Src/MachinePart.h
)

set( UTILS_SRC
	Src/Utils/network/TcpComm.cpp
	Src/Utils/network/UdpComm.cpp
	Src/Utils/network/TcpConnection.cpp
	Src/Utils/Security/Cryptography.cpp
	Src/Utils/SystemCall.cpp
)

set ( UTILS_INCLUDE
	Src/Utils/network/UdpComm.h
	Src/Utils/network/TcpComm.h
	Src/Utils/network/TcpConnection.h
	Src/Utils/Security/Cryptography.h
	Src/Utils/Serialization/json.hpp
	Src/Utils/SystemCall.h
	Src/Utils/util.h
)

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
else("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
endif("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library( utilsCore ${UTILS_SRC} ${UTILS_INCLUDE})
if(WIN32)
	target_link_libraries( utilsCore wsock32 ws2_32)
endif()

add_library( plantCorelib ${CORE_SRC} ${CORE_INCLUDE})
target_link_libraries( plantCorelib utilsCore )

add_executable ( Test_Flat Src/Test/test.cpp ${CORE_INCLUDE} ${UTILS_INCLUDE})
target_link_libraries( Test_Flat plantCorelib utilsCore)
