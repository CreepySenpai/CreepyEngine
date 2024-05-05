#include <iostream>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanSwapChain.hpp>
#include <Platform/Vulkan/VulkanRenderPass.hpp>
#include <Platform/Vulkan/VulkanImage.hpp>
#include <Platform/Vulkan/VulkanShader.hpp>
#include <Platform/Vulkan/VulkanPipeline.hpp>
#include <Platform/Vulkan/VulkanDescriptor.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>
#include <CreepyEngine/Utils/VulkanUtils.hpp>
#include <GLFW/glfw3.h>

namespace Creepy{

    VulkanContext* VulkanContext::s_instance = nullptr;

    VulkanContext::VulkanContext(GLFWwindow* windowHandle) noexcept : m_windowHandle{windowHandle} {
        s_instance = this;
         // Create Instance
        initInstance();
        std::clog << "Create Debug\n";

        // Create Debugger
        initDebugMessage();
        std::clog << "Create Surface\n";

        // Create Surface
        initSurface();

        std::clog << "Create Device\n";

        // Create Device
        initDevice();

        std::clog << "Create SwapChain\n";

        // Create SwapChain
        initSwapChain();

        std::clog << "Create Main RenderPass\n";
        initRenderPass();

        std::clog << "Create Frame Buffer\n";
        createFrameBuffer();

        std::clog << "Create Command Buffer\n";
        createCommandBuffer();

        std::clog << "Create Sync Obj\n";
        createSyncObject();

        std::clog << "Create Descriptor\n";
        createDescriptor();

        std::clog << "Create Shader\n";
        // TODO: Remove
        createShader();

        std::clog << "Create Pipeline\n";
        createPipeline();
    }

    void VulkanContext::Init() noexcept {
        
    }

    void VulkanContext::SwapBuffers() noexcept {

    }

    void VulkanContext::Update(TimeStep timeStep) noexcept {
        // std::clog << "W: " << m_cacheFrameBufferWidth << ", H: " << m_cacheFrameBufferHeight << '\n';
        // if(m_cacheFrameBufferWidth != 0 && m_cacheFrameBufferHeight != 0){
            
        // }

        BeginFrame(timeStep);

        EndFrame();
        
    }

    void VulkanContext::initInstance() noexcept {
        uint32_t version{0};
        auto _ = vk::enumerateInstanceVersion(&version);

        vk::ApplicationInfo appInfo("Test", version, "Creepy Engine Test", version, version);

        // Get Require Extension
        uint32_t extensionGLFWRequire{0};
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionGLFWRequire);
        std::vector<const char*> extensions{glfwExtensions, glfwExtensions + extensionGLFWRequire};
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        for(auto& ex : extensions){
            std::clog << "Require Ex: " << ex << '\n';
        }

        // Validation Layer
        const std::vector<const char*> layers{
            "VK_LAYER_KHRONOS_validation"
        };

        auto supportLayers = vk::enumerateInstanceLayerProperties();

        for(auto& layer : supportLayers){
            std::clog << "Support Layer: " << layer.layerName.data() << '\n';
        }

        vk::InstanceCreateInfo instanceInfo{};
        instanceInfo.flags = vk::InstanceCreateFlags{};
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceInfo.ppEnabledExtensionNames = extensions.data();
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        instanceInfo.ppEnabledLayerNames = layers.data();
        
        std::clog << "Gonna Create Instance\n";

        VULKAN_CHECK_ERROR(m_instance = vk::createInstance(instanceInfo, nullptr));

        m_dynamicLoader = vk::DispatchLoaderDynamic(m_instance, vkGetInstanceProcAddr);
    }

    void VulkanContext::initDebugMessage() noexcept {
        vk::DebugUtilsMessengerCreateInfoEXT debugInfo{
            vk::DebugUtilsMessengerCreateFlagsEXT{},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            VulkanUtils::VulkanDebugCallBack,
            nullptr
        };
        
        std::clog << "Gonna Create Debug\n";
        
        m_debugUtils = m_instance.createDebugUtilsMessengerEXT(debugInfo, nullptr, m_dynamicLoader);

        std::clog << "Create Vulkan Debug\n";
    }

    void VulkanContext::initSurface() noexcept {
        VkSurfaceKHR surfaceHandle;
        
        if(glfwCreateWindowSurface(m_instance, m_windowHandle, nullptr, &surfaceHandle) != VK_SUCCESS) {
            std::clog << "Cannot Create Surface\n";
        }

        m_surface = vk::SurfaceKHR{surfaceHandle};

        std::clog << "Created Vulkan Surface\n";
    }
    void VulkanContext::initDevice() noexcept {
        m_devices = std::make_shared<VulkanDevice>();
    }

    void VulkanContext::initSwapChain() noexcept {
        VulkanSwapChainSpec swapChainSpec{};
        swapChainSpec.Width = FrameBufferWidth;
        swapChainSpec.Height = FrameBufferHeight;
        swapChainSpec.GraphicFamilyIndex = m_devices->GetGraphicsFamilyIndex();
        swapChainSpec.PresentFamilyIndex = m_devices->GetPresentFamilyIndex();
        swapChainSpec.Surface = m_surface;
        swapChainSpec.LogicalDev = m_devices->GetLogicalDeviceHandle();
        m_swapChains = std::make_shared<VulkanSwapChain>(swapChainSpec);
    }

    void VulkanContext::initRenderPass() noexcept {
        VulkanRenderPassSpec renderPassSpec;
        
        renderPassSpec.RenderArea = glm::vec4{0.0f, 0.0f, FrameBufferWidth, FrameBufferHeight};
        renderPassSpec.ClearColor = glm::vec4{0.0f, 0.0f, 0.2f, 0.1f};
        renderPassSpec.Depth = 1.0f;
        renderPassSpec.Stencil = 0;
        renderPassSpec.LogicalDev = m_devices->GetLogicalDeviceHandle();

        m_mainRenderPass = std::make_shared<VulkanRenderPass>(renderPassSpec);
    }

    void VulkanContext::createFrameBuffer() noexcept {
        auto&& frameBuffers = m_swapChains->GetFrameBuffers();
        for(auto&& frame : frameBuffers) {
            frame.Destroy(m_devices->GetLogicalDeviceHandle());
        }

        frameBuffers.clear();
        
        for(uint32_t i{}; i < m_swapChains->GetMaxFramesInFlight(); ++i){
            VulkanFrameBufferSpec frameBufferSpec;
            
            frameBufferSpec.Attachments = {m_swapChains->GetImageViews()[i], m_swapChains->GetDepthBuffer()->GetImageView()};

            frameBufferSpec.Width = FrameBufferWidth;
            frameBufferSpec.Height = FrameBufferHeight;
            frameBufferSpec.RenderPassHandle = m_mainRenderPass->GetHandle();
            frameBufferSpec.LogicalDev = m_devices->GetLogicalDeviceHandle();

            frameBuffers.emplace_back(frameBufferSpec);
        }

        std::clog << "Total Frame Buffer: " << frameBuffers.size() << '\n';
    }

    void VulkanContext::createCommandBuffer() noexcept {

        for(auto&& command : m_graphicCommandBuffers){
            command.Free(m_devices->GetLogicalDeviceHandle(), m_devices->GraphicCommandPool);
        }

        m_graphicCommandBuffers.clear();

        for(auto&& image : m_swapChains->GetImages()){
            m_graphicCommandBuffers.emplace_back(m_devices->GetLogicalDeviceHandle(), m_devices->GraphicCommandPool, vk::CommandBufferLevel::ePrimary);
        }

        std::clog << "Total Command Buff: " << m_graphicCommandBuffers.size() << '\n';
    }

    void VulkanContext::createSyncObject() noexcept {

        m_imagesAvailable.reserve(m_swapChains->GetMaxFramesInFlight());
        m_queuesComplete.reserve(m_swapChains->GetMaxFramesInFlight());
        m_inFlights.reserve(m_swapChains->GetMaxFramesInFlight());

        for(uint32_t i{}; i < m_swapChains->GetMaxFramesInFlight(); ++i){
            vk::SemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.flags = vk::SemaphoreCreateFlags{};
            m_imagesAvailable.emplace_back(m_devices->GetLogicalDeviceHandle().createSemaphore(semaphoreInfo));
            m_queuesComplete.emplace_back(m_devices->GetLogicalDeviceHandle().createSemaphore(semaphoreInfo));
            m_inFlights.emplace_back(m_devices->GetLogicalDeviceHandle(), true);
        }

        std::clog << "Total ImgSem: " << m_imagesAvailable.size() << '\n';
        std::clog << "Total Fence: " << m_inFlights.size() << '\n';
    }

    void VulkanContext::recreateSwapChain() noexcept {
        
        if(m_isRecreatingSwapChain){
            std::clog << "Already Recreate SwapChain\n";
            return;
        }

        std::clog << "Recreate SwapChain Inside\n";

        m_isRecreatingSwapChain = true;
        FrameBufferWidth = m_cacheFrameBufferWidth;
        FrameBufferHeight = m_cacheFrameBufferHeight;

        m_devices->GetLogicalDeviceHandle().waitIdle();

        m_devices->ReQuerySwapChainSupport(m_surface);
        VulkanSwapChainSpec swapChainSpec{};
        swapChainSpec.Width = FrameBufferWidth;
        swapChainSpec.Height = FrameBufferHeight;
        swapChainSpec.GraphicFamilyIndex = m_devices->GetGraphicsFamilyIndex();
        swapChainSpec.PresentFamilyIndex = m_devices->GetPresentFamilyIndex();
        swapChainSpec.Surface = m_surface;
        swapChainSpec.LogicalDev = m_devices->GetLogicalDeviceHandle();
        m_swapChains->Recreate(swapChainSpec);
        m_mainRenderPass->GetRenderArea().z = FrameBufferWidth;
        m_mainRenderPass->GetRenderArea().w = FrameBufferHeight;

        for(auto&& command : m_graphicCommandBuffers){
            command.Free(m_devices->GetLogicalDeviceHandle(), m_devices->GraphicCommandPool);
        }

        m_graphicCommandBuffers.clear();
        
        m_mainRenderPass->GetRenderArea().x = 0.0f;
        m_mainRenderPass->GetRenderArea().y = 0.0f;
        m_mainRenderPass->GetRenderArea().z = FrameBufferWidth;
        m_mainRenderPass->GetRenderArea().w = FrameBufferHeight;

        
        createFrameBuffer();
        createCommandBuffer();
        // createSyncObject();

        
        
        m_isRecreatingSwapChain = false;
    }

    void VulkanContext::ShutDown() noexcept {
        auto&& logicalDev = m_devices->GetLogicalDeviceHandle();
        logicalDev.waitIdle();

        {
            
            std::clog << "Call Destroy Pipeline\n";
            m_pipeLine->Destroy(logicalDev);
            
            std::clog << "Call Destroy Des\n";
            m_descriptorSet->Destroy();
            m_descriptorSetLayout->Destroy();
            m_descriptorPool->Destroy();


            std::clog << "Call Destroy Shader\n";
            m_nahShader->Destroy();
        }

        std::clog << "Call Destroy Sync Obj\n";

        for(auto&& imgSem : m_imagesAvailable){
            logicalDev.destroySemaphore(imgSem);
        }

        for(auto&& queueSem : m_queuesComplete){
            logicalDev.destroySemaphore(queueSem);
        }

        for(auto&& fen : m_inFlights){
            fen.Destroy(logicalDev);
        }

        std::clog << "Call Destroy Command Buffer\n";

        for(auto&& command : m_graphicCommandBuffers){
            command.Free(logicalDev, m_devices->GraphicCommandPool);
        }

        std::clog << "Call Destroy Frame Buffer\n";

        for(auto&& frame : m_swapChains->GetFrameBuffers()) {
            frame.Destroy(logicalDev);
        }

        std::clog << "Call Destroy RenderPass\n";

        m_mainRenderPass->Destroy(logicalDev);

        std::clog << "Call Destroy Swapchain\n";

        m_swapChains->Destroy(logicalDev);

        std::clog << "Call Destroy Device\n";

        m_devices->DestroyDevice();

        m_dynamicLoader.vkDestroyDebugUtilsMessengerEXT(m_instance, static_cast<VkDebugUtilsMessengerEXT>(m_debugUtils), nullptr);

        std::clog << "Call Destroy Debug\n";

        m_instance.destroySurfaceKHR(m_surface);

        std::clog << "Call Destroy Surface\n";
        m_instance.destroy();
        std::clog << "Destroy Vulkan\n";
    }

    std::shared_ptr<VulkanSwapChain> VulkanContext::GetSwapChain() const noexcept {
        return m_swapChains;
    }

    std::shared_ptr<VulkanDevice> VulkanContext::GetDevices() const noexcept {
        return m_devices;
    }

    void VulkanContext::BeginFrame(TimeStep timeStep) noexcept {
        if(m_isRecreatingSwapChain){
            m_devices->GetLogicalDeviceHandle().waitIdle();
        }

        if(FrameBufferWidth != m_cacheFrameBufferWidth || FrameBufferHeight != m_cacheFrameBufferHeight){
            recreateSwapChain();
        }

        m_inFlights.at(m_currentFrame).Wait(m_devices->GetLogicalDeviceHandle(), std::numeric_limits<uint64_t>::max());

        try{
            m_currentImageIndex = m_swapChains->AcquireNextImageIndex(std::numeric_limits<uint64_t>::max(), m_imagesAvailable.at(m_currentFrame), nullptr);
        }
        catch(const vk::OutOfDateKHRError& e){
            std::clog << "Error: " << e.what() << '\n';
            recreateSwapChain();
        }

        auto commandBuffer = m_graphicCommandBuffers.at(m_currentImageIndex);
        commandBuffer.Reset();
        commandBuffer.Begin(false, false, false);

        vk::Viewport viewPort{0.0f, static_cast<float>(FrameBufferHeight), static_cast<float>(FrameBufferWidth), -static_cast<float>(FrameBufferHeight), 0.0f, 1.0f};

        vk::Rect2D scissor{};
        scissor.extent.width = FrameBufferWidth;
        scissor.extent.height = FrameBufferHeight;

        commandBuffer.GetHandle().setViewport(0, viewPort);
        commandBuffer.GetHandle().setScissor(0, scissor);

        m_mainRenderPass->GetRenderArea().z = FrameBufferWidth;
        m_mainRenderPass->GetRenderArea().w = FrameBufferHeight;

        m_mainRenderPass->Begin(m_graphicCommandBuffers.at(m_currentImageIndex), m_swapChains->GetFrameBuffers().at(m_currentImageIndex).GetHandle());
    }
    
    void VulkanContext::EndFrame() noexcept{
        auto&& commandBuffer = m_graphicCommandBuffers.at(m_currentImageIndex);

        m_mainRenderPass->End(commandBuffer);

        commandBuffer.End();

        m_inFlights.at(m_currentFrame).Wait(m_devices->GetLogicalDeviceHandle(), std::numeric_limits<uint64_t>::max());

        m_inFlights.at(m_currentFrame).Reset(m_devices->GetLogicalDeviceHandle());

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.GetHandle();
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_queuesComplete.at(m_currentFrame);
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_imagesAvailable.at(m_currentFrame);

        vk::PipelineStageFlags flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        submitInfo.pWaitDstStageMask = &flags;

        m_devices->GetGraphicsQueue().submit(submitInfo, m_inFlights.at(m_currentFrame).GetHandle());
        commandBuffer.UpdateSubmitted();

        // vk::Result ress{};

        auto ii = m_swapChains->GetHandle();

        vk::PresentInfoKHR presentInfo{};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_queuesComplete.at(m_currentFrame);
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &ii;
        presentInfo.pImageIndices = &m_currentImageIndex;

        try{
            auto i = m_devices->GetPresentQueue().presentKHR(presentInfo);
        }
        catch(const vk::OutOfDateKHRError& e){
            std::clog << "Need Recreate Swap\n";
            recreateSwapChain();
        }

        // try {
            // ress = m_swapChains->Present(m_devices->GetPresentQueue(), m_queuesComplete.at(m_currentFrame), m_currentImageIndex);
        // }
        // catch(const vk::OutOfDateKHRError& e){
        //     std::clog << "Need Recreate SwapChain\n";
        //     recreateSwapChain();
        //     ress = vk::Result::eErrorOutOfDateKHR;
        // }
        
        // if(ress == vk::Result::eErrorOutOfDateKHR | ress == vk::Result::eSuboptimalKHR){
        //     std::clog << "Need Recreate SwapChain2\n";
        //     recreateSwapChain();
        // }

        m_currentFrame = (m_currentFrame + 1) % m_swapChains->GetMaxFramesInFlight();
    }
    
    void VulkanContext::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
        m_cacheFrameBufferWidth = width;
        m_cacheFrameBufferHeight = height;
    }

    
    // TODO: Remove
    void VulkanContext::createDescriptor() noexcept {
        m_descriptorPool = std::make_shared<VulkanDescriptorPool>();
        
        m_descriptorSetLayout = std::make_shared<VulkanDescriptorSetLayout>(std::initializer_list<VulkanDescriptorSetLayoutSpec>{
            {0, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex}
        });

        const std::array layout{m_descriptorSetLayout->GetLayout(), m_descriptorSetLayout->GetLayout(), m_descriptorSetLayout->GetLayout()};
        m_descriptorSet = std::make_shared<VulkanDescriptorSet>(layout);

    }

    void VulkanContext::createShader() noexcept {
        m_nahShader = std::make_shared<VulkanShader>("./assets/shaders/cache/vulkan/RectVertexShader.vulkan.spv", "./assets/shaders/cache/vulkan/RectFragmentShader.vulkan.spv");
    }

    void VulkanContext::createPipeline() noexcept {
        vk::Viewport viewPort;
        viewPort.x = 0.0f;
        viewPort.y = (float)FrameBufferHeight;
        viewPort.width = (float)FrameBufferWidth;
        viewPort.height = (float)FrameBufferHeight;
        viewPort.minDepth = 0.0f;
        viewPort.maxDepth = 1.0f;

        vk::Rect2D scissor;
        scissor.offset.x = scissor.offset.y = 0;
        scissor.extent.width = FrameBufferWidth;
        scissor.extent.height = FrameBufferHeight;
        
        std::vector<vk::VertexInputAttributeDescription> attributes;

        VulkanPipelineSpec pipeSpec;
        pipeSpec.Viewport = viewPort;
        pipeSpec.Scissor = scissor;
        pipeSpec.Attributes = attributes;
        pipeSpec.ShaderStages = m_nahShader->GetShaderStages();
        pipeSpec.IsWireFrame = false;
        pipeSpec.RenderPass = m_mainRenderPass->GetHandle();

        const std::array layouts{m_descriptorSetLayout->GetLayout(), m_descriptorSetLayout->GetLayout(), m_descriptorSetLayout->GetLayout()};

        pipeSpec.DescriptorSetLayouts = layouts;
        
        m_pipeLine = std::make_shared<VulkanPipeline>(pipeSpec);
    }
}