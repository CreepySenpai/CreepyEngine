using System.Numerics;

namespace Creepy;

public abstract class Component {
    public Entity? entity{get; internal set;}
}

public class IDComponent : Component {

}

public class TagComponent : Component {

}

public class TransformComponent : Component {
    public Vector3 Position {
        get{
            unsafe
            {
                InternalCalls.TransformComponent_GetPosition(entity.UUID, out Vector3 position);
                return position;
            }
        }

        set {
            unsafe
            {

                InternalCalls.TransformComponent_SetPosition(entity.UUID, ref value);

            }
        }
    }

    public Vector3 Rotation {
        get{
            unsafe
            {
                InternalCalls.TransformComponent_GetRotation(entity.UUID, out Vector3 rotation);
                return rotation;
            }
        }

        set {
            unsafe
            {

                InternalCalls.TransformComponent_SetRotation(entity.UUID, ref value);

            }
        }
    }

    public Vector3 Scale {
        get{
            unsafe
            {
                InternalCalls.TransformComponent_GetScale(entity.UUID, out Vector3 scale);
                return scale;
            }
        }

        set {
            unsafe
            {

                InternalCalls.TransformComponent_SetScale(entity.UUID, ref value);

            }
        }
    }
}

public class SpriteComponent : Component {

}

public class CircleSpriteComponent : Component {

}

public class CameraComponent : Component {

}

public class NativeScriptComponent : Component {

}

public class ScriptComponent : Component {
    public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPoint, bool wake){

    }

    public void ApplyLinearImpulseToCenter(Vector2 impulse, bool wake){

    }
}

public class RigidBody2DComponent : Component {

}

public class BoxCollider2DComponent : Component {

}