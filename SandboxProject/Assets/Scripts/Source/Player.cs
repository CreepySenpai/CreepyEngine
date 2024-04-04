using System;
using System.Numerics;
using Creepy.NativeDataType;
using Sandbox;
using Creepy;

namespace Creepy;

public class Player : Entity {
    private float Speed = 1.0f;
    public float Age = 54.0f;
    public override void OnCreate(){
        Console.WriteLine($"Create Player {UUID}");
    }

    public override void OnUpdate(float timeStep){
        Vector3 pos = Position;
        float sp = Speed;
        pos.X += sp * timeStep;
        Position = pos;
    }

}