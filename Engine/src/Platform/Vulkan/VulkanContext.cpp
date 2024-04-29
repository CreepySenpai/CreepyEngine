#include <iostream>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanSwapChain.hpp>
#include <Platform/Vulkan/VulkanRenderPass.hpp>
#include <Platform/Vulkan/VulkanImage.hpp>
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
    }

    void VulkanContext::Init() noexcept {
        
    }

    void VulkanContext::SwapBuffers() noexcept {

    }

    void VulkanContext::Update(TimeStep timeStep) noexcept {
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

        VULKAN_CHECK_ERROR(Instance = vk::createInstance(instanceInfo, nullptr));

        DynamicLoader = vk::DispatchLoaderDynamic(Instance, vkGetInstanceProcAddr);
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
        
        m_debugUtils = Instance.createDebugUtilsMessengerEXT(debugInfo, nullptr, DynamicLoader);

        std::clog << "Create Vulkan Debug\n";
    }

    void VulkanContext::initSurface() noexcept {
        VkSurfaceKHR surfaceHandle;
        
        if(glfwCreateWindowSurface(Instance, m_windowHandle, nullptr, &surfaceHandle) != VK_SUCCESS) {
            std::clog << "Cannot Create Surface\n";
        }

        Surface = vk::SurfaceKHR{surfaceHandle};

        std::clog << "Created Vulkan Surface\n";
    }
    void VulkanContext::initDevice() noexcept {
        Devices = std::make_shared<VulkanDevice>();
    }

    void VulkanContext::initSwapChain() noexcept {
        SwapChain = std::make_shared<VulkanSwapChain>(FrameBufferWidth, FrameBufferHeight);
    }

    void VulkanContext::initRenderPass() noexcept {
        VulkanRenderPassSpec renderPassSpec;
        
        renderPassSpec.RenderArea = glm::vec4{0.0f, 0.0f, FrameBufferWidth, FrameBufferHeight};
        renderPassSpec.ClearColor = glm::vec4{0.0f, 0.0f, 0.2f, 0.1f};
        renderPassSpec.Depth = 1.0f;
        renderPassSpec.Stencil = 0;

        MainRenderPass = std::make_shared<VulkanRenderPass>(renderPassSpec);
    }

    void VulkanContext::createFrameBuffer() noexcept {
        auto&& frameBuffers = SwapChain->GetFrameBuffers();
        for(auto&& frame : frameBuffers) {
            frame.Destroy();
        }

        frameBuffers.clear();
        
        for(uint32_t i{}; i < SwapChain->GetMaxFramesInFlight(); ++i){
            VulkanFrameBufferSpec frameBufferSpec;
            
            frameBufferSpec.Attachments = {SwapChain->GetImageViews()[i], SwapChain->GetDepthBuffer()->GetImageView()};

            frameBufferSpec.Width = FrameBufferWidth;
            frameBufferSpec.Height = FrameBufferHeight;
            frameBufferSpec.RenderPassHandle = MainRenderPass->GetHandle();

            frameBuffers.emplace_back(frameBufferSpec);
        }

        std::clog << "Total Frame Buffer: " << frameBuffers.size() << '\n';
    }

    void VulkanContext::createCommandBuffer() noexcept {

        for(auto&& command : GraphicCommandBuffers){
            command.Free(Devices->GraphicCommandPool);
        }

        GraphicCommandBuffers.clear();

        for(auto&& image : SwapChain->GetImages()){
            GraphicCommandBuffers.emplace_back(Devices->GraphicCommandPool, vk::CommandBufferLevel::ePrimary);
        }

        std::clog << "Total Command Buff: " << GraphicCommandBuffers.size() << '\n';
    }

    void VulkanContext::createSyncObject() noexcept {

        m_imagesAvailable.reserve(SwapChain->GetMaxFramesInFlight());
        m_queuesComplete.reserve(SwapChain->GetMaxFramesInFlight());
        m_inFlights.reserve(SwapChain->GetMaxFramesInFlight());

        for(uint32_t i{}; i < SwapChain->GetMaxFramesInFlight(); ++i){
            vk::SemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.flags = vk::SemaphoreCreateFlags{};
            m_imagesAvailable.emplace_back(Devices->GetLogicalDevice().createSemaphore(semaphoreInfo));
            m_queuesComplete.emplace_back(Devices->GetLogicalDevice().createSemaphore(semaphoreInfo));
            m_inFlights.emplace_back(true);
        }

        std::clog << "Total ImgSem: " << m_imagesAvailable.size() << '\n';
        std::clog << "Total Fence: " << m_inFlights.size() << '\n';
    }

    void VulkanContext::recreateSwapChain() noexcept {
        
        if(m_isRecreatingSwapChain){
            std::clog << "Already Recreate SwapChain\n";
            return;
        }

        std::clog << "Recreate SwapChain\n";

        m_isRecreatingSwapChain = true;
        FrameBufferWidth = m_cacheFrameBufferWidth;
        FrameBufferHeight = m_cacheFrameBufferHeight;
        Devices->GetLogicalDevice().waitIdle();

        Devices->ReQuerySwapChainSupport(Surface);
        SwapChain->Recreate(FrameBufferWidth, FrameBufferHeight);
        MainRenderPass->GetRenderArea().z = FrameBufferWidth;
        MainRenderPass->GetRenderArea().w = FrameBufferHeight;

        for(auto&& command : GraphicCommandBuffers){
            command.Free(Devices->GraphicCommandPool);
        }

        GraphicCommandBuffers.clear();
        
        MainRenderPass->GetRenderArea().x = 0.0f;
        MainRenderPass->GetRenderArea().y = 0.0f;
        MainRenderPass->GetRenderArea().z = FrameBufferWidth;
        MainRenderPass->GetRenderArea().w = FrameBufferHeight;

        
        createFrameBuffer();
        createCommandBuffer();
        // createSyncObject();

        
        
        m_isRecreatingSwapChain = false;
    }

    void VulkanContext::ShutDown() noexcept {
        Devices->GetLogicalDevice().waitIdle();

        std::clog << "Call Destroy Sync Obj\n";

        for(auto&& imgSem : m_imagesAvailable){
            Devices->GetLogicalDevice().destroySemaphore(imgSem);
        }

        for(auto&& queueSem : m_queuesComplete){
            Devices->GetLogicalDevice().destroySemaphore(queueSem);
        }

        for(auto&& fen : m_inFlights){
            fen.Destroy();
        }

        std::clog << "Call Destroy Command Buffer\n";

        for(auto&& command : GraphicCommandBuffers){
            command.Free(Devices->GraphicCommandPool);
        }

        std::clog << "Call Destroy Frame Buffer\n";

        for(auto&& frame : SwapChain->GetFrameBuffers()) {
            frame.Destroy();
        }

        std::clog << "Call Destroy RenderPass\n";

        MainRenderPass->Destroy();

        std::clog << "Call Destroy Swapchain\n";

        SwapChain->Destroy();

        std::clog << "Call Destroy Device\n";

        Devices->DestroyDevice();

        DynamicLoader.vkDestroyDebugUtilsMessengerEXT(Instance, static_cast<VkDebugUtilsMessengerEXT>(m_debugUtils), nullptr);

        std::clog << "Call Destroy Debug\n";

        Instance.destroySurfaceKHR(Surface);

        std::clog << "Call Destroy Surface\n";
        Instance.destroy();
        std::clog << "Destroy Vulkan\n";
    }

    vk::Device VulkanContext::GetLogicalDevice() const noexcept {
        return Devices->GetLogicalDevice();
    }

    int VulkanContext::FindMemoryIndex(uint32_t filterType, vk::MemoryPropertyFlags memoryFlags) noexcept {
        
        auto&& property = Devices->GetPhysicalDevice().getMemoryProperties();
        
        for(uint32_t i{}; i < property.memoryTypeCount; ++i){
            if(filterType & (1 << i) && (property.memoryTypes[i].propertyFlags & memoryFlags) == memoryFlags){
                return i;
            }
        }

        return -1;
    }

    void VulkanContext::BeginFrame(TimeStep timeStep) noexcept {
        if(m_isRecreatingSwapChain){
            Devices->GetLogicalDevice().waitIdle();
        }

        if(FrameBufferWidth != m_cacheFrameBufferWidth || FrameBufferHeight != m_cacheFrameBufferHeight){
            recreateSwapChain();
        }

        m_inFlights.at(CurrentFrame).Wait(std::numeric_limits<uint64_t>::max());

        try{
            CurrentImageIndex = SwapChain->AcquireNextImageIndex(std::numeric_limits<uint64_t>::max(), m_imagesAvailable.at(CurrentFrame), nullptr);
        }
        catch(const vk::OutOfDateKHRError& e){
            std::clog << "Error: " << e.what() << '\n';
            recreateSwapChain();
        }

        auto commandBuffer = GraphicCommandBuffers.at(CurrentImageIndex);
        commandBuffer.Reset();
        commandBuffer.Begin(false, false, false);

        vk::Viewport viewPort{0.0f, static_cast<float>(FrameBufferHeight), static_cast<float>(FrameBufferWidth), -static_cast<float>(FrameBufferHeight), 0.0f, 1.0f};

        vk::Rect2D scissor{};
        scissor.extent.width = FrameBufferWidth;
        scissor.extent.height = FrameBufferHeight;

        commandBuffer.GetHandle().setViewport(0, viewPort);
        commandBuffer.GetHandle().setScissor(0, scissor);

        MainRenderPass->GetRenderArea().z = FrameBufferWidth;
        MainRenderPass->GetRenderArea().w = FrameBufferHeight;

        MainRenderPass->Begin(GraphicCommandBuffers.at(CurrentImageIndex), SwapChain->GetFrameBuffers().at(CurrentImageIndex).GetHandle());
    }
    
    void VulkanContext::EndFrame() noexcept{
        auto&& commandBuffer = GraphicCommandBuffers.at(CurrentImageIndex);

        MainRenderPass->End(commandBuffer);

        commandBuffer.End();

        m_inFlights.at(CurrentFrame).Wait(std::numeric_limits<uint64_t>::max());

        m_inFlights.at(CurrentFrame).Reset();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.GetHandle();
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_queuesComplete.at(CurrentFrame);
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_imagesAvailable.at(CurrentFrame);

        vk::PipelineStageFlags flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        submitInfo.pWaitDstStageMask = &flags;

        Devices->GetGraphicsQueue().submit(submitInfo, m_inFlights.at(CurrentFrame).GetHandle());
        commandBuffer.UpdateSubmitted();

        vk::Result ress{};

        try {
            ress = SwapChain->Present(Devices->GetPresentQueue(), m_queuesComplete.at(CurrentFrame), CurrentImageIndex);
        }
        catch(const vk::OutOfDateKHRError& e){
            std::clog << "Need Recreate SwapChain\n";
            ress = vk::Result::eErrorOutOfDateKHR;
        }
        
        if(ress == vk::Result::eErrorOutOfDateKHR | ress == vk::Result::eSuboptimalKHR){
            std::clog << "Need Recreate SwapChain2\n";
            recreateSwapChain();
        }

        CurrentFrame = (CurrentFrame + 1) % SwapChain->GetMaxFramesInFlight();
    }
    
    void VulkanContext::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
        m_cacheFrameBufferWidth = width;
        m_cacheFrameBufferHeight = height;
    }
}