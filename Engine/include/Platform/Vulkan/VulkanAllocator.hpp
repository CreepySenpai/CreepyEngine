#pragma once

#include <vulkan/vulkan.hpp>
#include <VulkanMemoryAllocator/vk_mem_alloc.hpp>

namespace Creepy {

    struct VulkanAllocator
    {
        VmaAllocator GlobalAllocator;

        void InitAllocator(const vk::Instance instance, const vk::PhysicalDevice physicalDev, const vk::Device logicalDev) noexcept{
            VmaAllocatorCreateInfo allocatorInfo{};
            allocatorInfo.instance = instance;
            allocatorInfo.physicalDevice = physicalDev;
            allocatorInfo.device = logicalDev;
            allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

            vmaCreateAllocator(&allocatorInfo, &GlobalAllocator);
        }

        void DestroyAllocator() noexcept {
            vmaDestroyAllocator(GlobalAllocator);
        }
    };
    

}