```rust
union overload {
    let x: i32,
    let y: char
} 
// or alternatively you could declare an inline union faster via:
inline overload union x{
    [i32,char],
    bool
}

my_func(union x) :: i32$ 
dispatch -> {

} char -> {

}
```
