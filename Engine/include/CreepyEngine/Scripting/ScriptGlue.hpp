#pragma once


namespace Creepy {
    
    class ScriptGlue
    {
        public:
            static void RegisterFunctions() noexcept;
            static void RegisterComponents() noexcept;
    };
    
}