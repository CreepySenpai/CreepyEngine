#pragma once

#include <CreepyEngine/Renderer/GraphicContext.hpp>
#include <GLFW/glfw3.h>

namespace Creepy {

    class OpenGLContext : public GraphicContext
    {
        public:
            OpenGLContext(GLFWwindow* windowHandle);

            void Init() noexcept override;
            void SwapBuffers() noexcept override;
        private:
            GLFWwindow* m_windowHandle;
    };
    
}