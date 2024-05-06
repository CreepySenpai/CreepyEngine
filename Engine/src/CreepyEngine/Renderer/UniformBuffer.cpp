#include <CreepyEngine/Renderer/UniformBuffer.hpp>
#include <Platform/OpenGL/OpenGLUniformBuffer.hpp>
#include <Platform/Vulkan/VulkanUniformBuffer.hpp>

namespace Creepy{

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) noexcept {

        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLUniformBuffer>(size, binding);
        }

        else if constexpr(UseVulkanAPI){
            return std::make_shared<VulkanUniformBuffer>(size);
        }

        else{
            ENGINE_LOG_ERROR("Wrong API");
        
            return nullptr;
        }
        
    }

}