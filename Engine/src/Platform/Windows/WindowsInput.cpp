#include <CreepyEngine/Application.hpp>
#include <Platform/Windows/WindowsInput.hpp>
#include <GLFW/glfw3.h>

namespace Creepy
{
    std::shared_ptr<Input> Input::instance = std::make_shared<WindowsInput>();

    bool WindowsInput::IsKeyPressedImpl(int keyCode) noexcept
    {
        auto&& window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

        int state = glfwGetKey(window, keyCode);

        return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
    }
    
    bool WindowsInput::IsMouseButtonPressedImpl(int mouseButton) noexcept {
        auto&& window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

        int state = glfwGetMouseButton(window, mouseButton);

        return (state == GLFW_PRESS);
    }

    float WindowsInput::GetMouseXImpl() noexcept {
        auto [x, _] = GetMousePositionImpl();

        return x;
    }

    float WindowsInput::GetMouseYImpl() noexcept {
        auto [_, y] = GetMousePositionImpl();

        return y;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl() noexcept {
        auto&& window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        
        double x{0.0}, y{0.0};
        
        glfwGetCursorPos(window, &x, &y);

        return std::make_pair<float, float>(static_cast<float>(x), static_cast<float>(y));
    }

}