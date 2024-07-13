#pragma once

#include <CreepyEngine/Renderer/GraphicContext.hpp>


class GLFWwindow;

namespace Creepy {

    class TimeStep;

    class OpenGLContext2 : public GraphicContext2
    {
        friend class GraphicContext2;
        
        public:
            OpenGLContext2(GLFWwindow* windowHandle) noexcept;
            
        private:
            void InitImpl() noexcept;
            void SwapBuffersImpl() noexcept;
            void UpdateImpl(TimeStep timeStep) noexcept;
            void ShutDownImpl() noexcept;
        private:
            GLFWwindow* m_windowHandle;
    };
    
}