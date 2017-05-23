cmake_minimum_required(VERSION 2.8) 

PROJECT (PlantCore)
set( NAME_SRC
    Src/Flat.cpp
    Src/Flat.h
    Src/Machine.cpp
    Src/Machine.h
    Src/MachinePart.cpp
    Src/MachinePart.h
	Src/TestPQ.cpp
    Src/TestPQ.h
)

if (WIN32)
    set(LIBSOCI "E:/mundosvirtuales-unreal/ThirdParty/PlantCore/SOCI/Windows")
endif (WIN32)

include_directories("${LIBSOCI}/include")

include_directories("C:/PostgreSQL/pg96/include")

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
	link_directories("${LIBSOCI}/lib/x64/Release")
else("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
	link_directories("${LIBSOCI}/lib/x86/Release")
endif("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

add_library( plantCorelib STATIC ${NAME_SRC} )

target_link_libraries( plantCorelib STATIC libsoci_core libsoci_postgresql libsoci_odbc libsoci_empty )