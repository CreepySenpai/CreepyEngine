#pragma once

#include <cstdint>
#include <functional>

namespace Creepy {

    class UUID
    {
        public:
            UUID() noexcept;
            UUID(uint64_t uuid) noexcept;

            constexpr uint64_t GetID() const noexcept {
                return m_uuid;
            }

        private:
            uint64_t m_uuid;
    };
}

namespace std {
    template <>
    struct hash<Creepy::UUID> {
        size_t operator()(const Creepy::UUID& uuid){
            return hash<uint64_t>()(uuid.GetID());
        }
    };
}