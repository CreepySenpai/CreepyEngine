#include <CreepyEngine/Renderer/RenderCommand.hpp>
#include <Platform/OpenGL/OpenGLRendererAPI.hpp>

namespace Creepy
{
    std::unique_ptr<RendererAPI> RenderCommand::s_rendererAPI = std::make_unique<OpenGLRendererAPI>();
    
}
