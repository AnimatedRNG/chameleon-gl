cmake_minimum_required(VERSION 3.7.1 FATAL_ERROR)

include("modules/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.27.tar.gz"
    SHA1 "0dd355cc4599ba3a5dc82471c147f4f2e2c1f276"
)

set(CMAKE_BUILD_TYPE "Debug")
project(ChameleonGL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

hunter_add_package(glm)
hunter_add_package(SDL2)
hunter_add_package(glew)
hunter_add_package(Catch)

find_package(Threads REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(Catch CONFIG REQUIRED)

# We still need to link with SDL2_image without hunter
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules )
find_package(SDL2_image REQUIRED)

set(CMAKE_CXX_FLAGS "-O3")

set(DEFINITIONS "")
add_definitions(${DEFINITIONS})

enable_testing(true)
include_directories("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(${PROJECT_NAME} 
	src/main.cpp)

target_link_libraries(${PROJECT_NAME} 
	glm glew::glew SDL2::SDL2-static Catch::Catch
        ${SDL2_IMAGE_LIBRARY})
