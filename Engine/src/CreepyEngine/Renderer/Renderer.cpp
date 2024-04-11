#include <numeric>

#include <CreepyEngine/Renderer/Renderer.hpp>
#include <CreepyEngine/Renderer/Camera.hpp>
#include <CreepyEngine/Renderer/Texture.hpp>
#include <CreepyEngine/Renderer/SubTexture2D.hpp>
#include <CreepyEngine/Renderer/EditorCamera.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Shader.hpp>
#include <CreepyEngine/Renderer/RenderCommand.hpp>
#include <CreepyEngine/Renderer/UniformBuffer.hpp>
#include <CreepyEngine/Renderer/Model.hpp>
#include <CreepyEngine/Utils/ModelImporterUtils.hpp>

#include <Platform/OpenGL/OpenGLShader.hpp>

namespace Creepy {

    // MARK: Static Storage
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

    struct ModelVertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec3 Normal;
        glm::vec2 TextureCoord;
        float TextureIndex;

        int EntityID{-1};
    };

    struct RendererCoreStorage{

        Renderer::Statistics Stats;

        struct CameraData{
            glm::mat4 ViewProjection;
        };

        CameraData CameraBuffer;
        Ref<UniformBuffer> UniformBuffer;
    };

    struct Renderer2DStorage {
        const uint32_t MaxRects{10000u};
        const uint32_t MaxRectVertex{MaxRects * 4};
        const uint32_t MaxRectIndex{MaxRects * 6};
        static const uint32_t MaxTextureSlots{32};     //TODO: Change Texture SLot To Asset Manager

        struct RectData{
            Ref<VertexArray> RectVertexArray;
            Ref<VertexBuffer> RectVertexBuffer;
            Ref<Shader> RectShader;
            uint32_t RectIndexCount{0};     // use to keep track how many rect we need to draw
            RectVertex* RectVertexBufferBase{nullptr};
            RectVertex* RectVertexBufferPointer{nullptr};
        };

        RectData Rect;

        struct CircleData {
            Ref<VertexArray> CircleVertexArray;
            Ref<VertexBuffer> CircleVertexBuffer;
            Ref<Shader> CircleShader;
            uint32_t CircleIndexCount{0};
            CircleVertex* CircleVertexBufferBase{nullptr};
            CircleVertex* CircleVertexBufferPointer{nullptr};
        };

        CircleData Circle;

        struct LinesData {
            Ref<VertexArray> LineVertexArray;
            Ref<VertexBuffer> LineVertexBuffer;
            Ref<Shader> LineShader;
            uint32_t LineVertexCount{0};
            LineVertex* LineVertexBufferBase{nullptr};
            LineVertex* LineVertexBufferPointer{nullptr};
        };

        LinesData Lines;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex{1};   // 0: white texture

        glm::vec4 RectVertexPosition[4];
        Ref<Texture2D> WhiteTexture{nullptr};
    };

    struct Renderer3DStorage {
        // TODO: Increase Max Cube
        const uint32_t MaxModels{1000u};

        struct CubeData {

            const uint32_t MaxCubesVertices{1000u * 36};
            const uint32_t MaxCubesIndices{1000u * 36};

            uint32_t CubeIndexCount{0};
            ModelVertex* CubeVertexBufferBase;
            ModelVertex* CubeVertexBufferPointer;
            Ref<VertexArray> CubeVertexArray;
            Ref<VertexBuffer> CubeVertexBuffer;
            Ref<Shader> CubeShader;
            Ref<Mesh> CubeMesh;
        };

        CubeData Cube;
    };

    static RendererCoreStorage s_rendererCoreStorage;

    static Renderer2DStorage s_renderer2dStorage;

    static Renderer3DStorage s_renderer3dStorage;

    void Renderer::WindowResize(uint32_t width, uint32_t height) noexcept {
        RenderCommand::SetViewPort(0, 0, width, height);
    }

    // MARK: Set up storage
    void initCoreStorage() noexcept {
        s_rendererCoreStorage.UniformBuffer = UniformBuffer::Create(sizeof(RendererCoreStorage::CameraData), 0);
    }

    void initRect() noexcept {
        auto&& Rect = s_renderer2dStorage.Rect;
        ENGINE_LOG_WARNING("Gona Create Vertex Arrray");
        Rect.RectVertexArray = Creepy::VertexArray::Create();

        ENGINE_LOG_WARNING("Gona Create Vertex Buffer");
        Rect.RectVertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxRectVertex * sizeof(RectVertex));

        Creepy::BufferLayout rectVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Float2, "a_textureCoord"},
            {Creepy::ShaderDataType::Float, "a_textureIndex"},
            {Creepy::ShaderDataType::Float, "a_tilingFactor"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        Rect.RectVertexBuffer->SetLayout(rectVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        Rect.RectVertexArray->AddVertexBuffer(Rect.RectVertexBuffer);

        Rect.RectVertexBufferBase = new RectVertex[s_renderer2dStorage.MaxRectVertex];

        // Because alloc too much index on stack may cause stack overflow so we alloc on heap

        uint32_t* rectIndex = new uint32_t[s_renderer2dStorage.MaxRectIndex];

        uint32_t offset{0};
        for (uint32_t i{}; i < s_renderer2dStorage.MaxRectIndex; i += 6)
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
        auto indexBuffer = Creepy::IndexBuffer::Create(rectIndex, s_renderer2dStorage.MaxRectIndex);

        Rect.RectVertexArray->SetIndexBuffer(indexBuffer);

        delete[] rectIndex;
        rectIndex = nullptr;
    }

    void initCircle() noexcept {

        auto&& Circle = s_renderer2dStorage.Circle;

        Circle.CircleVertexArray = VertexArray::Create();

        Circle.CircleVertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxRectVertex * sizeof(CircleVertex));

        Creepy::BufferLayout circleVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float2, "a_localPosition"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Float, "a_thickness"},
            {Creepy::ShaderDataType::Float, "a_fade"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        Circle.CircleVertexBuffer->SetLayout(circleVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        Circle.CircleVertexArray->AddVertexBuffer(Circle.CircleVertexBuffer);

        uint32_t* circleIndex = new uint32_t[s_renderer2dStorage.MaxRectIndex];

        uint32_t offset{0};

        for (uint32_t i{}; i < s_renderer2dStorage.MaxRectIndex; i += 6)
        {

            circleIndex[i + 0] = offset + 0;
            circleIndex[i + 1] = offset + 1;
            circleIndex[i + 2] = offset + 2;

            circleIndex[i + 3] = offset + 2;
            circleIndex[i + 4] = offset + 3;
            circleIndex[i + 5] = offset + 0;

            offset += 4;
        }

        ENGINE_LOG_WARNING("Gona Create Index");

        auto circleIndexBuffer = Creepy::IndexBuffer::Create(circleIndex, s_renderer2dStorage.MaxRectIndex);

        Circle.CircleVertexArray->SetIndexBuffer(circleIndexBuffer); // rectIndex

        Circle.CircleVertexBufferBase = new CircleVertex[s_renderer2dStorage.MaxRectVertex];

        delete[] circleIndex;
        circleIndex = nullptr;
    }

    void initLines() noexcept {

        s_renderer2dStorage.Lines.LineVertexArray = VertexArray::Create();

        s_renderer2dStorage.Lines.LineVertexBuffer = VertexBuffer::Create(s_renderer2dStorage.MaxRectVertex * sizeof(LineVertex));

        Creepy::BufferLayout lineVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        s_renderer2dStorage.Lines.LineVertexBuffer->SetLayout(lineVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        s_renderer2dStorage.Lines.LineVertexArray->AddVertexBuffer(s_renderer2dStorage.Lines.LineVertexBuffer);

        s_renderer2dStorage.Lines.LineVertexBufferBase = new LineVertex[s_renderer2dStorage.MaxRectVertex];
    }

    void initStorage2D() noexcept {
        initRect();
        initCircle();
        initLines();

        ENGINE_LOG_WARNING("Gona Create White Texture");
        s_renderer2dStorage.WhiteTexture = Texture2D::Create(1, 1);

        uint32_t whiteTextureData = 0xffffffff;

        s_renderer2dStorage.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        
        ENGINE_LOG_WARNING("Gona Create Shader");
        s_renderer2dStorage.Rect.RectShader = Shader::Create("./assets/shaders/RectVertexShader.glsl", "./assets/shaders/RectFragmentShader.glsl");

        s_renderer2dStorage.Circle.CircleShader = Shader::Create("./assets/shaders/CircleVertexShader.glsl", "./assets/shaders/CircleFragmentShader.glsl");
        
        s_renderer2dStorage.Lines.LineShader = Shader::Create("./assets/shaders/LineVertexShader.glsl", "./assets/shaders/LineFragmentShader.glsl");

        s_renderer2dStorage.TextureSlots[0] = s_renderer2dStorage.WhiteTexture;

        s_renderer2dStorage.RectVertexPosition[0] = {-0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[1] = { 0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[2] = { 0.5,  0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[3] = {-0.5,  0.5, 0.0f, 1.0f};
    }

    void initCube() noexcept {

        auto&& Cube = s_renderer3dStorage.Cube;

        Cube.CubeVertexArray = VertexArray::Create();

        Cube.CubeVertexBuffer = VertexBuffer::Create(Cube.MaxCubesVertices * sizeof(ModelVertex));

        BufferLayout cubeBufferLayout{
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float4, "a_color"},
            {ShaderDataType::Float3, "a_normal"},
            {ShaderDataType::Float2, "a_textureCoord"},
            {ShaderDataType::Float, "a_textureIndex"},
            {ShaderDataType::Int, "a_entityID"},
        };

        Cube.CubeVertexBuffer->SetLayout(cubeBufferLayout);

        Cube.CubeVertexArray->AddVertexBuffer(Cube.CubeVertexBuffer);

        Cube.CubeVertexBufferBase = new ModelVertex[s_renderer3dStorage.MaxModels];

        Cube.CubeMesh = Utils::ModelImporter::LoadMesh("./assets/models/cube/Cube.gltf").at(0);

        uint32_t* cubeIndices = new uint32_t[Cube.MaxCubesIndices];

        size_t offset{0};

        const auto&& cubeMeshIndices = Cube.CubeMesh->GetIndices();

        auto totalIndices = Cube.CubeMesh->GetTotalIndices();
        for(size_t i{}; i < Cube.MaxCubesIndices; i += totalIndices){
            // TODO: Use std::span::at()

            for(size_t j{}; j < totalIndices; j++){

                cubeIndices[i + j] = offset + cubeMeshIndices[j];
            }
            
            offset += totalIndices;
        }

        auto cubeIndexBuffer = IndexBuffer::Create(cubeIndices, Cube.MaxCubesIndices);
        
        Cube.CubeVertexArray->SetIndexBuffer(cubeIndexBuffer);
        
        delete[] cubeIndices;
        cubeIndices = nullptr;

        Cube.CubeShader = Shader::Create("./assets/shaders/CubeVertexShader.glsl", "./assets/shaders/CubeFragmentShader.glsl");
    }

    void initStorage3D() noexcept {
        initCube();
    }

    void Renderer::Init() noexcept {
        
        RenderCommand::Init();

        initCoreStorage();
        initStorage2D();
        initStorage3D();

    }

    void Renderer::ShutDown() noexcept {

        delete[] s_renderer2dStorage.Rect.RectVertexBufferBase;
        delete[] s_renderer2dStorage.Circle.CircleVertexBufferBase;
        delete[] s_renderer2dStorage.Lines.LineVertexBufferBase;
        delete[] s_renderer3dStorage.Cube.CubeVertexBufferBase;
        

        s_renderer2dStorage.Rect.RectVertexArray.reset();
        s_renderer2dStorage.Rect.RectVertexBuffer.reset();
        s_renderer2dStorage.Rect.RectShader.reset();

        s_renderer2dStorage.Circle.CircleVertexArray.reset();
        s_renderer2dStorage.Circle.CircleVertexBuffer.reset();
        s_renderer2dStorage.Circle.CircleShader.reset();

        s_renderer2dStorage.Lines.LineVertexArray.reset();
        s_renderer2dStorage.Lines.LineVertexBuffer.reset();
        s_renderer2dStorage.Lines.LineShader.reset();

        s_renderer3dStorage.Cube.CubeVertexArray.reset();
        s_renderer3dStorage.Cube.CubeVertexBuffer.reset();
        s_renderer3dStorage.Cube.CubeShader.reset();

        s_renderer2dStorage.WhiteTexture.reset();
    }

    

    void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform) noexcept {

        glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

        s_rendererCoreStorage.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        s_rendererCoreStorage.UniformBuffer->SetData(&s_rendererCoreStorage.CameraBuffer, sizeof(s_rendererCoreStorage.CameraBuffer));

        // Reset
        start2DBatch();
        start3DBatch();
    }

    void Renderer::BeginScene(const EditorCamera& camera) noexcept {
        glm::mat4 viewProjection = camera.GetViewProjectionMatrix();

        s_rendererCoreStorage.CameraBuffer.ViewProjection = viewProjection;
        s_rendererCoreStorage.UniformBuffer->SetData(&s_rendererCoreStorage.CameraBuffer, sizeof(s_rendererCoreStorage.CameraBuffer));

        start2DBatch();
        start3DBatch();
    }

    void Renderer::EndScene() noexcept {
        Flush2DBatch();
        Flush3DBatch();
    }

    void Renderer::Flush2DBatch() noexcept {

        if(auto&& Rect = s_renderer2dStorage.Rect; Rect.RectIndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Rect.RectVertexBufferPointer) - reinterpret_cast<uint8_t*>(Rect.RectVertexBufferBase);
        
            Rect.RectVertexBuffer->SetData(Rect.RectVertexBufferBase, dataSize);
            
            // Bind All Texture
            for(uint32_t i{0}; i < s_renderer2dStorage.TextureSlotIndex; i++){
                s_renderer2dStorage.TextureSlots[i]->Bind(i);
            }

            Rect.RectShader->Bind();
            RenderCommand::DrawIndex(Rect.RectVertexArray, Rect.RectIndexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }

        if(auto&& Circle = s_renderer2dStorage.Circle; Circle.CircleIndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Circle.CircleVertexBufferPointer) - reinterpret_cast<uint8_t*>(Circle.CircleVertexBufferBase);
        
            Circle.CircleVertexBuffer->SetData(Circle.CircleVertexBufferBase, dataSize);
        
            Circle.CircleShader->Bind();
            RenderCommand::DrawIndex(Circle.CircleVertexArray, Circle.CircleIndexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }
        
        if(auto&& Lines = s_renderer2dStorage.Lines; Lines.LineVertexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Lines.LineVertexBufferPointer) - reinterpret_cast<uint8_t*>(Lines.LineVertexBufferBase);
        
            Lines.LineVertexBuffer->SetData(Lines.LineVertexBufferBase, dataSize);
        
            Lines.LineShader->Bind();
            RenderCommand::DrawLines(Lines.LineVertexArray, Lines.LineVertexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }
    }

    void Renderer::Flush3DBatch() noexcept {

        if(auto&& Cube = s_renderer3dStorage.Cube; Cube.CubeIndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Cube.CubeVertexBufferPointer) - reinterpret_cast<uint8_t*>(Cube.CubeVertexBufferBase);

            Cube.CubeVertexBuffer->SetData(Cube.CubeVertexBufferBase, dataSize);

            for(uint32_t i{0}; i < s_renderer2dStorage.TextureSlotIndex; i++){
                s_renderer2dStorage.TextureSlots[i]->Bind(i);
            }

            Cube.CubeShader->Bind();

            RenderCommand::DrawIndex(Cube.CubeVertexArray, Cube.CubeIndexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }

    }

    void Renderer::start2DBatch() noexcept{
        s_renderer2dStorage.Rect.RectIndexCount = 0;
        s_renderer2dStorage.Rect.RectVertexBufferPointer = s_renderer2dStorage.Rect.RectVertexBufferBase;

        s_renderer2dStorage.Circle.CircleIndexCount = 0;
        s_renderer2dStorage.Circle.CircleVertexBufferPointer = s_renderer2dStorage.Circle.CircleVertexBufferBase;

        s_renderer2dStorage.Lines.LineVertexCount = 0;
        s_renderer2dStorage.Lines.LineVertexBufferPointer = s_renderer2dStorage.Lines.LineVertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer::start3DBatch() noexcept {
        s_renderer3dStorage.Cube.CubeIndexCount = 0;
        s_renderer3dStorage.Cube.CubeVertexBufferPointer = s_renderer3dStorage.Cube.CubeVertexBufferBase;
    }

    void Renderer::flushAndReset2DBatch() noexcept {
        Flush2DBatch();

        start2DBatch();
    }

    void Renderer::flushAndReset3DBatch() noexcept {
        Flush3DBatch();

        start3DBatch();
    }

    // TODO: Separate
    void Renderer::check2DBatchNeedReset() noexcept {
        if(s_renderer2dStorage.Rect.RectIndexCount >= s_renderer2dStorage.MaxRectIndex){
            flushAndReset2DBatch();
        }

        if(s_renderer2dStorage.Circle.CircleIndexCount >= s_renderer2dStorage.MaxRectIndex){
            flushAndReset2DBatch();
        }
    }

    void Renderer::check3DBatchNeedReset() noexcept {
        if(s_renderer3dStorage.Cube.CubeIndexCount >= s_renderer3dStorage.MaxModels){
            flushAndReset3DBatch();
        }
    }

    void Renderer::checkTextureNeedReset(float& textureIndex, const Ref<Texture2D>& texture) noexcept {

        for(uint32_t i{1}; i < s_renderer2dStorage.TextureSlotIndex; i++){
            if(*s_renderer2dStorage.TextureSlots[i].get() == *texture.get()){
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        // If it not exit we add texture to texture array
        if(std::is_eq(textureIndex <=> 0.0f)){

            if(s_renderer2dStorage.TextureSlotIndex >= s_renderer2dStorage.MaxTextureSlots){
                flushAndReset2DBatch();
            }

            textureIndex = static_cast<float>(s_renderer2dStorage.TextureSlotIndex);

            s_renderer2dStorage.TextureSlots[s_renderer2dStorage.TextureSlotIndex] = texture;

            ++s_renderer2dStorage.TextureSlotIndex;
        }

    }

    void Renderer::setRectProperty(const glm::mat4& transform, const glm::vec4& color, const std::array<glm::vec2, 4>& textureCoords, float textureIndex, float tilingFactor, int entityID) noexcept {
        auto&& Rect = s_renderer2dStorage.Rect;

        for(size_t i{0}; i < 4; i++){
            
            Rect.RectVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[i];
            Rect.RectVertexBufferPointer->Color = color;
            Rect.RectVertexBufferPointer->TextureCoord = textureCoords.at(i);
            Rect.RectVertexBufferPointer->TextureIndex = textureIndex;
            Rect.RectVertexBufferPointer->TilingFactor = tilingFactor;
            Rect.RectVertexBufferPointer->EntityID = entityID;

            Rect.RectVertexBufferPointer++;
        }

        Rect.RectIndexCount += 6;


        ++s_rendererCoreStorage.Stats.RectCount;
    }

    void Renderer::setCubeProperty(const glm::mat4& transform, const glm::vec4& color, float textureIndex, int entityID) noexcept {
        
        auto&& Cube = s_renderer3dStorage.Cube;

        for(size_t i{}; i < Cube.CubeMesh->GetTotalVertices(); i++){

            auto&& vertexAt =  Cube.CubeMesh->GetVertices()[i];

            Cube.CubeVertexBufferPointer->Position = transform * vertexAt.Position;
            Cube.CubeVertexBufferPointer->Color = color;
            Cube.CubeVertexBufferPointer->Normal = vertexAt.Normal;
            Cube.CubeVertexBufferPointer->TextureCoord = vertexAt.TextureCoord;
            Cube.CubeVertexBufferPointer->TextureIndex = textureIndex;
            Cube.CubeVertexBufferPointer->EntityID = entityID;

            Cube.CubeVertexBufferPointer++;

        }

        Cube.CubeIndexCount += Cube.CubeMesh->GetTotalIndices();

    }

    void Renderer::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {
        DrawRect({position.x, position.y, 0.0f}, size, color);
    }
    
    void Renderer::DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        check2DBatchNeedReset();

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

    void Renderer::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {
        DrawRect(glm::vec3(position.x, position.y, 0.0f), size, texture, tilingColor);
    }

    void Renderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        check2DBatchNeedReset();

        // We check if texture already exit on array
        float textureIndex{0.0f};
        checkTextureNeedReset(textureIndex, texture);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }

    void Renderer::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor) noexcept {
        DrawRect(glm::vec3(position.x, position.y, 0.0f), size, subTexture, tilingColor);
    }

    void Renderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor) noexcept {

        // For sure if we draw too much rect > limit, we need to reset it and then start new scene to draw fit rect
        check2DBatchNeedReset();

        const auto& textureCoords = subTexture->GetTextureCoords();

        const auto& texture = subTexture->GetTexture();

        // We check if texture already exit on array
        float textureIndex{0.0f};

        checkTextureNeedReset(textureIndex, texture);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

        setRectProperty(transform, tilingColor, textureCoords, textureIndex);
    }

    void Renderer::DrawRect(const glm::mat4& transform, const glm::vec4& color, float tilingFactor, int entityID) noexcept {
        check2DBatchNeedReset();

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        constexpr float textureIndex{0.0f};

        setRectProperty(transform, color, textureCoords, textureIndex, tilingFactor, entityID);
    }

    void Renderer::DrawRect(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tilingColor, float tilingFactor, int entityID) noexcept {
        check2DBatchNeedReset();

        // We check if texture already exit on array
        float textureIndex{0.0f};

        checkTextureNeedReset(textureIndex, texture);

        const std::array textureCoords {
            glm::vec2{0.0f, 0.0f},
            glm::vec2{1.0f, 0.0f},
            glm::vec2{1.0f, 1.0f},
            glm::vec2{0.0f, 1.0f}
        };

        setRectProperty(transform, tilingColor, textureCoords, textureIndex, tilingFactor, entityID);
    }

    void Renderer::DrawSprite(TransformComponent& transform, SpriteComponent& sprite, int entityID) noexcept {
        if(sprite.Texture){
            DrawRect(transform.GetTransform(), sprite.Texture, sprite.Color, sprite.TilingFactor, entityID);
        }
        DrawRect(transform.GetTransform(), sprite.Color, sprite.TilingFactor, entityID);
    }

    void Renderer::DrawCircle(TransformComponent& transform, CircleSpriteComponent& circle, int entityID) noexcept {

        check2DBatchNeedReset();

        for(uint32_t i{}; i < 4; i++){

            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Position = transform.GetTransform() * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->LocalPosition = s_renderer2dStorage.RectVertexPosition[i] * 2.0f;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Color = circle.Color;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Thickness = circle.Thickness;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Fade = circle.Fade;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.Circle.CircleVertexBufferPointer++;
        }

        s_renderer2dStorage.Circle.CircleIndexCount += 6;

    }

    void Renderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickNess, float fade, int entityID) noexcept {
        
        check2DBatchNeedReset();

        for(uint32_t i{}; i < 4; i++){

            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->LocalPosition = s_renderer2dStorage.RectVertexPosition[i] * 2.0f;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Color = color;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Thickness = thickNess;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->Fade = fade;
            s_renderer2dStorage.Circle.CircleVertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.Circle.CircleVertexBufferPointer++;
        }

        s_renderer2dStorage.Circle.CircleIndexCount += 6;
    }

    void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID) noexcept {

        s_renderer2dStorage.Lines.LineVertexBufferPointer->Position = start;
        s_renderer2dStorage.Lines.LineVertexBufferPointer->Color = color;
        s_renderer2dStorage.Lines.LineVertexBufferPointer->EntityID = entityID;
        s_renderer2dStorage.Lines.LineVertexBufferPointer++;

        s_renderer2dStorage.Lines.LineVertexBufferPointer->Position = end;
        s_renderer2dStorage.Lines.LineVertexBufferPointer->Color = color;
        s_renderer2dStorage.Lines.LineVertexBufferPointer->EntityID = entityID;
        s_renderer2dStorage.Lines.LineVertexBufferPointer++;

        s_renderer2dStorage.Lines.LineVertexCount += 2;
    }


    void Renderer::DrawLineRect(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, int entityID) noexcept {

        glm::vec3 p0 = glm::vec3{pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, pos.z};
        glm::vec3 p1 = glm::vec3{pos.x + size.x * 0.5f, pos.y - size.y * 0.5f, pos.z};
        glm::vec3 p2 = glm::vec3{pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, pos.z};
        glm::vec3 p3 = glm::vec3{pos.x - size.x * 0.5f, pos.y + size.y * 0.5f, pos.z};

        DrawLine(p0, p1, color, entityID);
        DrawLine(p1, p2, color, entityID);
        DrawLine(p2, p3, color, entityID);
        DrawLine(p3, p0, color, entityID);

    }
            
    void Renderer::DrawLineRect(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        glm::vec3 lineVertex[4];

        for(size_t i{}; i < 4; i++){
            lineVertex[i] = transform * s_renderer2dStorage.RectVertexPosition[i];
        }

        DrawLine(lineVertex[0], lineVertex[1], color, entityID);
        DrawLine(lineVertex[1], lineVertex[2], color, entityID);
        DrawLine(lineVertex[2], lineVertex[3], color, entityID);
        DrawLine(lineVertex[3], lineVertex[0], color, entityID);
    }


    // MARK: 3D Draw

    void Renderer::DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        constexpr float textureIndex{0.0f};
        
        setCubeProperty(transform, color, textureIndex, entityID);
    }


    void Renderer::ResetStatistics() noexcept {
        s_rendererCoreStorage.Stats.DrawCalls = 0;
        s_rendererCoreStorage.Stats.RectCount = 0;
    }

    Renderer::Statistics Renderer::GetStatistics() noexcept {
        return s_rendererCoreStorage.Stats;
    }

}