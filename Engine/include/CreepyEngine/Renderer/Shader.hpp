#pragma once

#include <string>
#include <span>
#include <filesystem>

#include <CreepyEngine/Core/Core.hpp>
#include <glm/glm.hpp>

namespace Creepy
{
    class Shader
    {
        public:
            virtual ~Shader() noexcept = default;
            virtual void Bind() const noexcept = 0;
            virtual void UnBind() const noexcept = 0;
            virtual const std::string& GetName() const noexcept = 0;
            virtual void SetName(const std::string& name) noexcept = 0;

            [[nodiscard]] static Ref<Shader> Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
    }; 

    class Shader2{
        public:
            void Bind(this auto&& self) noexcept {
                self.BindImpl();
            }

            void UnBind(this auto&& self) noexcept {
                self.UnBindImpl();
            }

            const std::string& GetName(this auto&& self) noexcept {
                return self.GetNameImpl();
            }

            void SetName(this auto&& self, const std::string& name) noexcept {
                self.SetNameImpl(name);
            }
    };

    class ShaderLibrary {
        public:
            void Add(Ref<Shader>& shader) noexcept;
            void Add(const std::string& name, Ref<Shader>& shader) noexcept;

            [[nodiscard]] Ref<Shader> Load(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
            [[nodiscard]] Ref<Shader> Load(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;

            [[nodiscard]] Ref<Shader> Get(const std::string& name) const noexcept;

        private:
            std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
}