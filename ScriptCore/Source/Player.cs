using System;

namespace Creepy;


public class Player : Entity {

    void OnCreate(){
        Console.WriteLine($"Create Player {UUID}");
    }

    void OnUpdate(float timeStep){
        Console.WriteLine("Player Update");
    }

}