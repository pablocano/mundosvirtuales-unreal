cmake_minimum_required(VERSION 2.8) 

PROJECT (PlantCore)
set( NAME_SRC
    Src/Flat.cpp
    Src/Flat.h
    Src/Machine.cpp
    Src/Machine.h
    Src/MachinePart.cpp
    Src/MachinePart.h
	Src/Loader.cpp
    Src/Loader.h
)

set( TEST_SRC
	Src/Test/test_flat.cpp
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

add_library( plantCorelib ${NAME_SRC} )

message("${LIBSOCI_LIB}")

target_link_libraries( plantCorelib "${LIBSOCI_LIB}/Debug/soci_core_4_0.lib" "${LIBSOCI_LIB}/Release/soci_core_4_0.lib")

add_executable ( Test_Flat ${TEST_SRC} )

target_link_libraries( Test_Flat plantCorelib )