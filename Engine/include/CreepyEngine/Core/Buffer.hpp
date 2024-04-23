#pragma once

#include <cstdint>
#include <cstring>
#include <CreepyEngine/Utils/TypesUtils.hpp>

namespace Creepy{

    struct [[nodiscard]] Buffer
    {
        public:
            // TODO: Rule of 5
            constexpr Buffer() noexcept = default;

            constexpr Buffer(uint64_t size) noexcept {
                Allocate(size);
            }

            constexpr void Allocate(uint64_t size) noexcept {

                Release();

                m_buffer = new uint8_t[size];
                m_size = size;
            }

            constexpr void Release() noexcept {
                delete[] m_buffer;
                m_buffer = nullptr;
                m_size = 0;
            }

            constexpr uint64_t Size() const noexcept {
                return m_size;
            }

            constexpr uint8_t* Data() noexcept {
                return m_buffer;
            }

            template <typename T>
            constexpr T* As() const noexcept {
                return reinterpret_cast<T*>(m_buffer);
            }

            constexpr operator bool() const noexcept {
                return (m_buffer != nullptr);
            }

            // TODO: Change memcpy to another func
            static constexpr Buffer Copy(Buffer& other) noexcept {
                Buffer result{other.Size()};
                std::memcpy(result.Data(), other.Data(), other.Size()); // Deep Copy
                return result;
            }

        private:
            uint8_t* m_buffer{nullptr};
            uint64_t m_size{0};
    };


    struct [[nodiscard]] ScopedBuffer
    {
        public:

            constexpr ScopedBuffer() noexcept = default;

            constexpr ScopedBuffer(const Buffer& other) noexcept : m_buffer{other}{}
            
            constexpr ScopedBuffer(uint64_t size) noexcept : m_buffer{size} {}

            constexpr ~ScopedBuffer() noexcept {
                m_buffer.Release();
            }

            template <typename T>
            constexpr T* As() noexcept {
                return m_buffer.As<T>();
            }

            constexpr uint64_t Size() const noexcept {
                return m_buffer.Size();
            }

            constexpr operator bool() noexcept {
                return (m_buffer.Data() != nullptr);
            }

            constexpr operator bool() const noexcept {
                return m_buffer;
            }

        private:
           Buffer m_buffer;
    };
    

}