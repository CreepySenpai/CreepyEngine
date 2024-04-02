using System;
using System.Numerics;
using Sandbox;

namespace Creepy;

public class Player : Entity {
    private Oni o;
    private float speed = 0.1f;

    public override void OnCreate(){
        Console.WriteLine("Gonna Create Player");
        Console.WriteLine($"Create Player {UUID}");
        o = new();
        o.Name = "Anh";
        o.Age = 10;
    }

    public override void OnUpdate(float timeStep){
        Vector3 pos = Position;

        if(Input.IsKeyPressed(KeyCode.KEY_W)){
            
            if(HasComponent<TransformComponent>()){
                Console.WriteLine("Player Has Component");
                Console.WriteLine($"Name: {o.Name}, Age: {o.Age}");
            }
            else {
                Console.WriteLine("Player No Component");
            }
        }
    }

}