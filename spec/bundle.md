# Bundle

As opposed to traditional methods of member bundling (e.g. structs, classes, etc), Udo uses `bundle`s. 

Example:
```rust

bundle A {
    x: i32,
    y: i32
}

make_a(a: i32, b: i32) :: A {
    return A |a b|;
}

main() :: i32 {
    reg x as A =
}
```