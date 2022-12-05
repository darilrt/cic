# CIC [WIP]

CIC is a language that compiles to c++ code.

## Example

```cic
import "iostream";

main() -> int {
    std::cout << "Hello, World!" << std::endl;
    ret 0;
}
```