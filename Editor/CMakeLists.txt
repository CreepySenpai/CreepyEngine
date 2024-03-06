cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CREEPYENGINE "D:/WorkSpace/GameEngineProjects/CreepyEngine_Cherno/Engine")

project(CreepyEditor VERSION 0.1.0 LANGUAGES CXX)

file(GLOB COREEDITOR ${CMAKE_SOURCE_DIR}/src/Editor/*.cpp)

add_executable(CreepyEditor src/CreepyEditor.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${COREEDITOR})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CREEPYENGINE}/include
)

target_link_directories(${PROJECT_NAME} PUBLIC 
    ${CREEPYENGINE}/build
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    CreepyEngine
)