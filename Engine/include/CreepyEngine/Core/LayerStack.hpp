#pragma once

#include <vector>
#include <memory>
#include "Layer.hpp"

namespace Creepy
{

    class LayerStack
    {
        public:
            LayerStack() noexcept;
            ~LayerStack() noexcept;

            void PushLayer(Layer *layer) noexcept;
            void PushOverlay(Layer *overlay) noexcept;
            void PopLayer(Layer *layer) noexcept;
            void PopOverlay(Layer *overlay) noexcept;

            constexpr inline decltype(auto) begin() noexcept
            {
                return m_layers.begin();
            }

            constexpr inline decltype(auto) end() noexcept
            {
                return m_layers.end();
            }

            constexpr inline decltype(auto) rbegin() noexcept {
                return m_layers.rbegin();
            }

            constexpr inline decltype(auto) rend() noexcept {
                return m_layers.rend();
            }
            // void
        private:
            std::vector<Layer*> m_layers;
            size_t m_layerInsertIndex{0};
    };

}