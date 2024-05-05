#include <ranges>
#include <CreepyEngine/Core/LayerStack.hpp>

namespace Creepy
{

    LayerStack::LayerStack() noexcept
    {
        
    }

    LayerStack::~LayerStack() noexcept
    {
        for (auto&& layer : m_layers)
        {

            layer->OnDetach();
            delete layer;
            layer = nullptr;
            
        }
    }

    // We alway want push layer on top of overlay so we kep a position where last layer was push and continue push to next position
    void LayerStack::PushLayer(Layer *layer) noexcept
    {
        m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
        ++m_layerInsertIndex;
    }

    void LayerStack::PushOverlay(Layer *overlay) noexcept
    {
        m_layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer *layer) noexcept
    {
        const auto&& needDelete = std::ranges::find(m_layers, layer);

        if(needDelete != m_layers.cend())
        {
            m_layers.erase(needDelete);
            --m_layerInsertIndex;
        }
    }

    void LayerStack::PopOverlay(Layer *overlay) noexcept
    {
        const auto&& needDelete = std::ranges::find(m_layers, overlay);

        if(needDelete != m_layers.cend())
        {
            m_layers.erase(needDelete);
        }
    }

}