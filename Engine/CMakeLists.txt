cmake_minimum_required(VERSION 3.30.0)

# Project Setting
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 26)

set(ENABLE_RENDERER_OPENGL OFF)
set(ENABLE_RENDERER_VULKAN OFF)
set(ENABLE_TRACY_PROFILER OFF)
set(ENABLE_CPPCHECK ON)

project(CreepyEngine VERSION 0.3.0 LANGUAGES C CXX)


# Libraries Sources
file(GLOB COREENGINE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Core/*.cpp)
file(GLOB RENDERERENGINE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Renderer/*.cpp)
file(GLOB PLATFORMWINDOWS ${CMAKE_SOURCE_DIR}/src/Platform/Windows/*.cpp)
file(GLOB GLAD ${CMAKE_SOURCE_DIR}/src/glad/*.c)
file(GLOB IMGUI ${CMAKE_SOURCE_DIR}/src/imgui/*.cpp)
file(GLOB DEBUG ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Debug/*.cpp)
file(GLOB OPENGL ${CMAKE_SOURCE_DIR}/src/Platform/OpenGL/*.cpp)
file(GLOB VULKAN ${CMAKE_SOURCE_DIR}/src/Platform/Vulkan/*.cpp)
file(GLOB SCENE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Scene/*.cpp)
file(GLOB MATH ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Math/*.cpp)
file(GLOB EXTENDED_YAMLCPP ${CMAKE_SOURCE_DIR}/src/Extended/yaml-cpp/*.cpp)
file(GLOB CORAL ${CMAKE_SOURCE_DIR}/src/Coral/*.cpp)
file(GLOB SCRIPTING ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Scripting/*.cpp)
file(GLOB UTILS ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Utils/*.cpp)
file(GLOB PROJECTCORE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Project/*.cpp)
file(GLOB PHYSIC2DCORE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Physics/2D/*.cpp)
file(GLOB VULKANBOOSTSTRAP ${CMAKE_SOURCE_DIR}/src/VulkanBootstrap/*.cpp)
file(GLOB VULKANMEMORYALLOCATOR ${CMAKE_SOURCE_DIR}/src/VulkanMemoryAllocator/*.cpp)
# file(GLOB PHYSIC3DCORE ${CMAKE_SOURCE_DIR}/src/CreepyEngine/Physics/3D/*.cpp)
file(GLOB TRACYPROFILER ${CMAKE_SOURCE_DIR}/src/tracy/tracycore/*.cpp)

# add_library(${PROJECT_NAME} SHARED ${COREENGINE} ${EVENTENGINE} ${RENDERERENGINE} ${PLATFORMWINDOWS} ${GLAD} ${IMGUI} ${DEBUG} ${OPENGL} ${VULKAN} ${SCENE} ${MATH} ${EXTENDED_YAMLCPP} ${CORAL} ${SCRIPTING} ${UTILS} ${PROJECTCORE} ${PHYSIC2DCORE} ${PHYSIC3DCORE})


# Add Lib, Include
add_library(${PROJECT_NAME} SHARED ${COREENGINE} ${RENDERERENGINE} ${PLATFORMWINDOWS} ${GLAD} ${IMGUI} ${DEBUG} ${OPENGL} ${VULKAN} ${SCENE} ${MATH} ${EXTENDED_YAMLCPP} ${CORAL} ${SCRIPTING} ${UTILS} ${PROJECTCORE} ${PHYSIC2DCORE} ${VULKANBOOSTSTRAP} ${VULKANMEMORYALLOCATOR})

find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC 
                    ${CMAKE_SOURCE_DIR}/include
                    ${Vulkan_INCLUDE_DIRS}
                )
                
target_link_directories(${PROJECT_NAME} PUBLIC 
                    ${CMAKE_SOURCE_DIR}/lib
                )

target_link_libraries(${PROJECT_NAME} PUBLIC glfw3dll opengl32 gdi32 yaml-cpp.dll box2d assimp.dll ${Vulkan_LIBRARIES} stdc++exp)

# Add Lib By Conditions
if(ENABLE_RENDERER_OPENGL)
    message(STATUS "Renderer: OpenGL")
    add_library(${PROJECT_NAME} SHARED ${OPENGL})
endif()

if(ENABLE_RENDERER_VULKAN)
    message(STATUS "Renderer: Vulkan")
    add_library(${PROJECT_NAME} SHARED ${VULKAN})

    add_compile_definitions(GLFW_INCLUDE_NONE)
    add_compile_definitions(GLFW_INCLUDE_VULKAN)
    add_compile_definitions(VULKAN_HPP_NO_EXCEPTIONS)
endif()

if(ENABLE_TRACY_PROFILER)
    message(STATUS "Tracy On")
    add_library(${PROJECT_NAME} SHARED ${TRACYPROFILER})
endif()


# Compile Define
add_compile_definitions(GLFW_INCLUDE_NONE)

add_compile_definitions(GLFW_INCLUDE_VULKAN)

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

add_compile_definitions(YAML_CPP_STATIC_DEFINE)

add_compile_definitions(CORAL_WINDOWS)


# Cpp Check Best Practive
if(ENABLE_CPPCHECK)

    message(STATUS "Use CppCheck")

    find_program(cppcheck_exe NAMES cppcheck REQUIRED)

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

endif()