cmake_minimum_required(VERSION 2.8) 

PROJECT (PlantCore)
set( NAME_SRC
    Src/Flat.cpp
    Src/Flat.h
    Src/Machine.cpp
    Src/Machine.h
    Src/MachinePart.cpp
    Src/MachinePart.h
)

if (WIN32)
    set(LIBPQXX "Postgres/Windows")
endif (WIN32)

include_directories("${LIBPQXX}/include")

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x64)
	# link_directories("${LIBPQXX}/lib/x64")
else("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/x86)
	# link_directories("${LIBPQXX}/lib/x86")
endif("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

# set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")

add_library( plantCorelib ${NAME_SRC} )

# target_link_libraries( plantCorelib libpqxx )