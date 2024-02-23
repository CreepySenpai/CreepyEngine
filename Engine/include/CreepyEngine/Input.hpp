#pragma once

#include <memory>

namespace Creepy
{
    class Input
    {
        public:
            // We create an interface to implement check key in every platform
            static constexpr inline bool IsKeyPressed(int keyCode) noexcept {
                return instance->IsKeyPressedImpl(keyCode);
            }

            static constexpr inline bool IsMouseButtonPressed(int mouseButton) noexcept {
                return instance->IsMouseButtonPressedImpl(mouseButton);
            }

            static constexpr inline float GetMouseX() noexcept {
                return instance->GetMouseXImpl();
            }

            static constexpr inline float GetMouseY() noexcept {
                return instance->GetMouseYImpl();
            }

            static constexpr inline std::pair<float, float> GetMousePosition() noexcept {
                return instance->GetMousePositionImpl();
            }


        protected:
            virtual bool IsKeyPressedImpl(int keyCode) noexcept = 0;
            
            virtual bool IsMouseButtonPressedImpl(int mouseButton) noexcept = 0;
            
            virtual float GetMouseXImpl() noexcept = 0;

            virtual float GetMouseYImpl() noexcept = 0;

            virtual std::pair<float, float> GetMousePositionImpl() noexcept = 0;

        private:
            static Ref<Input> instance;
    };
    
}