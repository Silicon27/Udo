# Expressions
Identifiers or values that are permitted to be present as part of an *expression* may be operable (meaning operator application must be valid on said identifier/value). We thereby refer to them as ***xapproved values*** (expression approved values). This implies  that all values present in any given expression is required to be *xapproved*, otherwise the program errors.




### Sub-expressions
An xapproved value is classified as a *sub-expression* of a larger expression group if any there exists at least one other operator and xapproved value within the larger expression group. 

Example:
```udo
let y: i32 = 1; // y and 1 are xapproved values
let x: i32 = y + 1; // since y and 1 are xapproved values this results in an expression that returns a new object
let z: i32 = y; // error, xapproved value y must be a sub-expression when directly assigning
```
> `z` errors since direct copies are not permitted in Udo, any one value has only one concurrent owner. `z` can copy `y` by instead manually TODO reconsider whether to allow direct copies and how that would look for operator overloads 
