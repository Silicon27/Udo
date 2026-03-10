# `GrammarStream`


```c++
std::string id;
std::vector<std::string> args;

auto rule = 
    identifier(id) 
    >> ch('(')
    >> optional(
        seq(
            ch(',') 
            >> 
            identifier(args)
        ))
    >> ch(')')
```

This parses a simple expression similar to the following 
```c++
my_id(arg1, arg2)
```
where `id = "my_id"` and `args = ["arg1", "arg2"]`

This is thereafter easily traversable and recoverable upon parsing fault.

Given some input `this(,)` to our `rule`:
```c++
Recovery r(diag_engine); // pre-defined DiagnosticEngine instance, any errors spotted on recovery and subsequent hints are stored to this engine
std::vector<Token> toks = {"this", "(", ",", ")"};

rule.parse(toks, 0) // this faults! 

if (rule.is_faulty()) {
    r.recover(rule);
}
```

## Divergent grammars

If divergence exists in a grammar, say some grammar may be divergent as:
```rust
this() :: i32;

this() :: i32 {

}

this() {

}
```
where past `)` the grammar diverges. Dispatchment of matching grammar rules depends proceeding tokens. In this situation, lookahead with runtime based dispatch is used. `rule` is specified thereby as:

```c++

Branch branch1 = 
    str("::")
    >> type() // custom type() grammar generator
    >> ch(';');

Branch branch2 = 
    str("::")
    >> type() // custom type() grammar generator
    >> block();

Branch branch3 = block(); // a block is code enclosed (default) with {} (otherwise specifiable via passing block(opening, closing))

std::string id;

auto rule = 
    identifier(id)
    >> args
    >> divergent(branch1, branch2, branch3);
```

To know which branch was ran we check if the branch was active:
```c++
if (branch1.is_active()) {
    ...
} else if (branch2.is_active()) {
    ...
} else if (branch3.is_active()) {
    ...
}
```