
using System.Numerics;
using Creepy.Attributes;

namespace Creepy.Runtime;

public class Camera : Entity {
    
    public float distanceToPlayer = 15.0f;

    public override void OnCreate()
    {
        Console.WriteLine($"Create Camera {UUID}");
    }

    public override void OnUpdate(float timeStep)
    {
        Entity player = GetEntityByName("Player");

        if(player != null){
            Position = new Vector3(player.Position.X, player.Position.Y, distanceToPlayer);
        }
    }
    
}