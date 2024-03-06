#pragma once

#include <iostream>
#include <format>

#include <glad/glad.h>

static inline void ClearError() noexcept {
    // Flush all error
    while (glGetError() != GL_NO_ERROR)
    {
        
    }
}

static inline void GetError(const char* func, int line) noexcept {
    while(auto error = glGetError()){
        std::clog << std::format("Error Code: {:#x} on function {}!!!", line, func);
    }
}


#define OPENGL_CHECK_ERROR(func) ClearError();  func;   GetError(#func, __LINE__)