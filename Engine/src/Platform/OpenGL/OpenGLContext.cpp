#include <Platform/OpenGL/OpenGLContext.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Creepy {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) noexcept : m_windowHandle{windowHandle} {

    }

    void OpenGLContext::Init() noexcept {

        glfwMakeContextCurrent(m_windowHandle);
        
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ENGINE_LOG_INFO("Init glad {}", status);
    }

    void OpenGLContext::SwapBuffers() noexcept {
        glfwSwapBuffers(m_windowHandle);
    }

    void OpenGLContext::ShutDown() noexcept {

    }

}