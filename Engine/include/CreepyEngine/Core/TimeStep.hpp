#pragma once

namespace Creepy
{
    class TimeStep
    {
        public:
            constexpr TimeStep(float time = 0.0f) noexcept : m_time{time} {

            }

            [[nodiscard]] constexpr float GetSeconds() const noexcept {
                return m_time;
            }

            [[nodiscard]] constexpr float GetMilliseconds() const noexcept {
                return m_time * 1000.0f;
            }

        private:
           float m_time;
    };
}
