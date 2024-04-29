#include <Platform/Vulkan/VulkanFence.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanFence::VulkanFence(bool isSignaled) noexcept : m_isSignaled{isSignaled} {
        vk::FenceCreateInfo fenceInfo{};

        if(isSignaled){
            fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
        }

        VULKAN_CHECK_ERROR(m_handle = VulkanContext::GetInstance()->GetLogicalDevice().createFence(fenceInfo));
    }
            
    void VulkanFence::Wait(uint64_t timeOut) noexcept {

        if(!m_isSignaled){
            auto&& res = VulkanContext::GetInstance()->GetLogicalDevice().waitForFences(m_handle, true, timeOut);

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

    void VulkanFence::Reset() noexcept {
        if(m_isSignaled) {
            VulkanContext::GetInstance()->GetLogicalDevice().resetFences(m_handle);
            m_isSignaled = false;
        }
    }
    
    void VulkanFence::Destroy() noexcept {
        VulkanContext::GetInstance()->GetLogicalDevice().destroyFence(m_handle);
        m_isSignaled = false;
    }

}