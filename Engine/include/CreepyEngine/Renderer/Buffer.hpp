#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <CreepyEngine/Core.hpp>

namespace Creepy {

    enum class ShaderDataType {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    constexpr inline static uint32_t GetShaderDataTypeSize(ShaderDataType dataType) noexcept {

        switch (dataType)
        {
            case ShaderDataType::Float: {
                return 4;
            }
            case ShaderDataType::Float2: {
                return 8;
            }
            case ShaderDataType::Float3: {
                return 12;
            }
            case ShaderDataType::Float4: {
                return 16;
            }
            case ShaderDataType::Mat3: {
                return 4 * 3 * 3;
            }
            case ShaderDataType::Mat4: {
                return 4 * 4 * 4;
            }
            case ShaderDataType::Int: {
                return 4;
            }
            case ShaderDataType::Int2: {
                return 8;
            }
            case ShaderDataType::Int3: {
                return 12;
            }
            case ShaderDataType::Int4: {
                return 16;
            }
            case ShaderDataType::Bool: {
                return 1;
            }
        }

        ENGINE_LOG_ERROR("Error Invalid Data Type!!!");

        std::unreachable();
        return 0;
    }

    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalize;

        BufferElement() noexcept = default;

        BufferElement(ShaderDataType type, const std::string& name, bool normalize = false) noexcept
            : Name{name}, Type{type}, Size{GetShaderDataTypeSize(type)}, Offset{0}, Normalize{normalize}{

        }

        constexpr inline uint32_t GetComponentCount() const noexcept {

            switch(Type){
                case ShaderDataType::Float: {
                    return 1;
                }
                case ShaderDataType::Float2: {
                    return 2;
                }
                case ShaderDataType::Float3: {
                    return 3;
                }
                case ShaderDataType::Float4: {
                    return 4;
                }
                case ShaderDataType::Mat3: {
                    return 3 * 3;
                }
                case ShaderDataType::Mat4: {
                    return 4 * 4;
                }
                case ShaderDataType::Int: {
                    return 1;
                }
                case ShaderDataType::Int2: {
                    return 2;
                }
                case ShaderDataType::Int3: {
                    return 3;
                }
                case ShaderDataType::Int4: {
                    return 4;
                }
                case ShaderDataType::Bool: {
                    return 1;
                }
            }

            ENGINE_LOG_ERROR("Error Invalid Data Type!!!");

            std::unreachable();
            return 0;
        }

        
    };

    class BufferLayout {
        public:
            BufferLayout() noexcept = default;
            BufferLayout(const std::initializer_list<BufferElement>& elements) noexcept : m_elements{elements} {
                calculateOffsetAndStride();
            }

            constexpr const inline std::vector<BufferElement>& GetElements() const noexcept {
                return m_elements;
            }

            constexpr inline decltype(auto) begin() noexcept
            {
                return m_elements.begin();
            }

            constexpr inline decltype(auto) end() noexcept
            {
                return m_elements.end();
            }

            constexpr inline decltype(auto) begin() const noexcept
            {
                return m_elements.cbegin();
            }

            constexpr inline decltype(auto) end() const noexcept
            {
                return m_elements.cend();
            }

            constexpr inline uint32_t GetStride() const noexcept {
                return m_stride;
            }

        private:
            void calculateOffsetAndStride() noexcept {
                uint32_t offset{0};
                m_stride = 0;

                for(auto&& element : m_elements) {
                    element.Offset = offset;
                    offset += element.Size;
                    m_stride += element.Size;
                }

                // {FLoat3, Position}
                // {Float4, Color}

                // Element 0:
                // Name: Position
                // Type: Float3
                // Size: 12     : size in byte
                // Offset: 0    : start of data in array
                // Stride Loop 1: 12

                // Element 1:
                // Name: Color
                // Type: Float4
                // Size: 16
                // Offset: 12
                // Stride Loop 2: 28    : final distance of same block data type in array
            }

            std::vector<BufferElement> m_elements;
            uint32_t m_stride;
    };

    class VertexBuffer{
        public:
            virtual ~VertexBuffer() noexcept = default;

            virtual void Bind() const noexcept = 0;

            virtual void UnBind() const noexcept = 0;
            
            virtual const BufferLayout& GetLayout() const noexcept = 0;
            virtual void SetLayout(const BufferLayout& layout) noexcept = 0;

            static VertexBuffer* Create(float* vertex, uint32_t size);
    };

    class IndexBuffer
    {
        public:
            virtual ~IndexBuffer() noexcept = default;

            virtual void Bind() const noexcept = 0;

            virtual void UnBind() const noexcept = 0;

            virtual uint32_t GetCount() const noexcept = 0;

            static IndexBuffer* Create(uint32_t* index, uint32_t size);
    };

    
}