#include <numeric>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Shader.hpp>
#include <CreepyEngine/Renderer/RenderCommand.hpp>
#include <CreepyEngine/Renderer/UniformBuffer.hpp>

#include <Platform/OpenGL/OpenGLShader.hpp>
#include <glm/glm.hpp>

namespace Creepy {

    struct RectVertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoord;
        float TextureIndex;
        float TilingFactor{1.0f};

        // Only For Editor
        int EntityID{-1};
    };

    struct CircleVertex{
        glm::vec3 Position;
        glm::vec2 LocalPosition;
        glm::vec4 Color;
        float Thickness;
        float Fade;

        int EntityID{-1};
    };

    struct LineVertex{
        glm::vec3 Position;
        glm::vec4 Color;

        int EntityID{-1};
    };

    struct Renderer2DStorage {
        const uint32_t MaxRects{10000};
        const uint32_t MaxVertex{MaxRects * 4};
        const uint32_t MaxIndex{MaxRects * 6};
        static const uint32_t MaxTextureSlots{32};     //TODO: Change Texture SLot To Asset Manager

        Ref<VertexArray> RectVertexArray;
        Ref<VertexBuffer> RectVertexBuffer;
        Ref<Shader> RectShader;
        Ref<Texture2D> WhiteTexture;

        Ref<VertexArray> CircleVertexArray;
        Ref<VertexBuffer> CircleVertexBuffer;
        Ref<Shader> CircleShader;

        Ref<VertexArray> LineVertexArray;
        Ref<VertexBuffer> LineVertexBuffer;
        Ref<Shader> LineShader;

        uint32_t RectIndexCount{0};     // use to keep track how many rect we need to draw
        RectVertex* RectVertexBufferBase{nullptr};
        RectVertex* RectVertexBufferPointer{nullptr};

        uint32_t CircleIndexCount{0};
        CircleVertex* CircleVertexBufferBase{nullptr};
        CircleVertex* CircleVertexBufferPointer{nullptr};

        uint32_t LineVertexCount{0};
        LineVertex* LineVertexBufferBase{nullptr};
        LineVertex* LineVertexBufferPointer{nullptr};

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex{1};   // 0: white texture

        glm::vec4 RectVertexPosition[4];

        Renderer2D::Statistics Stats;

        struct CameraData{
            glm::mat4 ViewProjection;
        };

        CameraData CameraBuffer;
        Ref<UniformBuffer> uniformBuffer;
    };

    static Renderer2DStorage s_renderer2dStorage;

    void Renderer2D::Init() noexcept {
        // Rect

        ENGINE_LOG_WARNING("Gona Create Vertex Arrray");
        s_renderer2dStorage.RectVertexArray = Creepy::VertexArray::Create();

        ENGINE_LOG_WARNING("Gona Create Vertex Buffer");
        s_renderer2dStorage.RectVertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxVertex * sizeof(RectVertex));

        Creepy::BufferLayout rectVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Float2, "a_textureCoord"},
            {Creepy::ShaderDataType::Float, "a_textureIndex"},
            {Creepy::ShaderDataType::Float, "a_tilingFactor"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        s_renderer2dStorage.RectVertexBuffer->SetLayout(rectVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        s_renderer2dStorage.RectVertexArray->AddVertexBuffer(s_renderer2dStorage.RectVertexBuffer);

        s_renderer2dStorage.RectVertexBufferBase = new RectVertex[s_renderer2dStorage.MaxVertex];

        // Because alloc too much index on stack may cause stack overflow so we alloc on heap

        uint32_t* rectIndex = new uint32_t[s_renderer2dStorage.MaxIndex];

        uint32_t offset{0};
        for (uint32_t i{}; i < s_renderer2dStorage.MaxIndex; i += 6)
        {

            rectIndex[i + 0] = offset + 0;
            rectIndex[i + 1] = offset + 1;
            rectIndex[i + 2] = offset + 2;

            rectIndex[i + 3] = offset + 2;
            rectIndex[i + 4] = offset + 3;
            rectIndex[i + 5] = offset + 0;

            offset += 4;
        }

        ENGINE_LOG_WARNING("Gona Create Index");
        auto indexBuffer = Creepy::IndexBuffer::Create(rectIndex, s_renderer2dStorage.MaxIndex);

        s_renderer2dStorage.RectVertexArray->SetIndexBuffer(indexBuffer);


        // Circle
        {
            s_renderer2dStorage.CircleVertexArray = VertexArray::Create();

            s_renderer2dStorage.CircleVertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxVertex * sizeof(CircleVertex));

            Creepy::BufferLayout circleVertexBufferLayout{
                {Creepy::ShaderDataType::Float3, "a_position"},
                {Creepy::ShaderDataType::Float2, "a_localPosition"},
                {Creepy::ShaderDataType::Float4, "a_color"},
                {Creepy::ShaderDataType::Float, "a_thickness"},
                {Creepy::ShaderDataType::Float, "a_fade"},
                {Creepy::ShaderDataType::Int, "a_entityID"},
            };

            s_renderer2dStorage.CircleVertexBuffer->SetLayout(circleVertexBufferLayout);
            
            // We need add buffer after it add layout, if not it will empty
            s_renderer2dStorage.CircleVertexArray->AddVertexBuffer(s_renderer2dStorage.CircleVertexBuffer);

            s_renderer2dStorage.CircleVertexArray->SetIndexBuffer(indexBuffer); // rectIndex
            
            s_renderer2dStorage.CircleVertexBufferBase = new CircleVertex[s_renderer2dStorage.MaxVertex];

        }

        // Lines
        {
            s_renderer2dStorage.LineVertexArray = VertexArray::Create();

            s_renderer2dStorage.LineVertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxVertex * sizeof(LineVertex));

            Creepy::BufferLayout lineVertexBufferLayout{
                {Creepy::ShaderDataType::Float3, "a_position"},
                {Creepy::ShaderDataType::Float4, "a_color"},
                {Creepy::ShaderDataType::Int, "a_entityID"},
            };

            s_renderer2dStorage.LineVertexBuffer->SetLayout(lineVertexBufferLayout);
            
            // We need add buffer after it add layout, if not it will empty
            s_renderer2dStorage.LineVertexArray->AddVertexBuffer(s_renderer2dStorage.LineVertexBuffer);
            
            s_renderer2dStorage.LineVertexBufferBase = new LineVertex[s_renderer2dStorage.MaxVertex];
        }

        delete[] rectIndex;
        rectIndex = nullptr;


        ENGINE_LOG_WARNING("Gona Create White Texture");
        s_renderer2dStorage.WhiteTexture = Texture2D::Create(1, 1);

        uint32_t whiteTextureData = 0xffffffff;

        s_renderer2dStorage.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        
        ENGINE_LOG_WARNING("Gona Create Shader");
        s_renderer2dStorage.RectShader = Shader::Create("./assets/shaders/RectVertexShader.glsl", "./assets/shaders/RectFragmentShader.glsl");

        s_renderer2dStorage.CircleShader = Shader::Create("./assets/shaders/CircleVertexShader.glsl", "./assets/shaders/CircleFragmentShader.glsl");
        
        s_renderer2dStorage.LineShader = Shader::Create("./assets/shaders/LineVertexShader.glsl", "./assets/shaders/LineFragmentShader.glsl");

        // std::array<int, s_renderer2dStorage.MaxTextureSlots> samplers;

        // std::ranges::iota(samplers, 0);

        // We need to init default texture unit
        // s_renderer2dStorage.RectShader->SetUniformIntArray("u_textures", samplers);

        // Set default slot 0 for white texture
        s_renderer2dStorage.TextureSlots[0] = s_renderer2dStorage.WhiteTexture;

        s_renderer2dStorage.RectVertexPosition[0] = {-0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[1] = { 0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[2] = { 0.5,  0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[3] = {-0.5,  0.5, 0.0f, 1.0f};

        s_renderer2dStorage.uniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorage::CameraData), 0);
    }

    void Renderer2D::ShutDown() noexcept {
        
        delete[] s_renderer2dStorage.RectVertexBufferBase;
        delete[] s_renderer2dStorage.CircleVertexBufferBase;
        delete[] s_renderer2dStorage.LineVertexBufferBase;

        s_renderer2dStorage.RectVertexArray.reset();
        s_renderer2dStorage.RectVertexBuffer.reset();
        s_renderer2dStorage.RectShader.reset();

        s_renderer2dStorage.CircleVertexArray.reset();
        s_renderer2dStorage.CircleVertexBuffer.reset();
        s_renderer2dStorage.CircleShader.reset();

        s_renderer2dStorage.LineVertexArray.reset();
        s_renderer2dStorage.LineVertexBuffer.reset();
        s_renderer2dStorage.LineShader.reset();

        s_renderer2dStorage.WhiteTexture.reset();
    }

    

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) noexcept {

        glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

        s_renderer2dStorage.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        s_renderer2dStorage.uniformBuffer->SetData(&s_renderer2dStorage.CameraBuffer, sizeof(s_renderer2dStorage.CameraBuffer));

        // Reset
        startBatch();
    }

    void Renderer2D::BeginScene(const EditorCamera& camera) noexcept {
        glm::mat4 viewProjection = camera.GetViewProjectionMatrix();

        s_renderer2dStorage.CameraBuffer.ViewProjection = viewProjection;
        s_renderer2dStorage.uniformBuffer->SetData(&s_renderer2dStorage.CameraBuffer, sizeof(s_renderer2dStorage.CameraBuffer));

        startBatch();
    }

    void Renderer2D::EndScene() noexcept {
        Flush();
    }

    void Renderer2D::Flush() noexcept {

        if(s_renderer2dStorage.RectIndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(s_renderer2dStorage.RectVertexBufferPointer) - reinterpret_cast<uint8_t*>(s_renderer2dStorage.RectVertexBufferBase);
        
            s_renderer2dStorage.RectVertexBuffer->SetData(s_renderer2dStorage.RectVertexBufferBase, dataSize);
            
            // Bind All Texture
            for(uint32_t i{0}; i < s_renderer2dStorage.TextureSlotIndex; i++){
                s_renderer2dStorage.TextureSlots[i]->Bind(i);
            }

            s_renderer2dStorage.RectShader->Bind();
            RenderCommand::DrawIndex(s_renderer2dStorage.RectVertexArray, s_renderer2dStorage.RectIndexCount);
            ++s_renderer2dStorage.Stats.DrawCalls;
        }

        if(s_renderer2dStorage.CircleIndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(s_renderer2dStorage.CircleVertexBufferPointer) - reinterpret_cast<uint8_t*>(s_renderer2dStorage.CircleVertexBufferBase);
        
            s_renderer2dStorage.CircleVertexBuffer->SetData(s_renderer2dStorage.CircleVertexBufferBase, dataSize);
        
            s_renderer2dStorage.CircleShader->Bind();
            RenderCommand::DrawIndex(s_renderer2dStorage.CircleVertexArray, s_renderer2dStorage.CircleIndexCount);
            ++s_renderer2dStorage.Stats.DrawCalls;
        }
        
        if(s_renderer2dStorage.LineVertexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(s_renderer2dStorage.LineVertexBufferPointer) - reinterpret_cast<uint8_t*>(s_renderer2dStorage.LineVertexBufferBase);
        
            s_renderer2dStorage.LineVertexBuffer->SetData(s_renderer2dStorage.LineVertexBufferBase, dataSize);
        
            s_renderer2dStorage.LineShader->Bind();
            RenderCommand::DrawLines(s_renderer2dStorage.LineVertexArray, s_renderer2dStorage.LineVertexCount);
            ++s_renderer2dStorage.Stats.DrawCalls;
        }

    }

    void Renderer2D::startBatch() noexcept{
        s_renderer2dStorage.RectIndexCount = 0;
        s_renderer2dStorage.RectVertexBufferPointer = s_renderer2dStorage.RectVertexBufferBase;

        s_renderer2dStorage.CircleIndexCount = 0;
        s_renderer2dStorage.CircleVertexBufferPointer = s_renderer2dStorage.CircleVertexBufferBase;

        s_renderer2dStorage.LineVertexCount = 0;
        s_renderer2dStorage.LineVertexBufferPointer = s_renderer2dStorage.LineVertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer2D::flushAndReset() noexcept {

        EndScene();

        startBatch();
    }

    void Renderer2D::setRectProperty(const glm::mat4& transform, const glm::vec4& color, const std::array<glm::vec2, 4>& textureCoords, float textureIndex, float tilingFactor, int entityID) noexcept {
        
        for(size_t i{0}; i < 4; i++){
            
            s_renderer2dStorage.RectVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.RectVertexBufferPointer->Color = color;
            s_renderer2dStorage.RectVertexBufferPointer->TextureCoord = textureCoords.at(i);
            s_renderer2dStorage.RectVertexBufferPointer->TextureIndex = textureIndex;
            s_renderer2dStorage.RectVertexBufferPointer->TilingFactor = tilingFactor;
            s_renderer2dStorage.RectVertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.RectVertexBufferPointer++;
        }

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

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

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

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

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

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }

    void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, float tilingFactor, int entityID) noexcept {

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

        setRectProperty(transform, color, textureCoords, textureIndex, tilingFactor, entityID);
    }

    void Renderer2D::DrawRect(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tilingColor, float tilingFactor, int entityID) noexcept {
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

        setRectProperty(transform, tilingColor, textureCoords, textureIndex, tilingFactor, entityID);
    }

    void Renderer2D::DrawSprite(TransformComponent& transform, SpriteComponent& sprite, int entityID) noexcept {
        if(sprite.Texture){
            DrawRect(transform.GetTransform(), sprite.Texture, sprite.Color, sprite.TilingFactor, entityID);
        }
        DrawRect(transform.GetTransform(), sprite.Color, sprite.TilingFactor, entityID);
    }

    void Renderer2D::DrawCircle(TransformComponent& transform, CircleSpriteComponent& circle, int entityID) noexcept {
        if(s_renderer2dStorage.CircleIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        for(uint32_t i{}; i < 4; i++){

            s_renderer2dStorage.CircleVertexBufferPointer->Position = transform.GetTransform() * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.CircleVertexBufferPointer->LocalPosition = s_renderer2dStorage.RectVertexPosition[i] * 2.0f;
            s_renderer2dStorage.CircleVertexBufferPointer->Color = circle.Color;
            s_renderer2dStorage.CircleVertexBufferPointer->Thickness = circle.Thickness;
            s_renderer2dStorage.CircleVertexBufferPointer->Fade = circle.Fade;
            s_renderer2dStorage.CircleVertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.CircleVertexBufferPointer++;
        }

        s_renderer2dStorage.CircleIndexCount += 6;

    }

    void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickNess, float fade, int entityID) noexcept {
        if(s_renderer2dStorage.CircleIndexCount >= s_renderer2dStorage.MaxIndex){
            flushAndReset();
        }

        for(uint32_t i{}; i < 4; i++){

            s_renderer2dStorage.CircleVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.CircleVertexBufferPointer->LocalPosition = s_renderer2dStorage.RectVertexPosition[i] * 2.0f;
            s_renderer2dStorage.CircleVertexBufferPointer->Color = color;
            s_renderer2dStorage.CircleVertexBufferPointer->Thickness = thickNess;
            s_renderer2dStorage.CircleVertexBufferPointer->Fade = fade;
            s_renderer2dStorage.CircleVertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.CircleVertexBufferPointer++;
        }

        s_renderer2dStorage.CircleIndexCount += 6;
    }

    void Renderer2D::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID) noexcept {

        s_renderer2dStorage.LineVertexBufferPointer->Position = start;
        s_renderer2dStorage.LineVertexBufferPointer->Color = color;
        s_renderer2dStorage.LineVertexBufferPointer->EntityID = entityID;
        s_renderer2dStorage.LineVertexBufferPointer++;

        s_renderer2dStorage.LineVertexBufferPointer->Position = end;
        s_renderer2dStorage.LineVertexBufferPointer->Color = color;
        s_renderer2dStorage.LineVertexBufferPointer->EntityID = entityID;
        s_renderer2dStorage.LineVertexBufferPointer++;

        s_renderer2dStorage.LineVertexCount += 2;
    }


    void Renderer2D::DrawLineRect(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, int entityID) noexcept {

        glm::vec3 p0 = glm::vec3{pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, pos.z};
        glm::vec3 p1 = glm::vec3{pos.x + size.x * 0.5f, pos.y - size.y * 0.5f, pos.z};
        glm::vec3 p2 = glm::vec3{pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, pos.z};
        glm::vec3 p3 = glm::vec3{pos.x - size.x * 0.5f, pos.y + size.y * 0.5f, pos.z};

        DrawLine(p0, p1, color, entityID);
        DrawLine(p1, p2, color, entityID);
        DrawLine(p2, p3, color, entityID);
        DrawLine(p3, p0, color, entityID);

    }
            
    void Renderer2D::DrawLineRect(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        glm::vec3 lineVertex[4];

        for(size_t i{}; i < 4; i++){
            lineVertex[i] = transform * s_renderer2dStorage.RectVertexPosition[i];
        }

        DrawLine(lineVertex[0], lineVertex[1], color, entityID);
        DrawLine(lineVertex[1], lineVertex[2], color, entityID);
        DrawLine(lineVertex[2], lineVertex[3], color, entityID);
        DrawLine(lineVertex[3], lineVertex[0], color, entityID);
    }


    void Renderer2D::ResetStatistics() noexcept {
        s_renderer2dStorage.Stats.DrawCalls = 0;
        s_renderer2dStorage.Stats.RectCount = 0;
    }

    Renderer2D::Statistics Renderer2D::GetStatistics() noexcept {
        return s_renderer2dStorage.Stats;
    }

}