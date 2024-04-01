namespace Creepy;


public static class Input {
    
    public static bool IsKeyPressed(KeyCode keyCode) {

        unsafe {
            return InternalCalls.Input_IsKeyPressed(keyCode);
        }
        
    }

}