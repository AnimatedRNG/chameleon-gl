cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
SET(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules )
set(EXECUTABLE_NAME sdf_renderer)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(GLM REQUIRED)

set(CMAKE_CXX_FLAGS "-O3")

set(DEFINITIONS "")
add_definitions(${DEFINITIONS})

set(CMAKE_CXX_FLAGS "")

set(INCLUDE_DIRS 
	"${PROJECT_SOURCE_DIR}/include"
        ${GLM_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR})

set(LIBS
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY})

set(SRC_FILES "")
        
include_directories(${INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(${EXECUTABLE_NAME} 
	${SRC_FILES} 
	src/main.cpp)

target_link_libraries(${EXECUTABLE_NAME} 
	${LIBS})
