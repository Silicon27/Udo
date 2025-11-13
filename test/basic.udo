// Udo classifies functions as tables, which holds pointers to different functors, structs and variables
fn square(int x) -> int {
    // special type callable which is here declared as a functor that captures all variables in the scope above
    // first dot is which scope (. being current, .. being one back, and :: for other namespaces/scopes accessible to this function)
    // and second dot being what to capture, in this case being all
    functor callable -> int [. .]{
        // enables external resources to call this function
        return x**2
    }
}

fn my_type() -> type {
    struct {
        .int x = 42;
    }
}

fn entry() {
    square::(4); // calls the callable functor in square, same as call square::callable
    return 0;
}