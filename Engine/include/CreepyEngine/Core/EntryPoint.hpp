#pragma once

#include "Application.hpp"
#include "Log.hpp"
#include "Core.hpp"
#include <iostream>

extern Creepy::Scope<Creepy::Application> Creepy::CreateApplication() noexcept;


int main(int argc, char** argv){
    Creepy::Log::Init();
    auto app = Creepy::CreateApplication();
    DEBUG_PROFILE_BEGIN_SESSION("App_Run", "appRun.json");
    app->Run();
    DEBUG_PROFILE_END_SESSION();
    
    return 0;
}