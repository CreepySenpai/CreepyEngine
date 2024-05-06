#include <CreepyEngine/Renderer/FrameBuffer.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLFrameBuffer.hpp>
#include <Platform/Vulkan/VulkanFrameBuffer.hpp>

namespace Creepy {

    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& data) noexcept {
        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLFrameBuffer>(data);
        }

        else if constexpr(UseVulkanAPI){
            return nullptr;
        }
        else {
            ENGINE_LOG_ERROR("Wrong API!!!");
            
            return nullptr;
        }

    }

}