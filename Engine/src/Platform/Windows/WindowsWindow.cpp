#include <Platform/Windows/WindowsWindow.hpp>
#include <CreepyEngine/Events/ApplicationEvent.hpp>
#include <CreepyEngine/Events/KeyEvent.hpp>
#include <CreepyEngine/Events/MouseEvent.hpp>
#include <CreepyEngine/Core/Log.hpp>
#include <Platform/OpenGL/OpenGLContext.hpp>



namespace Creepy {
    
    // Error Callback
    static void GFLW_ERROR_CALLBACK(int errorCode, const char* description) {
        ENGINE_LOG_ERROR("GLFW ERROR ({} : {})", errorCode, description);
    }

    Scope<Window> Window::Create(const WindowProperty& windowProperty) {
        return std::make_unique<WindowsWindow>(windowProperty);
    }

    WindowsWindow::WindowsWindow(const WindowProperty& windowProperty) noexcept {
        this->Init(windowProperty);
    }

    constexpr void WindowsWindow::OnUpdate() noexcept {
        glfwPollEvents();
        m_context->SwapBuffers();
    }

    constexpr void WindowsWindow::SetEventCallBack(const std::function<void(Event&)>& callBack) noexcept {
        m_windowData.EventCallBack = callBack;
    }

    constexpr void WindowsWindow::SetVSync(bool setting) noexcept {
        if(setting) {
            glfwSwapInterval(1);
            
        }
        else {
            glfwSwapInterval(0);
        }
        m_windowData.VSync = setting;
    }
    constexpr bool WindowsWindow::IsVSyncEnable() const noexcept {
        return m_windowData.VSync;
    }

    static bool isWindowInit{false};

    constexpr void WindowsWindow::Init(const WindowProperty& windowProperty) noexcept {
        m_windowData.Title = windowProperty.Title;
        m_windowData.Width = windowProperty.Width;
        m_windowData.Height = windowProperty.Height;
        
        ENGINE_LOG_INFO("Create Window {}, ({} : {})", m_windowData.Title, m_windowData.Width, m_windowData.Height);

        if(!isWindowInit) {
            auto init = glfwInit();

            if(!init) {
                ENGINE_LOG_ERROR("Couldn't init glfw");
            }
            glfwSetErrorCallback(GFLW_ERROR_CALLBACK);
            isWindowInit = true;
        }

        m_window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(static_cast<int>(m_windowData.Width), static_cast<int>(m_windowData.Height), m_windowData.Title.c_str(), nullptr, nullptr),
            [](GLFWwindow* window) {
                glfwDestroyWindow(window);
            });
        
        m_context = std::make_shared<OpenGLContext>(m_window.get());
        m_context->Init();

        glfwSetWindowUserPointer(m_window.get(), &m_windowData);    // Sign A Pointer To Window Property
        
        this->SetVSync(true);

        // Set CallBack Event
       
        this->AttachEvent();
        
    }


    // We create a memory to store windowData and assign it to window, every time have event it will cast it memory to data then we
    // call callback function with it parameter
    constexpr void WindowsWindow::AttachEvent() noexcept {

        glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) {
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            windowData.Width = width;
            windowData.Height = height;

            WindowResizeEvent event{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            windowData.EventCallBack(event);
        });

        glfwSetWindowCloseCallback(m_window.get(), [](GLFWwindow* window){
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            WindowCloseEvent event;

            windowData.EventCallBack(event);
        });

        glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods){
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            switch (action)
            {
                case GLFW_PRESS: {
                    KeyPressedEvent event{keyCode, 0};
                    windowData.EventCallBack(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event{keyCode};
                    windowData.EventCallBack(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event{keyCode, 1};
                    windowData.EventCallBack(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetCharCallback(m_window.get(), [](GLFWwindow* window, unsigned int keyCode){
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            KeyTypedEvent event{static_cast<int>(keyCode)};
            windowData.EventCallBack(event);

        });

        glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow* window, int button, int action, int mods){
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event{button};
                    windowData.EventCallBack(event);
                    break;
                }

                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event{button};
                    windowData.EventCallBack(event);
                    break;
                }
            
                default:
                    break;
            }
        });
        

        glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double xOffset, double yOffset){
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            MouseScrolledEvent event{xOffset, yOffset};

            windowData.EventCallBack(event);
        });
        

        glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xpos, double ypos){
            auto&& windowData = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event{xpos, ypos};

            windowData.EventCallBack(event);
        });
    }


    constexpr void WindowsWindow::ShutDown() noexcept {
        
    }
}