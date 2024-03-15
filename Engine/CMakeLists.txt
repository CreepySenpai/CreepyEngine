cmake_minimum_required(VERSION 3.20.0)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

project(CreepyEngine VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB COREENGINE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Core/*.cpp)
file(GLOB EVENTENGINE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Events/*.cpp)
file(GLOB RENDERERENGINE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Renderer/*.cpp)
file(GLOB PLATFORMWINDOWS ${CMAKE_SOURCE_DIR}/src/Platform/Windows/*.cpp)
file(GLOB GLAD ${CMAKE_SOURCE_DIR}/src/glad/*.c)
file(GLOB IMGUI ${CMAKE_SOURCE_DIR}/src/imgui/*.cpp)
file(GLOB DEBUG ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Debug/*.cpp)
file(GLOB OPENGL ${CMAKE_SOURCE_DIR}/src/Platform/OpenGL/*.cpp)
file(GLOB CONTROLLER ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Controller/*.cpp)
file(GLOB SCENE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Scene/*.cpp)


add_library(${PROJECT_NAME} SHARED ${COREENGINE} ${EVENTENGINE} ${RENDERERENGINE} ${PLATFORMWINDOWS} ${GLAD} ${IMGUI} ${DEBUG} ${OPENGL} ${CONTROLLER} ${SCENE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw3dll opengl32 gdi32 yaml-cpp.dll)

add_compile_definitions(GLFW_INCLUDE_NONE)
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

add_compile_definitions(YAML_CPP_STATIC_DEFINE)