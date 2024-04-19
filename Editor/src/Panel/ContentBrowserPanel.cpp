#include <string>
#include <Panel/ContentBrowserPanel.hpp>

#include <imgui/imgui.h>


namespace Creepy {

    // extern const std::filesystem::path AssetDirectory{"./assets"};

    ContentBrowserPanel::ContentBrowserPanel() noexcept : m_currentDirectory{Project::GetActive()->GetAssetDirectory()} {
        ENGINE_LOG_ERROR("Gonna Access1");
        if(Project::GetActive() == nullptr){
            ENGINE_LOG_WARNING("Null");
        }
        else {
            ENGINE_LOG_ERROR("Init Path: {}", Project::GetActive()->GetAssetDirectory().string());
            ENGINE_LOG_ERROR("Gonna Access2");
        }
        
        m_directoryIcon = Texture2D::Create("./assets/icons/directory_icon.png");
        m_fileIcon = Texture2D::Create("./assets/icons/file_icon.png");
    }

    void ContentBrowserPanel::OnImGuiRender() noexcept {
        ImGui::Begin("Content Browser");
        
        if(m_currentDirectory != Project::GetActive()->GetAssetDirectory()){
            if(ImGui::Button("<-")){
                m_currentDirectory = m_currentDirectory.parent_path();
            }
        }

        constexpr float padding = 16.f;
        constexpr float thumbnailSize = 120.f;
        constexpr float cellSize = thumbnailSize + padding;

        const float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = static_cast<int>(panelWidth / cellSize);

        if(columnCount < 1){
            columnCount = 1;
        }

        ImGui::Columns(columnCount, nullptr, false);

        for(auto&& entry : std::filesystem::directory_iterator(m_currentDirectory)){
            
            auto&& path = entry.path();

            auto&& relativePath = std::filesystem::relative(entry.path(), Project::GetActive()->GetAssetDirectory());

            auto&& fileName = relativePath.filename().string();

            ImGui::PushID(fileName.c_str());    // for select and open multi file

            const bool isDirectory = entry.is_directory();

            auto&& icon = isDirectory ? m_directoryIcon : m_fileIcon;
            
            ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
            
            ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)icon->GetRendererID()), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

            if(ImGui::BeginDragDropSource()){

                auto itemPath = relativePath.string().c_str();

                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (relativePath.string().size() + 1) * sizeof(char), ImGuiCond_Once);

                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                if(isDirectory){
                    m_currentDirectory /= path.filename();
                }
            }

            ImGui::TextWrapped(fileName.c_str());
            ImGui::NextColumn();

            ImGui::PopID();
        }
        
        // Reset
        ImGui::Columns(1);

        ImGui::End();
    }

}