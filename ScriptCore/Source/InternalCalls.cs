using System.Runtime.CompilerServices;
using System;
using Creepy.NativeDataType;
using System.Numerics;

namespace Creepy;

internal static class InternalCalls {
    internal static unsafe delegate* <ulong, out Vector3, void> Entity_GetPosition;

    internal static unsafe delegate* <ulong, ref Vector3, void> Entity_SetPosition;
    
    internal static unsafe delegate* <KeyCode, bool> Input_IsKeyPressed;
    
}
