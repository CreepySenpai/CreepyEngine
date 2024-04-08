using System;
using System.Numerics;
using Creepy.NativeDataType;
using Creepy;

namespace Creepy.Runtime;


public class Entity
{
    public readonly ulong UUID;

    protected Entity()
    {
        UUID = 0;
    }

    protected internal Entity(ulong uuid)
    {
        UUID = uuid;
    }

    public virtual void OnCreate(){}

    public virtual void OnUpdate(float timeStep){}

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

    public bool IsExits(){
        return UUID != 0;
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

    public Entity GetEntityByName(string entityName){

        unsafe {
            NativeString nativeName = entityName;
            ulong entityID = InternalCalls.Entity_GetEntityByName(nativeName);

            if(entityID == 0){
                return null;
            }

            return new Entity(entityID);
        }

    }

    public T As<T>() where T : Entity {
        unsafe {
            var instance = InternalCalls.Entity_GetInstance(UUID);
            return instance.Get() as T;
        }
    }
}
