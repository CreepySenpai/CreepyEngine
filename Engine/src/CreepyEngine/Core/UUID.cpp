#include <random>
#include <CreepyEngine/Core/UUID.hpp>

namespace Creepy {

    // TODO: If use multithreading maybe change to thread local or sync
    static std::random_device RandomDevice;
    static std::mt19937_64 RandomEngine{RandomDevice()};
    static std::uniform_int_distribution<uint64_t> UniformDistribution;

    UUID::UUID() noexcept : m_uuid{UniformDistribution(RandomEngine)} {

    }

    UUID::UUID(uint64_t uuid) noexcept : m_uuid{uuid} {
        
    }

}