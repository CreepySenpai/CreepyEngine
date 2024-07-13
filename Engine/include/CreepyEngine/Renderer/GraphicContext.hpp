#pragma once


namespace Creepy
{
    class TimeStep;
    
    class GraphicContext
    {
        public:
            virtual ~GraphicContext() noexcept = default;
            virtual void Init() noexcept = 0;
            virtual void SwapBuffers() noexcept = 0;
            virtual void Update(TimeStep timeStep) noexcept = 0;
            virtual void ShutDown() noexcept = 0;
    };

    class GraphicContext2{
        public:
            void Init(this auto&& self) noexcept {
                self.InitImpl();
            }

            void SwapBuffers() noexcept {
                self.SwapBuffersImpl();
            }

            void Update(TimeStep timeStep) noexcept {
                self.UpdateImpl(timeStep);
            }

            void ShutDown() noexcept {
                self.ShutDownImpl();
            }
    };
    
}
