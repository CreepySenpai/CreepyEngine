using System;
using System.Numerics;
using Creepy.NativeDataType;
using Sandbox;
using Creepy;
using Creepy.Attributes;

namespace Creepy.Runtime;

public class Player : Entity {

    [Serializer]
    private float Speed = 1.0f;
    public float Age = 54.0f;

    private Camera cam = null;
    
    public override void OnCreate(){
        Console.WriteLine($"Create Player {UUID}");
    }

    public override void OnUpdate(float timeStep){
        Vector3 pos = Position;
        float sp = Speed;
        pos.X += sp * timeStep;
        Position = pos;

        var en = GetEntityByName("Camera");

        if(en != null){
            cam = en.As<Camera>();
        }

        if(cam != null){
            if(Input.IsKeyPressed(KeyCode.KEY_Q)){
                cam.distanceToPlayer += 1.0f * timeStep;
            }
            if(Input.IsKeyPressed(KeyCode.KEY_E)){
                cam.distanceToPlayer -= 1.0f * timeStep;
            }
        }
    }

}