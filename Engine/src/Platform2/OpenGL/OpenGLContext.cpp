#include <Platform2/OpenGL/OpenGLContext.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Creepy {

    OpenGLContext2::OpenGLContext2(GLFWwindow* windowHandle) noexcept : m_windowHandle{windowHandle} {

    }
            
    void OpenGLContext2::InitImpl() noexcept {
        glfwMakeContextCurrent(m_windowHandle);
        
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ENGINE_LOG_INFO("Init glad {}", status);
    }

    void OpenGLContext2::SwapBuffersImpl() noexcept {
        glfwSwapBuffers(m_windowHandle);
    }

    void OpenGLContext2::UpdateImpl(TimeStep timeStep) noexcept {

    }

    void OpenGLContext2::ShutDownImpl() noexcept {

    }

}