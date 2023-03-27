cmake_minimum_required(VERSION 3.0)

project(NixSoundPlayer)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${PROJECT_DIR}/vendor/NixTools)

set(PROJECT_SOURCES
	${PROJECT_SOURCE_DIR}/QueuedSoundBuffer.cpp
	${PROJECT_SOURCE_DIR}/SoundPlayer.cpp
	${PROJECT_SOURCE_DIR}/SoundBuffer.cpp
	${PROJECT_SOURCE_DIR}/SoundDevice.cpp
	${PROJECT_SOURCE_DIR}/SoundManager.cpp
	${PROJECT_SOURCE_DIR}/SoundSource.cpp
)

include_directories(${PROJECT_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

file(COPY ${PROJECT_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_VENDOR_DIR}/libsndfile-1.2.0-win64/bin/sndfile.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_VENDOR_DIR}/OpenAL/OpenAL32.dll DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} NixTools)
target_link_libraries(${PROJECT_NAME} ${PROJECT_VENDOR_DIR}/libsndfile-1.2.0-win64/lib/sndfile.lib)
target_link_libraries(${PROJECT_NAME} ${PROJECT_VENDOR_DIR}/OpenAL/OpenAL32.lib)
target_link_libraries(${PROJECT_NAME} ${PROJECT_VENDOR_DIR}/OpenAL/common.lib)
target_link_libraries(${PROJECT_NAME} ${PROJECT_VENDOR_DIR}/OpenAL/ex-common.lib)