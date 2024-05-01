cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CREEPYENGINE "D:/WorkSpace/GameEngineProjects/CreepyEngine_Cherno/Engine")

project(CreepyEditor VERSION 0.2.0 LANGUAGES CXX)

file(GLOB COREEDITOR ${CMAKE_SOURCE_DIR}/src/Editor/*.cpp)
file(GLOB COREPANEL ${CMAKE_SOURCE_DIR}/src/Panel/*.cpp)

add_executable(CreepyEditor src/CreepyEditor.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${COREEDITOR} ${COREPANEL})

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


set(cppcheck_exe "D:/Enviroments/cppcheck/build/bin/cppcheck.exe")

message(STATUS "Cpp Check Dir: ${cppcheck_exe}")

set(CMAKE_CXX_CPPCHECK "${cppcheck_exe}" --std=c++23
            "--enable=all"
            "--output-file=report.txt"
            "--inconclusive"
            "--inline-suppr"
            "--quiet"
            "--verbose"
            "--suppress=unmatchedSuppression"
            "--suppress=unusedFunction"
            "--suppress=containerOutOfBounds"
            "--template='{file}:{line}: warning: {id} ({severity}): {message}'")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_CPPCHECK "${CMAKE_CXX_CPPCHECK}")