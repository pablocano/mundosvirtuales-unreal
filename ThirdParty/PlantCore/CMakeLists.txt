cmake_minimum_required(VERSION 2.8) 

PROJECT (PlantCore)
set( CORE_SRC
    Src/Flat.cpp
    Src/Machine.cpp
    Src/MachinePart.cpp
	Src/ClientPlant.cpp
)

set( CORE_INCLUDE
    Src/Flat.h
    Src/Machine.h
    Src/MachinePart.h
	Src/ClientPlant.h
)

set( SERVER_SRC
	Src/Server/main.cpp
	Src/Server/Serverdata.cpp
	Src/Server/Serverdata.h
)

set( UTILS_SRC
	Src/Utils/Loader.cpp
	Src/Utils/network/TcpComm.cpp
	Src/Utils/network/UdpComm.cpp
	Src/Utils/SystemCall.cpp
)

set ( UTILS_INCLUDE
	Src/Utils/Loader.h
	Src/Utils/network/UdpComm.h
	Src/Utils/network/TcpComm.h
	Src/Utils/SystemCall.h
	Src/Utils/util.h
)

if (WIN32)
    set(LIBSOCI "E:/mundosvirtuales-unreal/ThirdParty/PlantCore/SOCI/Windows")
endif (WIN32)

include_directories("${LIBSOCI}/include")

include_directories("C:/PostgreSQL/pg96/include")

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
	set(LIBSOCI_LIB "${LIBSOCI}/lib/x64")
else("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
	set(LIBSOCI_LIB "${LIBSOCI}/lib/x86")
endif("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

add_library( utilsCore ${UTILS_SRC} ${UTILS_INCLUDE})
if(WIN32)
	target_link_libraries( utilsCore "${LIBSOCI_LIB}/Debug/soci_core_4_0.lib" "${LIBSOCI_LIB}/Release/soci_core_4_0.lib" wsock32 ws2_32)
else()
	target_link_libraries( utilsCore "${LIBSOCI_LIB}/Debug/soci_core_4_0.lib" "${LIBSOCI_LIB}/Release/soci_core_4_0.lib" )
endif()

add_library( plantCorelib ${CORE_SRC} ${CORE_INCLUDE})
target_link_libraries( plantCorelib utilsCore)

add_executable ( Server ${SERVER_SRC} ${UTILS_INCLUDE} ${CORE_INCLUDE})
target_link_libraries( Server plantCorelib utilsCore )

add_executable ( Test_Flat Src/Test/test_flat.cpp ${UTILS_INCLUDE} ${CORE_INCLUDE})
target_link_libraries( Test_Flat plantCorelib utilsCore )

add_executable ( Test_Server Src/Test/test_server.cpp ${UTILS_INCLUDE} ${CORE_INCLUDE})
target_link_libraries( Test_Server plantCorelib utilsCore )