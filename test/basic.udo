// Udo classifies functions as tables, which holds pointers to different functors, structs and variables
fn square(x: i32) -> i32 {
    // special type callable which is here declared as a functor that captures all variables in the scope above
    // first dot is which scope (. being current, .. being one back, and :: for other namespaces/scopes accessible to this function)
    // and second dot being what to capture, in this case being all
    functor callable -> i32 [. .]{
        // enables external resources to call this function
        return x**2
    }
}

fn my_type() -> type {
    struct {
        let i32 x = 42;
    }
}

fn main() {
    square(4); // calls the callable functor in square, same as call square::callable
    return 0;
}