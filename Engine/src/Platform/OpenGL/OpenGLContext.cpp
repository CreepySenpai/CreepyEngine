#include <Platform/OpenGL/OpenGLContext.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <glad/glad.h>

namespace Creepy {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle{windowHandle} {

    }

    void OpenGLContext::Init() noexcept {

        glfwMakeContextCurrent(m_windowHandle);
        
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ENGINE_LOG_INFO("Init glad {}", status);

        // auto i = glGetString(GL_VENDOR);
        // ENGINE_LOG_INFO("Device Infor: {}", glGetString(GL_VENDOR));
        // ENGINE_LOG_INFO("Device Version: {}", glGetString(GL_VERSION));
        // ENGINE_LOG_INFO("Device Renderer: {}", glGetString(GL_RENDERER));
    }

    void OpenGLContext::SwapBuffers() noexcept {
        glfwSwapBuffers(m_windowHandle);
    }

}