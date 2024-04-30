#include <Platform/Vulkan/VulkanFence.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanFence::VulkanFence(vk::Device logicalDev, bool isSignaled) noexcept : m_isSignaled{isSignaled} {
        vk::FenceCreateInfo fenceInfo{};

        if(isSignaled){
            fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
        }

        VULKAN_CHECK_ERROR(m_handle = logicalDev.createFence(fenceInfo));
    }
            
    void VulkanFence::Wait(vk::Device logicalDev, uint64_t timeOut) noexcept {

        if(!m_isSignaled){
            auto&& res = logicalDev.waitForFences(m_handle, true, timeOut);

            switch(res){
                case vk::Result::eSuccess:{
                    m_isSignaled = true;
                    break;
                }
                case vk::Result::eTimeout:{
                    std::clog << "Fence Wait - Time Out\n";
                    break;
                }
                case vk::Result::eErrorDeviceLost:{
                    std::clog << "Fence Wait - Device Lost\n";
                    break;
                }
                case vk::Result::eErrorOutOfHostMemory:{
                    std::clog << "Fence Wait - Out Of Host Memory\n";
                    break;
                }
                case vk::Result::eErrorOutOfDeviceMemory:{
                    std::clog << "Fence Wait - Out Of Device Memory\n";
                    break;
                }
            }
        }
    }

    void VulkanFence::Reset(vk::Device logicalDev) noexcept {
        if(m_isSignaled) {
            logicalDev.resetFences(m_handle);
            m_isSignaled = false;
        }
    }
    
    void VulkanFence::Destroy(vk::Device logicalDev) noexcept {
        logicalDev.destroyFence(m_handle);
        m_isSignaled = false;
    }

}