using System;
using System.Numerics;
using Creepy.NativeDataType;
using Sandbox;
using Creepy;

namespace Creepy;

public class Player : Entity {

    [ShowField]
    private float Speed = 1.0f;
    public float Age = 54.0f;

    [ShowField]
    public float a = 3.0f;

    [CustomAttb]
    private float b = 1.0f;

    [CustomAttb]
    public float c = 3.0f;

    private float d;

    private float e;


    public Oni on;
    public override void OnCreate(){
        Console.WriteLine($"Create Player {UUID}");
        on = new();
    }

    public override void OnUpdate(float timeStep){
        Vector3 pos = Position;
        float sp = Speed;
        pos.X += sp * timeStep;
        Position = pos;
    }

}