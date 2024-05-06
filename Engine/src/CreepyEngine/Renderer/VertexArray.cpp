#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLVertexArray.hpp>

namespace Creepy {

    Ref<VertexArray> VertexArray::Create() noexcept {
        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLVertexArray>();
        }

        ENGINE_LOG_ERROR("Error Wrong Render API!");
        return nullptr;
    }


}