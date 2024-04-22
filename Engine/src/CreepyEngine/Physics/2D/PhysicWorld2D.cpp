#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Physics/2D/PhysicWorld2D.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace Creepy::Physic2D{

    // TODO: Move to util
    static b2BodyType ConvertRigid2DBodyTypeToBox2DTemp(RigidBody2DComponent::BodyType bodyType) noexcept {
        switch(bodyType){
            case RigidBody2DComponent::BodyType::STATIC:
                return b2BodyType::b2_staticBody;

            case RigidBody2DComponent::BodyType::DYNAMIC:
                return b2BodyType::b2_dynamicBody;

            case RigidBody2DComponent::BodyType::KINEMATIC:
                return b2BodyType::b2_kinematicBody;
        }
        std::unreachable();
        return b2BodyType::b2_staticBody;
    }

    using namespace Creepy;

    Ref<b2World> PhysicWorld2D::s_physicWorld2D = nullptr;

    void PhysicWorld2D::InitWorld2D(Scene* scene) noexcept {
        s_physicWorld2D = std::make_shared<b2World>(b2Vec2{0.0f, -9.8f});
        
        auto&& rigidView  = scene->m_registry.view<RigidBody2DComponent>();

        for(auto&& entityID : rigidView){

            Entity entity{entityID, scene};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rigid2D = entity.GetComponent<RigidBody2DComponent>();

            b2BodyDef bodyDef;

            bodyDef.type = ConvertRigid2DBodyTypeToBox2DTemp(rigid2D.Type);

            bodyDef.position.Set(transform.Position.x, transform.Position.y);
            bodyDef.angle = transform.Rotation.z;

            auto body = s_physicWorld2D->CreateBody(&bodyDef);

            body->SetFixedRotation(rigid2D.FixedRotation);

            rigid2D.RuntimeBody = body;

            if(entity.HasComponent<BoxCollider2DComponent>()){
                auto& bxCol2D = entity.GetComponent<BoxCollider2DComponent>();
                
                b2PolygonShape boxShape;
                
                boxShape.SetAsBox(transform.Scale.x * bxCol2D.Size.x, transform.Scale.y * bxCol2D.Size.y, {bxCol2D.Offset.x, bxCol2D.Offset.y}, 0.0f);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bxCol2D.Density;
                fixtureDef.friction = bxCol2D.Friction;
                fixtureDef.restitution = bxCol2D.Restitution;
                fixtureDef.restitutionThreshold = bxCol2D.RestitutionThreshold;

                body->CreateFixture(&fixtureDef);
            }

            if(entity.HasComponent<CircleCollider2DComponent>()){

                auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

                b2CircleShape circleShape;
                circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
                circleShape.m_radius = cc2d.Radius * transform.Scale.x;

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &circleShape;
                fixtureDef.density = cc2d.Density;
                fixtureDef.friction = cc2d.Friction;
                fixtureDef.restitution = cc2d.Restitution;
                fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;

                body->CreateFixture(&fixtureDef);
            }

        }
    }

    void PhysicWorld2D::UpdateWorld2D(Creepy::TimeStep timeStep, Scene* scene) noexcept {

        constexpr const int32_t velocityIteration{8};
        constexpr const int32_t positionIteration{3};

        s_physicWorld2D->Step(timeStep.GetSeconds(), velocityIteration, positionIteration);

        auto &&view = scene->m_registry.view<RigidBody2DComponent>();

        for (auto &&entityID : view)
        {
            Entity entity{entityID, scene};
            auto &transform = entity.GetComponent<TransformComponent>();
            auto &rigid2D = entity.GetComponent<RigidBody2DComponent>();

            auto body = reinterpret_cast<b2Body *>(rigid2D.RuntimeBody);
            auto &&position = body->GetPosition();

            transform.Position.x = position.x;
            transform.Position.y = position.y;

            transform.Rotation.z = body->GetAngle();
        }
    }

    void PhysicWorld2D::DestroyWorld2D(Scene* scene) noexcept {
        
        if(!s_physicWorld2D){
            return;
        }

        auto&& entityDestroy = scene->m_registry.view<RigidBody2DComponent>();

        for(auto&& entityID : entityDestroy){
            Entity entity{entityID, scene};
            s_physicWorld2D->DestroyBody(static_cast<b2Body*>(entity.GetComponent<RigidBody2DComponent>().RuntimeBody));

            entity.GetComponent<RigidBody2DComponent>().RuntimeBody = nullptr;
        }

        s_physicWorld2D.reset();
        s_physicWorld2D = nullptr;
    }

}