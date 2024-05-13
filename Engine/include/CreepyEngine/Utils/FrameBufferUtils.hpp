#pragma once

#include <CreepyEngine/Renderer/FrameBuffer.hpp>

namespace Creepy {

    constexpr bool IsDepthFormat(FrameBufferTextureFormat format) noexcept {

        switch(format){
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
            case FrameBufferTextureFormat::DEPTH32:
                return true;
        }

        return false;
    }

}