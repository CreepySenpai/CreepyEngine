using System;
using System.Numerics;

namespace Creepy;


public class Player : Entity {
    
    private float speed = 0.1f;

    void OnCreate(){
        Console.WriteLine($"Create Player {UUID}");
    }

    void OnUpdate(float timeStep){
        Vector3 pos = Position;

        if(Input.IsKeyPressed(KeyCode.KEY_W)){
            pos.X += speed * timeStep;
        }
        else if(Input.IsKeyPressed(KeyCode.KEY_S)) {
            pos.X -= speed * timeStep;
        }
        else if(Input.IsKeyPressed(KeyCode.KEY_A)) {
            pos.Y -= speed * timeStep;
        }
        else if(Input.IsKeyPressed(KeyCode.KEY_D)) {
            pos.Y += speed * timeStep;
        }

        
        Position = pos;
        Console.WriteLine($"Pos: {Position.X}, {Position.Y}, {Position.Z}");
        

    }

}