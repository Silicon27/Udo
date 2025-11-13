# Udo 
Udo is a compiled and imperative language aimed at provisioning requested memory whilst allowing maximum control.

Memory provisioning is an operation the compiler provides to requested memory 

Example code snippet:

```c++
@use <core/io>

main :: void => { // no arguments must be marked as void
    int v = 1;
    io.print(v);
    return 0;
}

```


### Concepts
Udo introduces `contracts`, which is a form of allowance or constraint between the programmer and the compiler.

```c++
@use <core/contracts>

// form alias if needed
// contract nopvs = contracts.nopvs

// nopvs is a compiler promise to never provision memory used in my_ctr
<contract #contacts.nopvs> // no provisioning, dangerous
execution my_ctr int: (int x) { // int is defined as an alias of int32
    return x;
}
```

> A `contract`defines an allowance or constraint between the programmer and the compiler. 
> 
> the `execution` of said contract is formed similar to that of a function
The cboard (contract board) keeps tab on contracts present at CET, or, Contract Enforcement Time.