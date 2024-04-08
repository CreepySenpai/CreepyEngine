using Creepy.NativeDataType;
using Creepy.Runtime;
using System.Numerics;

namespace Creepy;

internal static class InternalCalls {

    internal static unsafe delegate* <KeyCode, bool> Input_IsKeyPressed;
    
    internal static unsafe delegate* <ulong, NativeString, bool> Entity_HasComponent;

    internal static unsafe delegate* <NativeString, ulong> Entity_GetEntityByName;

    internal static unsafe delegate* <ulong, NativeInstance<Entity>> Entity_GetInstance;

    internal static unsafe delegate* <ulong, out Vector3, void> TransformComponent_GetPosition;

    internal static unsafe delegate* <ulong, ref Vector3, void> TransformComponent_SetPosition;

    internal static unsafe delegate* <ulong, out Vector3, void> TransformComponent_GetRotation;

    internal static unsafe delegate* <ulong, ref Vector3, void> TransformComponent_SetRotation;

    internal static unsafe delegate* <ulong, out Vector3, void> TransformComponent_GetScale;

    internal static unsafe delegate* <ulong, ref Vector3, void> TransformComponent_SetScale;

    
}
