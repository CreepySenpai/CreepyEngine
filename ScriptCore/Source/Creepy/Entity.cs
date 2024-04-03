using System;
using System.Numerics;
using Creepy.NativeDataType;

namespace Creepy;


public class Entity
{
    public readonly ulong UUID;

    protected Entity()
    {
        UUID = 0;
    }

    public virtual void OnCreate(){}

    public virtual void OnUpdate(float timeStep){}

    protected internal Entity(ulong uuid)
    {
        UUID = uuid;
    }

    public Vector3 Position
    {
        get {
            
            unsafe {
                InternalCalls.TransformComponent_GetPosition(UUID, out Vector3 position);
                return position;
            }
        }
        
        set {
            
            unsafe {
                InternalCalls.TransformComponent_SetPosition(UUID, ref value);
            }
        }
    }

    public bool HasComponent<T>() where T : Component, new() {
        Type componentType = typeof(T);
        unsafe {
            return InternalCalls.Entity_HasComponent(UUID, (NativeString)componentType.FullName);
        }
    }

    public T GetComponent<T>() where T : Component, new() {
        
        if(!HasComponent<T>()){
            return null;
        }
        
        T component = new T(){entity = this};

        return component;
    }
    
}
