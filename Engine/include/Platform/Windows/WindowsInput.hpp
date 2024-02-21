#include <CreepyEngine/Input.hpp>

namespace Creepy
{
    class WindowsInput : public Input
    {
        public:
        
        protected:

            virtual bool IsKeyPressedImpl(int keyCode) noexcept override;
            
            virtual bool IsMouseButtonPressedImpl(int mouseButton) noexcept override;
            
            virtual float GetMouseXImpl() noexcept override;

            virtual float GetMouseYImpl() noexcept override;

            virtual std::pair<float, float> GetMousePositionImpl() noexcept override;

        private:
            
    };
}