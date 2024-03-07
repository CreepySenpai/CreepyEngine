#include <CreepyEngine/Core/Application.hpp>
#include <CreepyEngine/Core/Input.hpp>
#include <GLFW/glfw3.h>

namespace Creepy {

    bool Input::IsKeyPressed(KeyCode keyCode) noexcept
    {
        auto&& window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

        int state = glfwGetKey(window, std::to_underlying(keyCode));

        return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
    }
    
    bool Input::IsMouseButtonPressed(MouseButtonCode mouseButton) noexcept {
        auto&& window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

        int state = glfwGetMouseButton(window, std::to_underlying(mouseButton));

        return (state == GLFW_PRESS);
    }

    float Input::GetMouseX() noexcept {
        auto [x, _] = GetMousePosition();

        return x;
    }

    float Input::GetMouseY() noexcept {
        auto [_, y] = GetMousePosition();

        return y;
    }

    std::pair<float, float> Input::GetMousePosition() noexcept {
        auto&& window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        
        double x{0.0}, y{0.0};
        
        glfwGetCursorPos(window, &x, &y);

        return std::make_pair<float, float>(static_cast<float>(x), static_cast<float>(y));
    }
    
}