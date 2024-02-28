#pragma once

#include <CreepyEngine/Core/Layer.hpp>
#include <CreepyEngine/Core/KeyCode.hpp>
#include <CreepyEngine/Events/MouseEvent.hpp>
#include <CreepyEngine/Events/ApplicationEvent.hpp>
#include <CreepyEngine/Events/KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

namespace Creepy
{
    class ImGuiLayer : public Layer
    {
        public:
            ImGuiLayer() noexcept;
            virtual ~ImGuiLayer() = default;

            void OnAttach() noexcept override;
            void OnDetach() noexcept override;
            void OnImGuiRender() noexcept override;

            void Begin() noexcept;
            void End() noexcept;

            // void OnEvent(Event& event) noexcept override;
        private:
            float m_time{0.0f};

        private:

            constexpr inline static ImGuiKey Creepy_Key_To_ImGui(int keyCode) noexcept
            {
                switch (keyCode)
                {
                case CREEPY_KEY_TAB:
                    return ImGuiKey_Tab;
                case CREEPY_KEY_LEFT:
                    return ImGuiKey_LeftArrow;
                case CREEPY_KEY_RIGHT:
                    return ImGuiKey_RightArrow;
                case CREEPY_KEY_UP:
                    return ImGuiKey_UpArrow;
                case CREEPY_KEY_DOWN:
                    return ImGuiKey_DownArrow;
                case CREEPY_KEY_PAGE_UP:
                    return ImGuiKey_PageUp;
                case CREEPY_KEY_PAGE_DOWN:
                    return ImGuiKey_PageDown;
                case CREEPY_KEY_HOME:
                    return ImGuiKey_Home;
                case CREEPY_KEY_END:
                    return ImGuiKey_End;
                case CREEPY_KEY_INSERT:
                    return ImGuiKey_Insert;
                case CREEPY_KEY_DELETE:
                    return ImGuiKey_Delete;
                case CREEPY_KEY_BACKSPACE:
                    return ImGuiKey_Backspace;
                case CREEPY_KEY_SPACE:
                    return ImGuiKey_Space;
                case CREEPY_KEY_ENTER:
                    return ImGuiKey_Enter;
                case CREEPY_KEY_ESCAPE:
                    return ImGuiKey_Escape;
                case CREEPY_KEY_APOSTROPHE:
                    return ImGuiKey_Apostrophe;
                case CREEPY_KEY_COMMA:
                    return ImGuiKey_Comma;
                case CREEPY_KEY_MINUS:
                    return ImGuiKey_Minus;
                case CREEPY_KEY_PERIOD:
                    return ImGuiKey_Period;
                case CREEPY_KEY_SLASH:
                    return ImGuiKey_Slash;
                case CREEPY_KEY_SEMICOLON:
                    return ImGuiKey_Semicolon;
                case CREEPY_KEY_EQUAL:
                    return ImGuiKey_Equal;
                case CREEPY_KEY_LEFT_BRACKET:
                    return ImGuiKey_LeftBracket;
                case CREEPY_KEY_BACKSLASH:
                    return ImGuiKey_Backslash;
                case CREEPY_KEY_RIGHT_BRACKET:
                    return ImGuiKey_RightBracket;
                case CREEPY_KEY_GRAVE_ACCENT:
                    return ImGuiKey_GraveAccent;
                case CREEPY_KEY_CAPS_LOCK:
                    return ImGuiKey_CapsLock;
                case CREEPY_KEY_SCROLL_LOCK:
                    return ImGuiKey_ScrollLock;
                case CREEPY_KEY_NUM_LOCK:
                    return ImGuiKey_NumLock;
                case CREEPY_KEY_PRINT_SCREEN:
                    return ImGuiKey_PrintScreen;
                case CREEPY_KEY_PAUSE:
                    return ImGuiKey_Pause;
                case CREEPY_KEY_KP_0:
                    return ImGuiKey_Keypad0;
                case CREEPY_KEY_KP_1:
                    return ImGuiKey_Keypad1;
                case CREEPY_KEY_KP_2:
                    return ImGuiKey_Keypad2;
                case CREEPY_KEY_KP_3:
                    return ImGuiKey_Keypad3;
                case CREEPY_KEY_KP_4:
                    return ImGuiKey_Keypad4;
                case CREEPY_KEY_KP_5:
                    return ImGuiKey_Keypad5;
                case CREEPY_KEY_KP_6:
                    return ImGuiKey_Keypad6;
                case CREEPY_KEY_KP_7:
                    return ImGuiKey_Keypad7;
                case CREEPY_KEY_KP_8:
                    return ImGuiKey_Keypad8;
                case CREEPY_KEY_KP_9:
                    return ImGuiKey_Keypad9;
                case CREEPY_KEY_KP_DECIMAL:
                    return ImGuiKey_KeypadDecimal;
                case CREEPY_KEY_KP_DIVIDE:
                    return ImGuiKey_KeypadDivide;
                case CREEPY_KEY_KP_MULTIPLY:
                    return ImGuiKey_KeypadMultiply;
                case CREEPY_KEY_KP_SUBTRACT:
                    return ImGuiKey_KeypadSubtract;
                case CREEPY_KEY_KP_ADD:
                    return ImGuiKey_KeypadAdd;
                case CREEPY_KEY_KP_ENTER:
                    return ImGuiKey_KeypadEnter;
                case CREEPY_KEY_KP_EQUAL:
                    return ImGuiKey_KeypadEqual;
                case CREEPY_KEY_LEFT_SHIFT:
                    return ImGuiKey_LeftShift;
                case CREEPY_KEY_LEFT_CONTROL:
                    return ImGuiKey_LeftCtrl;
                case CREEPY_KEY_LEFT_ALT:
                    return ImGuiKey_LeftAlt;
                case CREEPY_KEY_LEFT_SUPER:
                    return ImGuiKey_LeftSuper;
                case CREEPY_KEY_RIGHT_SHIFT:
                    return ImGuiKey_RightShift;
                case CREEPY_KEY_RIGHT_CONTROL:
                    return ImGuiKey_RightCtrl;
                case CREEPY_KEY_RIGHT_ALT:
                    return ImGuiKey_RightAlt;
                case CREEPY_KEY_RIGHT_SUPER:
                    return ImGuiKey_RightSuper;
                case CREEPY_KEY_MENU:
                    return ImGuiKey_Menu;
                case CREEPY_KEY_0:
                    return ImGuiKey_0;
                case CREEPY_KEY_1:
                    return ImGuiKey_1;
                case CREEPY_KEY_2:
                    return ImGuiKey_2;
                case CREEPY_KEY_3:
                    return ImGuiKey_3;
                case CREEPY_KEY_4:
                    return ImGuiKey_4;
                case CREEPY_KEY_5:
                    return ImGuiKey_5;
                case CREEPY_KEY_6:
                    return ImGuiKey_6;
                case CREEPY_KEY_7:
                    return ImGuiKey_7;
                case CREEPY_KEY_8:
                    return ImGuiKey_8;
                case CREEPY_KEY_9:
                    return ImGuiKey_9;
                case CREEPY_KEY_A:
                    return ImGuiKey_A;
                case CREEPY_KEY_B:
                    return ImGuiKey_B;
                case CREEPY_KEY_C:
                    return ImGuiKey_C;
                case CREEPY_KEY_D:
                    return ImGuiKey_D;
                case CREEPY_KEY_E:
                    return ImGuiKey_E;
                case CREEPY_KEY_F:
                    return ImGuiKey_F;
                case CREEPY_KEY_G:
                    return ImGuiKey_G;
                case CREEPY_KEY_H:
                    return ImGuiKey_H;
                case CREEPY_KEY_I:
                    return ImGuiKey_I;
                case CREEPY_KEY_J:
                    return ImGuiKey_J;
                case CREEPY_KEY_K:
                    return ImGuiKey_K;
                case CREEPY_KEY_L:
                    return ImGuiKey_L;
                case CREEPY_KEY_M:
                    return ImGuiKey_M;
                case CREEPY_KEY_N:
                    return ImGuiKey_N;
                case CREEPY_KEY_O:
                    return ImGuiKey_O;
                case CREEPY_KEY_P:
                    return ImGuiKey_P;
                case CREEPY_KEY_Q:
                    return ImGuiKey_Q;
                case CREEPY_KEY_R:
                    return ImGuiKey_R;
                case CREEPY_KEY_S:
                    return ImGuiKey_S;
                case CREEPY_KEY_T:
                    return ImGuiKey_T;
                case CREEPY_KEY_U:
                    return ImGuiKey_U;
                case CREEPY_KEY_V:
                    return ImGuiKey_V;
                case CREEPY_KEY_W:
                    return ImGuiKey_W;
                case CREEPY_KEY_X:
                    return ImGuiKey_X;
                case CREEPY_KEY_Y:
                    return ImGuiKey_Y;
                case CREEPY_KEY_Z:
                    return ImGuiKey_Z;
                case CREEPY_KEY_F1:
                    return ImGuiKey_F1;
                case CREEPY_KEY_F2:
                    return ImGuiKey_F2;
                case CREEPY_KEY_F3:
                    return ImGuiKey_F3;
                case CREEPY_KEY_F4:
                    return ImGuiKey_F4;
                case CREEPY_KEY_F5:
                    return ImGuiKey_F5;
                case CREEPY_KEY_F6:
                    return ImGuiKey_F6;
                case CREEPY_KEY_F7:
                    return ImGuiKey_F7;
                case CREEPY_KEY_F8:
                    return ImGuiKey_F8;
                case CREEPY_KEY_F9:
                    return ImGuiKey_F9;
                case CREEPY_KEY_F10:
                    return ImGuiKey_F10;
                case CREEPY_KEY_F11:
                    return ImGuiKey_F11;
                case CREEPY_KEY_F12:
                    return ImGuiKey_F12;
                case CREEPY_KEY_F13:
                    return ImGuiKey_F13;
                case CREEPY_KEY_F14:
                    return ImGuiKey_F14;
                case CREEPY_KEY_F15:
                    return ImGuiKey_F15;
                case CREEPY_KEY_F16:
                    return ImGuiKey_F16;
                case CREEPY_KEY_F17:
                    return ImGuiKey_F17;
                case CREEPY_KEY_F18:
                    return ImGuiKey_F18;
                case CREEPY_KEY_F19:
                    return ImGuiKey_F19;
                case CREEPY_KEY_F20:
                    return ImGuiKey_F20;
                case CREEPY_KEY_F21:
                    return ImGuiKey_F21;
                case CREEPY_KEY_F22:
                    return ImGuiKey_F22;
                case CREEPY_KEY_F23:
                    return ImGuiKey_F23;
                case CREEPY_KEY_F24:
                    return ImGuiKey_F24;
                default:
                    return ImGuiKey_None;
                }
            }
    };

}