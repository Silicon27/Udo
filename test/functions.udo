fn my_identifier(arg1: mut ref i32) =>  i32 {
    return *arg1 + 1;
}

fn main() => nothing {
    let x: i32; // default init is called (no args) and sets x to 0
    let y: i32 = my_identifier(x);
}