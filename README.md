# CIC [WIP]

CIC is a language that compiles to c++ code. Focused in c++17.

## Hello, World!

```cic
include "iostream";

main() -> int {
    std::cout << "Hello, World!" << std::endl;
    ret 0;
}
```

### Variables
The variables are declared with the `mut` or `let` keyword.
`mut` means that the variable can be changed,`let` means that the variable can't be changed.

```cic
let a : int = 5; // 'a' is a constant
mut b : int = 5; // 'b' is a mutable

mut c : const int* = 5; // 'c' is a pointer to a constant
```

### Functions
Functions do not need a keyword to be declared.

```cic
// The function 'add' takes two integers and returns an integer
add(a : int, b : int) -> int {
    ret a + b;
}
```

You can define function templates with the `<` and `>` symbols and define parameters inside the symbols.

```cic
add<T : type>(a : T, b : T) -> T {
    ret a + b;
}
```

### Classes
Classes are declared with the `class` keyword.
The properties or methods of a class are declared like variables or functions.
If you want to declare the protectionb level of a property or method you can use the `pub` or `priv` keyword, by default the properties and methods are protected.
Also you can define operators for your class with `op` keyword.

`.` is used to access the properties or methods of the class, it's translated to `this->` in c++.

```cic
class Point {
    pub mut x : int;
    pub mut y : int;

    pub Point(x : int, y : int) {
        .x = x;
        .y = y;
    }

    pub add(p : Point) -> Point {
        ret Point(.x + p.x, .y + p.y);
    }

    pub op + (p : Point) -> Point {
        ret .add(p);
    }
}
```
You can also define a class template
    
```cic
class Point<T : type> {
    pub mut x : T;
    pub mut y : T;

    pub Point(x : T, y : T) {
        .x = x;
        .y = y;
    }

    pub add(p : Point<T>) -> Point<T> {
        ret Point<T>(.x + p.x, .y + p.y);
    }

    pub op + (p : Point<T>) -> Point<T> {
        ret .add(p);
    }
}

// usage
let p1 : Point<int> = Point<int>(1, 2);
```

For inheritance you can use the `(...)` symbol.

```cic
class Base { ... }
class DerivedA(pub Base) { ... }
class DerivedB(priv Base) { ... }
```

### Enum
The enum is declared with the `enum` keyword.

```cic
enum Color {
    Red = 0,
    Green,
    Blue
}
```

### 