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
    
}
