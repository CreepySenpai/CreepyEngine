using System;
using ScriptCore.NativeDataType;

namespace ScriptCore;

public class MyTest
{
    private NativeString myName;
    public MyTest(NativeString Name){
        myName = Name;
        Console.WriteLine("Call Constructor");
    }

    public NativeString GetName() => myName;

    public void SetName(NativeString n) => myName = n;

    public void PrintName() => Console.WriteLine($"My Name Is {myName.ToString()}");

    public static void NahNah() => Console.WriteLine("Dit Me Tuyet Voi");
}
