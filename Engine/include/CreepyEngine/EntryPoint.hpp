#pragma once

#include "Application.hpp"
#include "Log.hpp"
#include "Core.hpp"

extern std::unique_ptr<Creepy::Application> Creepy::CreateApplication();


int main(int argc, char** argv){
    Creepy::Log::Init();

    ENGINE_LOG_ERROR("{} rat dep trai\n", "Anh");
    APP_LOG_INFO("{} hello\n", "APP");

    auto app = Creepy::CreateApplication();
    app->Run();
    return 0;
}