#pragma once

#include <filesystem>
#include <CreepyEngine.hpp>

namespace Creepy {

    class ContentBrowserPanel
    {
        public:
            ContentBrowserPanel() noexcept;

            void OnImGuiRender() noexcept;

        private:
            std::filesystem::path m_currentDirectory;
            Ref<Texture2D> m_directoryIcon, m_fileIcon;
    };
}