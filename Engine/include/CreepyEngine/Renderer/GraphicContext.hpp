#pragma once


namespace Creepy
{
    class GraphicContext
    {
        public:
            virtual void Init() noexcept = 0;
            virtual void SwapBuffers() noexcept = 0;
        private:
            
    };
    
}
