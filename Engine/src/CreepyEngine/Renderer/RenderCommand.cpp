#include <CreepyEngine/Renderer/RenderCommand.hpp>
#include <Platform/OpenGL/OpenGLRendererAPI.hpp>

namespace Creepy
{
    Scope<RendererAPI> RenderCommand::s_rendererAPI = std::make_unique<OpenGLRendererAPI>();
    
}
