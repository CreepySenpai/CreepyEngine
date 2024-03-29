using System.Runtime.CompilerServices;
using System;
using ScriptCore.NativeDataType;

namespace ScriptCore;

public struct MyVec3{
    public float x, y, z;

    public MyVec3(float _x, float _y, float _z){
        x = _x;
        y = _y;
        z = _z;
    }
}

internal static class InternalCalls {
    internal static unsafe delegate* <int, void> TestInternalCall;

    internal static unsafe delegate* <ref MyVec3, void> TestPrintVec3;

    internal static unsafe delegate* <in MyVec3, void> TestPrintConstVec3;
}


public class Main
{
    
    private NativeString myName;
    public Main(NativeString Name){
        myName = Name;
        Console.WriteLine("Call Constructor");

        MyVec3 v = new MyVec3(1.0f, 10.0f, 69.0f);
        
        unsafe{
            InternalCalls.TestPrintVec3(ref v);

            Console.WriteLine("Native Change");

            InternalCalls.TestPrintVec3(ref v);
        }
    }

    public void PrintName() => Console.WriteLine($"My Name Is {myName.ToString()}");

    public void PrintInternal(int i){

        unsafe{
            InternalCalls.TestInternalCall(i);
        }

    }

    public void CallPrintConstVec3(){
        MyVec3 v = new(1.0f, 6.0f, 90.0f);
        
        unsafe {
            InternalCalls.TestPrintConstVec3(v);
        }
    }
}
