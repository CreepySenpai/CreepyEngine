#include <ranges>
#include <CreepyEngine/Core/Application.hpp>
#include <CreepyEngine/Utils/PlatformUtils.hpp>
#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Creepy {

    std::string FileDialogs::OpenFile(const char* filter) noexcept {
        std::string filePath{};

        OPENFILENAMEA fileToOpen;
        char filePathBuffer[260];
        std::ranges::fill(filePathBuffer, 0);

        ZeroMemory(&fileToOpen, sizeof(OPENFILENAMEA));
        fileToOpen.lStructSize = sizeof(OPENFILENAMEA);
        fileToOpen.hwndOwner = glfwGetWin32Window(reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()));
        fileToOpen.lpstrFile = filePathBuffer;
        fileToOpen.nMaxFile = sizeof(filePathBuffer);
        fileToOpen.lpstrFilter = filter;
        fileToOpen.nFilterIndex = 1;
        fileToOpen.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;     // Set default directory in .exe file

        if(GetOpenFileNameA(&fileToOpen) == TRUE){
            filePath.assign(fileToOpen.lpstrFile);
        }
        return filePath;
    }

    std::string FileDialogs::SaveFile(const char* filter) noexcept{
        std::string filePath{};

        OPENFILENAMEA fileToOpen;
        char filePathBuffer[260];
        std::ranges::fill(filePathBuffer, 0);

        ZeroMemory(&fileToOpen, sizeof(OPENFILENAMEA));
        fileToOpen.lStructSize = sizeof(OPENFILENAMEA);
        fileToOpen.hwndOwner = glfwGetWin32Window(reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()));
        fileToOpen.lpstrFile = filePathBuffer;
        fileToOpen.nMaxFile = sizeof(filePathBuffer);
        fileToOpen.lpstrFilter = filter;
        fileToOpen.nFilterIndex = 1;
        fileToOpen.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;     // Set default directory in .exe file

        if(GetSaveFileNameA(&fileToOpen) == TRUE){
            filePath.assign(fileToOpen.lpstrFile);
        }
        return filePath;
    }

}