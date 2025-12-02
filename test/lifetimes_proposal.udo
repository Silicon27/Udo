let x: i32 = 32;
//fn myfunc(this: ref[i32])

// Lifetimes are determined by the scope the variable was declared in.
// That scope thereafter has the responsibility to call that object's remove.
// You can change the scope that has this responsibility with the lifetime keyword

lifetime this_life {
    let lifetime_var: i32 = 32; // currently hooked to this_life
    lifetime lifetime_var ->  other_life; // attach lifetime_var lifetime responsibility to other_life
}
// outside of some_life and other_life scopes, lifetime_var is still invalid
lifetime other_life {
    // lifetime_var deleted here
}

// this behaviour of course can also be applied to functions that return refs to local variables
fn myfunc() => mut ref i32 {
    let x: i32 = 32;
    lifetime x -> other_life;
    return ref x;
}'

fn