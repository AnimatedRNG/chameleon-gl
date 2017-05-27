cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
SET(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules )
set(EXECUTABLE_NAME sdf_renderer)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)

set(CMAKE_CXX_FLAGS "-O3")

set(DEFINITIONS "")
add_definitions(${DEFINITIONS})

set(CMAKE_CXX_FLAGS "")

set(INCLUDE_DIRS 
	"${PROJECT_SOURCE_DIR}/include"
        ${GLM_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIR})

set(LIBS ${SDL2_LIBRARY})

set(SRC_FILES "")
        
include_directories(${INCLUDE_DIRS})

add_executable(${EXECUTABLE_NAME} 
	${SRC_FILES} 
	src/simple_renderer.cpp)

target_link_libraries(${EXECUTABLE_NAME} 
	${LIBS})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
