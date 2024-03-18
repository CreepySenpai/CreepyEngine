#include <numeric>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Shader.hpp>
#include <CreepyEngine/Renderer/RenderCommand.hpp>

#include <Platform/OpenGL/OpenGLShader.hpp>
#include <glm/glm.hpp>

namespace Creepy {

    struct RectVertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoord;
        float TextureIndex;
        // TODO: Add texture...
    };

    struct Renderer2DStorage {
        const uint32_t MaxRects{10000};
        const uint32_t MaxVertex{MaxRects * 4};
        const uint32_t MaxIndex{MaxRects * 6};
        static const uint32_t MaxTextureSlots{32};     //TODO: Change Texture SLot To Asset Manager

        Ref<VertexArray> vertexArray;
        Ref<VertexBuffer> vertexBuffer;
        Ref<Shader> shader;
        Ref<Texture2D> whiteTexture;

        uint32_t RectIndexCount{0};     // use to keep track how many rect we need to draw
        RectVertex* RectVertexBufferBase{nullptr};
        RectVertex* RectVertexBufferPointer{nullptr};

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex{1};   // 0: white texture

        glm::vec4 RectVertexPosition[4];

        Renderer2D::Statistics Stats;
    };

    static Renderer2DStorage s_renderer2dStorage;

    


    void Renderer2D::Init() noexcept {

        s_renderer2dStorage.vertexArray = Creepy::VertexArray::Create();

        s_renderer2dStorage.vertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxVertex * sizeof(RectVertex));

        Creepy::BufferLayout vertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Float2, "a_textureCoord"},
            {Creepy::ShaderDataType::Float, "a_textureIndex"},
        };

        s_renderer2dStorage.vertexBuffer->SetLayout(vertexBufferLayout);
        
        // We need add buffer after it add layout, if not it will empty
        s_renderer2dStorage.vertexArray->AddVertexBuffer(s_renderer2dStorage.vertexBuffer);

        
        s_renderer2dStorage.RectVertexBufferBase = new RectVertex[s_renderer2dStorage.MaxVertex];
        

        // Because alloc too much index on stack may cause stack overflow so we alloc on heap
        uint32_t* rectIndex = new uint32_t[s_renderer2dStorage.MaxIndex];

        uint32_t offset{0};
        for(uint32_t i{}; i < s_renderer2dStorage.MaxIndex; i += 6){

            rectIndex[i + 0] = offset + 0;
            rectIndex[i + 1] = offset + 1;
            rectIndex[i + 2] = offset + 2;

            rectIndex[i + 3] = offset + 2;
            rectIndex[i + 4] = offset + 3;
            rectIndex[i + 5] = offset + 0;

            offset += 4;
        }

        auto indexBuffer = Creepy::IndexBuffer::Create(rectIndex, s_renderer2dStorage.MaxIndex);

        s_renderer2dStorage.vertexArray->SetIndexBuffer(indexBuffer);

        // WARING: May cause bug be cause upload data to GPU may not don't and we delete it data
        delete[] rectIndex;
        rectIndex = nullptr;


        s_renderer2dStorage.whiteTexture = Texture2D::Create(1, 1);

        uint32_t whiteTextureData = 0xffffffff;

        s_renderer2dStorage.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_renderer2dStorage.shader = Shader::Create("./assets/shaders/VertexShader.glsl", "./assets/shaders/FragmentShader.glsl");

        s_renderer2dStorage.shader->Bind();
        
        std::array<int, s_renderer2dStorage.MaxTextureSlots> samplers;

        std::ranges::iota(samplers, 0);

        // We need to init default texture unit
        s_renderer2dStorage.shader->SetUniformIntArray("u_textures", samplers);

        // Set default slot 0 for white texture
        s_renderer2dStorage.TextureSlots[0] = s_renderer2dStorage.whiteTexture;

        s_renderer2dStorage.RectVertexPosition[0] = {-0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[1] = { 0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[2] = { 0.5,  0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[3] = {-0.5,  0.5, 0.0f, 1.0f};
    }

    void Renderer2D::ShutDown() noexcept {
        
        delete[] s_renderer2dStorage.RectVertexBufferBase;
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) noexcept {

        glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);
        
        s_renderer2dStorage.shader->Bind();

        s_renderer2dStorage.shader->SetUniformMat4("u_viewProjectionMatrix", viewProjection);

        // Reset
        s_renderer2dStorage.RectIndexCount = 0;
        s_renderer2dStorage.RectVertexBufferPointer = s_renderer2dStorage.RectVertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const EditorCamera& camera) noexcept {
        glm::mat4 viewProjection = camera.GetViewProjectionMatrix();
        
        s_renderer2dStorage.shader->Bind();

        s_renderer2dStorage.shader->SetUniformMat4("u_viewProjectionMatrix", viewProjection);

        // Reset
        s_renderer2dStorage.RectIndexCount = 0;
        s_renderer2dStorage.RectVertexBufferPointer = s_renderer2dStorage.RectVertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) noexcept {

        s_renderer2dStorage.shader->Bind();

        s_renderer2dStorage.shader->SetUniformMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());

        // Reset
        s_renderer2dStorage.RectIndexCount = 0;
        s_renderer2dStorage.RectVertexBufferPointer = s_renderer2dStorage.RectVertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene() noexcept {
        
        // Get Data Size In Byte
        uint32_t dataSize = reinterpret_cast<uint8_t*>(s_renderer2dStorage.RectVertexBufferPointer) - reinterpret_cast<uint8_t*>(s_renderer2dStorage.RectVertexBufferBase);
        
        s_renderer2dStorage.vertexBuffer->SetData(s_renderer2dStorage.RectVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush() noexcept {
        // Bind All Texture
        for(uint32_t i{0}; i < s_renderer2dStorage.TextureSlotIndex; i++){
            s_renderer2dStorage.TextureSlots[i]->Bind(i);
        }

        RenderCommand::DrawIndex(s_renderer2dStorage.vertexArray, s_renderer2dStorage.RectIndexCount);

        ++s_renderer2dStorage.Stats.DrawCalls;
        
    }

    void Renderer2D::flushAndReset() noexcept {

        EndScene();

        s_renderer2dStorage.RectIndexCount = 0;
        s_renderer2dStorage.RectVertexBufferPointer = s_renderer2dStorage.RectVertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer2D::setRectProperty(const glm::mat4& transform, const glm::vec4& color, const std::array<glm::vec2, 4>& textureCoords, float textureIndex) noexcept {

        s_renderer2dStorage.RectVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[0];
        s_renderer2dStorage.RectVertexBufferPointer->Color = color;
        s_renderer2dStorage.RectVertexBufferPointer->TextureCoord = textureCoords.at(0);
        s_renderer2dStorage.RectVertexBufferPointer->TextureIndex = textureIndex;

        s_renderer2dStorage.RectVertexBufferPointer++;

        s_renderer2dStorage.RectVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[1];
        s_renderer2dStorage.RectVertexBufferPointer->Color = color;
        s_renderer2dStorage.RectVertexBufferPointer->TextureCoord = textureCoords.at(1);
        s_renderer2dStorage.RectVertexBufferPointer->TextureIndex = textureIndex;

        s_renderer2dStorage.RectVertexBufferPointer++;

        s_renderer2dStorage.RectVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[2];
        s_renderer2dStorage.RectVertexBufferPointer->Color = color;
        s_renderer2dStorage.RectVertexBufferPointer->TextureCoord = textureCoords.at(2);
        s_renderer2dStorage.RectVertexBufferPointer->TextureIndex = textureIndex;

        s_renderer2dStorage.RectVertexBufferPointer++;

        s_renderer2dStorage.RectVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[3];
        s_renderer2dStorage.RectVertexBufferPointer->Color = color;
        s_renderer2dStorage.RectVertexBufferPointer->TextureCoord = textureCoords.at(3);
        s_renderer2dStorage.RectVertexBufferPointer->TextureIndex = textureIndex;

        s_renderer2dStorage.RectVertexBufferPointer++;

        s_renderer2dStorage.RectIndexCount += 6;


        ++s_renderer2dStorage.Stats.RectCount;
    }

    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {
        DrawRect({position.x, position.y, 0.0f}, size, color);
    }
    
    void Renderer2D::DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        constexpr float textureIndex{0.0f};

        setRectProperty(transform, color, textureCoords, textureIndex);
    }

    void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {
        DrawRect(glm::vec3(position.x, position.y, 0.0f), size, texture, tilingColor);
    }

    void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        // We check if texture already exit on array
        float textureIndex{0.0f};
        
        for(uint32_t i{1}; i < s_renderer2dStorage.TextureSlotIndex; i++){
            if(*s_renderer2dStorage.TextureSlots[i].get() == *texture.get()){
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        // If it not exit we add texture to texture array
        if(std::is_eq(textureIndex <=> 0.0f)){

            if(s_renderer2dStorage.TextureSlotIndex >= s_renderer2dStorage.MaxTextureSlots){
                flushAndReset();
            }

            textureIndex = static_cast<float>(s_renderer2dStorage.TextureSlotIndex);

            s_renderer2dStorage.TextureSlots[s_renderer2dStorage.TextureSlotIndex] = texture;

            ++s_renderer2dStorage.TextureSlotIndex;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }

    void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor) noexcept {
        DrawRect(glm::vec3(position.x, position.y, 0.0f), size, subTexture, tilingColor);
    }

    void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        const auto& textureCoords = subTexture->GetTextureCoords();

        const auto& texture = subTexture->GetTexture();

        // We check if texture already exit on array
        float textureIndex{0.0f};
        
        for(uint32_t i{1}; i < s_renderer2dStorage.TextureSlotIndex; i++){

            if(*s_renderer2dStorage.TextureSlots[i].get() == *texture.get()){
                textureIndex = static_cast<float>(i);
                break;
            }

        }

        // If it not exit we add texture to texture array
        if(std::is_eq(textureIndex <=> 0.0f)){

            if(s_renderer2dStorage.TextureSlotIndex >= s_renderer2dStorage.MaxTextureSlots){
                flushAndReset();
            }

            textureIndex = static_cast<float>(s_renderer2dStorage.TextureSlotIndex);

            s_renderer2dStorage.TextureSlots[s_renderer2dStorage.TextureSlotIndex] = texture;

            ++s_renderer2dStorage.TextureSlotIndex;

        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }

    void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color) noexcept {

        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        constexpr float textureIndex{0.0f};

        setRectProperty(transform, color, textureCoords, textureIndex);
    }

    void Renderer2D::DrawRect(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        // We check if texture already exit on array
        float textureIndex{0.0f};
        
        for(uint32_t i{1}; i < s_renderer2dStorage.TextureSlotIndex; i++){
            if(*s_renderer2dStorage.TextureSlots[i].get() == *texture.get()){
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        // If it not exit we add texture to texture array
        if(std::is_eq(textureIndex <=> 0.0f)){
            
            if(s_renderer2dStorage.TextureSlotIndex >= s_renderer2dStorage.MaxTextureSlots){
                flushAndReset();
            }

            textureIndex = static_cast<float>(s_renderer2dStorage.TextureSlotIndex);

            s_renderer2dStorage.TextureSlots[s_renderer2dStorage.TextureSlotIndex] = texture;

            ++s_renderer2dStorage.TextureSlotIndex;
        }

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }

    void Renderer2D::DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) noexcept {

        DrawRotRect({position.x, position.y, 0.0f}, size, rotation, color);

    }
    void Renderer2D::DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::rotate(transform, rotation, {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        constexpr float textureIndex{0.0f};

        setRectProperty(transform, color, textureCoords, textureIndex);
    }
 
    void Renderer2D::DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {

        DrawRotRect(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture, tilingColor);

    }
    void Renderer2D::DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {
        
        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        // We check if texture already exit on array
        float textureIndex{0.0f};
        
        for(uint32_t i{1}; i < s_renderer2dStorage.TextureSlotIndex; i++){
            if(*s_renderer2dStorage.TextureSlots[i].get() == *texture.get()){
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        // If it not exit we add texture to texture array
        if(std::is_eq(textureIndex <=> 0.0f)){
            
            if(s_renderer2dStorage.TextureSlotIndex >= s_renderer2dStorage.MaxTextureSlots){
                flushAndReset();
            }

            textureIndex = static_cast<float>(s_renderer2dStorage.TextureSlotIndex);

            s_renderer2dStorage.TextureSlots[s_renderer2dStorage.TextureSlotIndex] = texture;

            ++s_renderer2dStorage.TextureSlotIndex;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::rotate(transform, rotation, {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }


    void Renderer2D::DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor) noexcept {

        DrawRotRect(glm::vec3(position.x, position.y, 0.0f), size, rotation, subTexture, tilingColor);

    }
    void Renderer2D::DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor) noexcept {
        
        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        if(s_renderer2dStorage.RectIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        const auto& textureCoords = subTexture->GetTextureCoords();

        const auto& texture = subTexture->GetTexture();

        // We check if texture already exit on array
        float textureIndex{0.0f};
        
        for(uint32_t i{1}; i < s_renderer2dStorage.TextureSlotIndex; i++){
            if(*s_renderer2dStorage.TextureSlots[i].get() == *texture.get()){
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        // If it not exit we add texture to texture array
        if(std::is_eq(textureIndex <=> 0.0f)){
            
            if(s_renderer2dStorage.TextureSlotIndex >= s_renderer2dStorage.MaxTextureSlots){
                flushAndReset();
            }

            textureIndex = static_cast<float>(s_renderer2dStorage.TextureSlotIndex);

            s_renderer2dStorage.TextureSlots[s_renderer2dStorage.TextureSlotIndex] = texture;

            ++s_renderer2dStorage.TextureSlotIndex;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::rotate(transform, rotation, {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }


    void Renderer2D::ResetStatistics() noexcept {
        s_renderer2dStorage.Stats.DrawCalls = 0;
        s_renderer2dStorage.Stats.RectCount = 0;
    }

    Renderer2D::Statistics Renderer2D::GetStatistics() noexcept {
        return s_renderer2dStorage.Stats;
    }

}