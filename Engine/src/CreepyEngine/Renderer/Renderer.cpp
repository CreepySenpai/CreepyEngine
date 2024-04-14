#include <numeric>
#include <type_traits>

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
#include <CreepyEngine/Utils/ConceptUtils.hpp>

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
        glm::mat4 Transform{};
        glm::vec4 Color{};
        float TextureIndex{};
        int EntityID{-1};

        ModelVertex() noexcept = default;

        ModelVertex(const glm::mat4& transform, const glm::vec4& color, float textureIndex, int entityID) noexcept 
            : Transform{transform}, Color{color}, TextureIndex{textureIndex}, EntityID{entityID} {
        }

    };

    struct RendererCoreStorage{

        Renderer::Statistics Stats;

        struct CameraData{
            glm::mat4 ViewProjection;
        };

        CameraData CameraBuffer;
        Ref<UniformBuffer> UniformBuffer;
    };

    template <typename T>
    concept ValidBatch2D = Utils::IsAnyOf<std::remove_cvref_t<T>, RectVertex, CircleVertex, LineVertex>;

    template <typename T>
    concept ValidBatch3D = Utils::IsAnyOf<std::remove_cvref_t<T>, ModelVertex>;

    template <ValidBatch2D T>
    struct BatchData2D{
        Ref<Creepy::VertexArray> VertexArray{nullptr};
        Ref<Creepy::VertexBuffer> VertexBuffer{nullptr};
        Ref<Creepy::Shader> Shader{nullptr};
        uint32_t IndexCount{0};
        T* VertexBufferBase{nullptr};
        T* VertexBufferPointer{nullptr};
    };

    struct InstanceData3D{
        Ref<Creepy::VertexArray> VertexArray{nullptr};
        Ref<Creepy::VertexBuffer> VertexBuffer{nullptr};
        Ref<Creepy::VertexBuffer> InstanceBuffer{nullptr};
        Ref<Creepy::Shader> Shader{nullptr};
        Ref<Creepy::Mesh> Mesh{nullptr};
        uint32_t InstanceCount{0};
        std::vector<ModelVertex> InstanceData;
    };


    struct Renderer2DStorage {
        const uint32_t MaxRects{10000u};
        const uint32_t MaxRectVertex{MaxRects * 4};
        const uint32_t MaxRectIndex{MaxRects * 6};
        static const uint32_t MaxTextureSlots{32};     //TODO: Change Texture SLot To Asset Manager

        BatchData2D<RectVertex> Rect;
        
        BatchData2D<CircleVertex> Circle;

        BatchData2D<LineVertex> Lines;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex{1};   // 0: white texture

        glm::vec4 RectVertexPosition[4];
        Ref<Texture2D> WhiteTexture{nullptr};
    };

    struct Renderer3DStorage {
        // TODO: Increase Max Cube
        const uint32_t MaxModels{10000u};

        InstanceData3D Cube;

        InstanceData3D Cylinder;

        InstanceData3D Cone;

        InstanceData3D Torus;
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
        Rect.VertexArray = Creepy::VertexArray::Create();

        ENGINE_LOG_WARNING("Gona Create Vertex Buffer");
        Rect.VertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxRectVertex * sizeof(RectVertex));

        Creepy::BufferLayout rectVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Float2, "a_textureCoord"},
            {Creepy::ShaderDataType::Float, "a_textureIndex"},
            {Creepy::ShaderDataType::Float, "a_tilingFactor"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        Rect.VertexBuffer->SetLayout(rectVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        Rect.VertexArray->AddVertexBuffer(Rect.VertexBuffer);

        Rect.VertexBufferBase = new RectVertex[s_renderer2dStorage.MaxRectVertex];

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

        Rect.VertexArray->SetIndexBuffer(indexBuffer);

        delete[] rectIndex;
        rectIndex = nullptr;
    }

    void initCircle() noexcept {

        auto&& Circle = s_renderer2dStorage.Circle;

        Circle.VertexArray = VertexArray::Create();

        Circle.VertexBuffer = Creepy::VertexBuffer::Create(s_renderer2dStorage.MaxRectVertex * sizeof(CircleVertex));

        Creepy::BufferLayout circleVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float2, "a_localPosition"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Float, "a_thickness"},
            {Creepy::ShaderDataType::Float, "a_fade"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        Circle.VertexBuffer->SetLayout(circleVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        Circle.VertexArray->AddVertexBuffer(Circle.VertexBuffer);

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

        Circle.VertexArray->SetIndexBuffer(circleIndexBuffer); // rectIndex

        Circle.VertexBufferBase = new CircleVertex[s_renderer2dStorage.MaxRectVertex];

        delete[] circleIndex;
        circleIndex = nullptr;
    }

    void initLines() noexcept {

        s_renderer2dStorage.Lines.VertexArray = VertexArray::Create();

        s_renderer2dStorage.Lines.VertexBuffer = VertexBuffer::Create(s_renderer2dStorage.MaxRectVertex * sizeof(LineVertex));

        Creepy::BufferLayout lineVertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float4, "a_color"},
            {Creepy::ShaderDataType::Int, "a_entityID"},
        };

        s_renderer2dStorage.Lines.VertexBuffer->SetLayout(lineVertexBufferLayout);

        // We need add buffer after it add layout, if not it will empty
        s_renderer2dStorage.Lines.VertexArray->AddVertexBuffer(s_renderer2dStorage.Lines.VertexBuffer);

        s_renderer2dStorage.Lines.VertexBufferBase = new LineVertex[s_renderer2dStorage.MaxRectVertex];
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
        s_renderer2dStorage.Rect.Shader = Shader::Create("./assets/shaders/RectVertexShader.glsl", "./assets/shaders/RectFragmentShader.glsl");

        s_renderer2dStorage.Circle.Shader = Shader::Create("./assets/shaders/CircleVertexShader.glsl", "./assets/shaders/CircleFragmentShader.glsl");
        
        s_renderer2dStorage.Lines.Shader = Shader::Create("./assets/shaders/LineVertexShader.glsl", "./assets/shaders/LineFragmentShader.glsl");

        s_renderer2dStorage.TextureSlots[0] = s_renderer2dStorage.WhiteTexture;

        s_renderer2dStorage.RectVertexPosition[0] = {-0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[1] = { 0.5, -0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[2] = { 0.5,  0.5, 0.0f, 1.0f};
        s_renderer2dStorage.RectVertexPosition[3] = {-0.5,  0.5, 0.0f, 1.0f};
    }

    
    static void initPrimitive(InstanceData3D& primitive, const std::filesystem::path& meshPath) noexcept{

        primitive.VertexArray = VertexArray::Create();

        primitive.Mesh = Utils::ModelImporter::LoadMesh(meshPath).at(0);

        auto totalVertices = primitive.Mesh->GetTotalVertices();
        auto totalIndices = primitive.Mesh->GetTotalIndices();

        {   // Init Shared Vertex Buffer
        
            struct ModelSharedVertex{
                glm::vec3 Position;
                glm::vec3 Normal;
                glm::vec2 TextureCoord;
            };

            BufferLayout primitiveBufferLayout{
                {ShaderDataType::Float3, "a_position"},
                {ShaderDataType::Float3, "a_normal"},
                {ShaderDataType::Float2, "a_textureCoord"}
            };

            ModelSharedVertex* vertices = new ModelSharedVertex[totalVertices];

            auto&& meshVertices = primitive.Mesh->GetVertices();

            for(size_t i{}; i < totalVertices; i++){
                vertices[i].Position = meshVertices[i].Position;
                vertices[i].Normal = meshVertices[i].Normal;
                vertices[i].TextureCoord = meshVertices[i].TextureCoord;
            }

            primitive.VertexBuffer = VertexBuffer::Create(vertices, primitive.Mesh->GetTotalVertices() * sizeof(ModelSharedVertex));

            primitive.VertexBuffer->SetLayout(primitiveBufferLayout);

            primitive.VertexArray->AddVertexBuffer(primitive.VertexBuffer);

            delete[] vertices;
            vertices = nullptr;
        }

        {   // Init Instance Vertex Buffer
            BufferLayout instanceBufferLayout{
                {ShaderDataType::Mat4, "a_transform"},
                {ShaderDataType::Float4, "a_color"},
                {ShaderDataType::Float, "a_textureIndex"},
                {ShaderDataType::Int, "a_entityID"}
            };

            primitive.InstanceBuffer = VertexBuffer::Create(s_renderer3dStorage.MaxModels * sizeof(ModelVertex));

            primitive.InstanceBuffer->SetLayout(instanceBufferLayout);

            primitive.VertexArray->AddVertexBufferInstance(primitive.InstanceBuffer);
        }
        
        {   // Init Index Buffers
            auto primitiveIndexBuffer = IndexBuffer::Create(primitive.Mesh->GetIndices().data(), totalIndices);
            
            primitive.VertexArray->SetIndexBuffer(primitiveIndexBuffer);
        }

        primitive.Shader = Shader::Create("./assets/shaders/PrimitiveVertexShader.glsl", "./assets/shaders/PrimitiveFragmentShader.glsl");
    }

    void initStorage3D() noexcept {
        
        initPrimitive(s_renderer3dStorage.Cube, "./assets/models/primitive/cube/Cube.gltf");
        initPrimitive(s_renderer3dStorage.Cylinder, "./assets/models/primitive/cylinder/Cylinder.gltf");

        initPrimitive(s_renderer3dStorage.Cone, "./assets/models/primitive/cone/Cone.gltf");
        initPrimitive(s_renderer3dStorage.Torus, "./assets/models/primitive/torus/Torus.gltf");

    }

    void Renderer::Init() noexcept {
        
        RenderCommand::Init();

        initCoreStorage();
        initStorage2D();
        initStorage3D();

    }

    void Renderer::ShutDown() noexcept {

        delete[] s_renderer2dStorage.Rect.VertexBufferBase;
        delete[] s_renderer2dStorage.Circle.VertexBufferBase;
        delete[] s_renderer2dStorage.Lines.VertexBufferBase;

        s_renderer2dStorage.Rect.VertexArray.reset();
        s_renderer2dStorage.Rect.VertexBuffer.reset();
        s_renderer2dStorage.Rect.Shader.reset();

        s_renderer2dStorage.Circle.VertexArray.reset();
        s_renderer2dStorage.Circle.VertexBuffer.reset();
        s_renderer2dStorage.Circle.Shader.reset();

        s_renderer2dStorage.Lines.VertexArray.reset();
        s_renderer2dStorage.Lines.VertexBuffer.reset();
        s_renderer2dStorage.Lines.Shader.reset();

        s_renderer3dStorage.Cube.VertexArray.reset();
        s_renderer3dStorage.Cube.VertexBuffer.reset();
        s_renderer3dStorage.Cube.Shader.reset();
        
        s_renderer3dStorage.Cylinder.VertexArray.reset();
        s_renderer3dStorage.Cylinder.VertexBuffer.reset();
        s_renderer3dStorage.Cylinder.Shader.reset();

        s_renderer3dStorage.Cone.VertexArray.reset();
        s_renderer3dStorage.Cone.VertexBuffer.reset();
        s_renderer3dStorage.Cone.Shader.reset();

        s_renderer3dStorage.Torus.VertexArray.reset();
        s_renderer3dStorage.Torus.VertexBuffer.reset();
        s_renderer3dStorage.Torus.Shader.reset();

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

        if(auto&& Rect = s_renderer2dStorage.Rect; Rect.IndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Rect.VertexBufferPointer) - reinterpret_cast<uint8_t*>(Rect.VertexBufferBase);
            
            Rect.VertexBuffer->SetData(Rect.VertexBufferBase, dataSize);
            
            // Bind All Texture
            for(uint32_t i{0}; i < s_renderer2dStorage.TextureSlotIndex; i++){
                s_renderer2dStorage.TextureSlots[i]->Bind(i);
            }

            Rect.Shader->Bind();
            RenderCommand::DrawIndex(Rect.VertexArray, Rect.IndexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }

        if(auto&& Circle = s_renderer2dStorage.Circle; Circle.IndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Circle.VertexBufferPointer) - reinterpret_cast<uint8_t*>(Circle.VertexBufferBase);
        
            Circle.VertexBuffer->SetData(Circle.VertexBufferBase, dataSize);
        
            Circle.Shader->Bind();
            RenderCommand::DrawIndex(Circle.VertexArray, Circle.IndexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }
        
        if(auto&& Lines = s_renderer2dStorage.Lines; Lines.IndexCount){
            uint32_t dataSize = reinterpret_cast<uint8_t*>(Lines.VertexBufferPointer) - reinterpret_cast<uint8_t*>(Lines.VertexBufferBase);

            Lines.VertexBuffer->SetData(Lines.VertexBufferBase, dataSize);
        
            Lines.Shader->Bind();
            RenderCommand::DrawLines(Lines.VertexArray, Lines.IndexCount);
            ++s_rendererCoreStorage.Stats.DrawCalls;
        }
    }

    static void flushPrimitive(InstanceData3D& primitive) noexcept {

        if(primitive.InstanceCount){
            
            primitive.InstanceBuffer->SetData(primitive.InstanceData.data(), primitive.InstanceData.size() * sizeof(ModelVertex));

            for(uint32_t i{}; i < s_renderer2dStorage.TextureSlotIndex; i++){
                s_renderer2dStorage.TextureSlots[i]->Bind(i);
            }

            primitive.Shader->Bind();

            RenderCommand::DrawInstances(primitive.VertexArray, primitive.Mesh->GetTotalIndices(), primitive.InstanceCount);

            ++s_rendererCoreStorage.Stats.DrawCalls;
        }

    }

    void Renderer::Flush3DBatch() noexcept {

        flushPrimitive(s_renderer3dStorage.Cube);

        flushPrimitive(s_renderer3dStorage.Cylinder);

        flushPrimitive(s_renderer3dStorage.Cone);

        flushPrimitive(s_renderer3dStorage.Torus);

    }

    void Renderer::start2DBatch() noexcept{
        s_renderer2dStorage.Rect.IndexCount = 0;
        s_renderer2dStorage.Rect.VertexBufferPointer = s_renderer2dStorage.Rect.VertexBufferBase;

        s_renderer2dStorage.Circle.IndexCount = 0;
        s_renderer2dStorage.Circle.VertexBufferPointer = s_renderer2dStorage.Circle.VertexBufferBase;

        s_renderer2dStorage.Lines.IndexCount = 0;
        s_renderer2dStorage.Lines.VertexBufferPointer = s_renderer2dStorage.Lines.VertexBufferBase;

        s_renderer2dStorage.TextureSlotIndex = 1;
    }

    void Renderer::start3DBatch() noexcept {
        s_renderer3dStorage.Cube.InstanceCount = 0;
        s_renderer3dStorage.Cube.InstanceData.clear();
        
        s_renderer3dStorage.Cylinder.InstanceCount = 0;
        s_renderer3dStorage.Cylinder.InstanceData.clear();

        s_renderer3dStorage.Cone.InstanceCount = 0;
        s_renderer3dStorage.Cone.InstanceData.clear();

        s_renderer3dStorage.Torus.InstanceCount = 0;
        s_renderer3dStorage.Torus.InstanceData.clear();
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
        if(s_renderer2dStorage.Rect.IndexCount >= s_renderer2dStorage.MaxRectIndex){
            flushAndReset2DBatch();
        }

        if(s_renderer2dStorage.Circle.IndexCount >= s_renderer2dStorage.MaxRectIndex){
            flushAndReset2DBatch();
        }
    }

    void Renderer::check3DBatchNeedReset() noexcept {
        // if(s_renderer3dStorage.Cube.InstanceCount >= s_renderer3dStorage.MaxModels){
        //     flushAndReset3DBatch();
        // }

        // if(s_renderer3dStorage.Cylinder.InstanceCount >= s_renderer3dStorage.MaxModels){
        //     flushAndReset3DBatch();
        // }

        // if(s_renderer3dStorage.Cone.InstanceCount >= s_renderer3dStorage.MaxModels){
        //     flushAndReset3DBatch();
        // }

        // if(s_renderer3dStorage.Torus.InstanceCount >= s_renderer3dStorage.MaxModels){
        //     flushAndReset3DBatch();
        // }
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
            
            Rect.VertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[i];
            Rect.VertexBufferPointer->Color = color;
            Rect.VertexBufferPointer->TextureCoord = textureCoords.at(i);
            Rect.VertexBufferPointer->TextureIndex = textureIndex;
            Rect.VertexBufferPointer->TilingFactor = tilingFactor;
            Rect.VertexBufferPointer->EntityID = entityID;

            Rect.VertexBufferPointer++;
        }

        Rect.IndexCount += 6;


        ++s_rendererCoreStorage.Stats.RectCount;
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

            s_renderer2dStorage.Circle.VertexBufferPointer->Position = transform.GetTransform() * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.Circle.VertexBufferPointer->LocalPosition = s_renderer2dStorage.RectVertexPosition[i] * 2.0f;
            s_renderer2dStorage.Circle.VertexBufferPointer->Color = circle.Color;
            s_renderer2dStorage.Circle.VertexBufferPointer->Thickness = circle.Thickness;
            s_renderer2dStorage.Circle.VertexBufferPointer->Fade = circle.Fade;
            s_renderer2dStorage.Circle.VertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.Circle.VertexBufferPointer++;
        }

        s_renderer2dStorage.Circle.IndexCount += 6;

    }

    void Renderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickNess, float fade, int entityID) noexcept {
        
        check2DBatchNeedReset();

        for(uint32_t i{}; i < 4; i++){

            s_renderer2dStorage.Circle.VertexBufferPointer->Position = transform * s_renderer2dStorage.RectVertexPosition[i];
            s_renderer2dStorage.Circle.VertexBufferPointer->LocalPosition = s_renderer2dStorage.RectVertexPosition[i] * 2.0f;
            s_renderer2dStorage.Circle.VertexBufferPointer->Color = color;
            s_renderer2dStorage.Circle.VertexBufferPointer->Thickness = thickNess;
            s_renderer2dStorage.Circle.VertexBufferPointer->Fade = fade;
            s_renderer2dStorage.Circle.VertexBufferPointer->EntityID = entityID;

            s_renderer2dStorage.Circle.VertexBufferPointer++;
        }

        s_renderer2dStorage.Circle.IndexCount += 6;
    }

    void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID) noexcept {

        s_renderer2dStorage.Lines.VertexBufferPointer->Position = start;
        s_renderer2dStorage.Lines.VertexBufferPointer->Color = color;
        s_renderer2dStorage.Lines.VertexBufferPointer->EntityID = entityID;
        s_renderer2dStorage.Lines.VertexBufferPointer++;

        s_renderer2dStorage.Lines.VertexBufferPointer->Position = end;
        s_renderer2dStorage.Lines.VertexBufferPointer->Color = color;
        s_renderer2dStorage.Lines.VertexBufferPointer->EntityID = entityID;
        s_renderer2dStorage.Lines.VertexBufferPointer++;

        s_renderer2dStorage.Lines.IndexCount += 2;
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

    
    static void setPrimitiveProperty(InstanceData3D& primitive, const glm::mat4& transform, const glm::vec4& color, float textureIndex, int entityID = -1) noexcept {

        primitive.InstanceData.emplace_back(transform, color, textureIndex, entityID);

        primitive.InstanceCount++;

    }

    void Renderer::DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        constexpr float textureIndex{0.0f};
        
        setPrimitiveProperty(s_renderer3dStorage.Cube, transform, color, textureIndex, entityID);
    }

    void Renderer::DrawLineCube(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        glm::vec3 lineVertex[8];
        auto&& vertices = s_renderer3dStorage.Cube.Mesh->GetVertices();

        lineVertex[0] = transform * glm::vec4(vertices[0].Position, 1.0f);
        lineVertex[1] = transform * glm::vec4(vertices[1].Position, 1.0f);
        lineVertex[2] = transform * glm::vec4(vertices[2].Position, 1.0f);
        lineVertex[3] = transform * glm::vec4(vertices[3].Position, 1.0f);
        lineVertex[4] = transform * glm::vec4(vertices[4].Position, 1.0f);
        lineVertex[5] = transform * glm::vec4(vertices[5].Position, 1.0f);
        lineVertex[6] = transform * glm::vec4(vertices[10].Position, 1.0f);
        lineVertex[7] = transform * glm::vec4(vertices[28].Position, 1.0f);
        
        DrawLine(lineVertex[0], lineVertex[2], color, entityID);
        DrawLine(lineVertex[0], lineVertex[4], color, entityID);
        DrawLine(lineVertex[0], lineVertex[6], color, entityID);
        DrawLine(lineVertex[1], lineVertex[2], color, entityID);
        DrawLine(lineVertex[1], lineVertex[3], color, entityID);
        DrawLine(lineVertex[1], lineVertex[6], color, entityID);
        DrawLine(lineVertex[2], lineVertex[5], color, entityID);
        DrawLine(lineVertex[3], lineVertex[5], color, entityID);
        DrawLine(lineVertex[3], lineVertex[7], color, entityID);
        DrawLine(lineVertex[4], lineVertex[5], color, entityID);
        DrawLine(lineVertex[4], lineVertex[7], color, entityID);
        DrawLine(lineVertex[6], lineVertex[7], color, entityID);
    }

    void Renderer::DrawCylinder(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {

        constexpr float textureIndex{0.0f};
        
        setPrimitiveProperty(s_renderer3dStorage.Cylinder, transform, color, textureIndex, entityID);
    }

    void Renderer::DrawCone(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        constexpr float textureIndex{0.0f};
        
        setPrimitiveProperty(s_renderer3dStorage.Cone, transform, color, textureIndex, entityID);
    }

    void Renderer::DrawTorus(const glm::mat4& transform, const glm::vec4& color, int entityID) noexcept {
        constexpr float textureIndex{0.0f};
        
        setPrimitiveProperty(s_renderer3dStorage.Torus, transform, color, textureIndex, entityID);
    }


    void Renderer::ResetStatistics() noexcept {
        s_rendererCoreStorage.Stats.DrawCalls = 0;
        s_rendererCoreStorage.Stats.RectCount = 0;
    }

    Renderer::Statistics Renderer::GetStatistics() noexcept {
        return s_rendererCoreStorage.Stats;
    }

}