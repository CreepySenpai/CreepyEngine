using System;
using System.Numerics;


namespace Creepy;

public class Entity
{

    public readonly ulong UUID;

    protected Entity()
    {
        UUID = 0;
    }

    internal Entity(ulong uuid)
    {
        UUID = uuid;
    }

    public Vector3 Position
    {
        get
        {

            unsafe
            {
                InternalCalls.Entity_GetPosition(UUID, out Vector3 position);
                return position;
            }

        }
        set
        {

            unsafe
            {

                InternalCalls.Entity_SetPosition(UUID, ref value);

            }

        }
    }



}
