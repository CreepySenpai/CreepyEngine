cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CREEPYENGINE "D:/WorkSpace/GameEngineProjects/CreepyEngine_Cherno/Engine")

project(CreepySandbox VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} src/Application.cpp)

file(GLOB COREAPP ${CMAKE_SOURCE_DIR}/src/Core/*.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${COREAPP})

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

message(STATUS ${CREEPYENGINE}/include)