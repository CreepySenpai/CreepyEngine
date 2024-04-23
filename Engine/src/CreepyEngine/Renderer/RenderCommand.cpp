#include <CreepyEngine/Renderer/RenderCommand.hpp>
// #include <Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <Platform/Vulkan/VulkanRendererAPI.hpp>

namespace Creepy
{
    // Scope<RendererAPI> RenderCommand::s_rendererAPI = std::make_unique<OpenGLRendererAPI>();
    Scope<RendererAPI> RenderCommand::s_rendererAPI = std::make_unique<VulkanRendererAPI>();
    
}
