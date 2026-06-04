# Number

An arbitrary precision arithmetic library written in C. It provides extensive support for multiple mathematical representations and operations, optimized with advanced multiplication algorithms

## Modules

The library is organized into several distinct sub-modules under `lib/`, each tailored for different numerical representations:

- **`num` (BigInt):** Arbitrary precision unsigned integers. Supports core arithmetic (add, subtract, multiply, divide, modulo, power, square root), bitwise operations (shift left/right), and base conversions.
- **`sig` (Signed Integers):** Arbitrary precision signed integers.
- **`fxd` (Fixed-Point):** Arbitrary precision fixed-point numbers for high-precision fractional calculations.
- **`flt` (Floating-Point):** Arbitrary precision floating-point numbers.
- **`mod` (Modular Arithmetic):** Specialized numbers and operations for modular arithmetic.

## Features

- **Advanced Mathematics:** Compute Pi (`π`), Euler's number (`e`), square roots, Fibonacci sequences, and factorials to arbitrary limits.
- **Fast Multiplication:** Implements classical multiplication alongside more advanced algorithms for handling very large numbers efficiently.
- **Base Conversion:** Built-in functions for reading and displaying decimals as well as base-N conversion.
- **Memory Tracking:** Integrates with `clu` (Command Line Utilities) for memory tracing and maximum occupancy tracking during heavy operations (debug mode only).

## Directory Structure

- `lib/` - Core library modules (`num`, `fxd`, `flt`, `sig`, `mod`).
- `src/` - Main executable and example calculations (e.g., Pi algorithms, Fibonacci generators).
- `mods/` - Submodule dependencies including `clu` and `macros`.
- `makefiles/` - Modular Make configuration used across the library.

## Building and Running

The project uses modular `Makefile` configurations.

To build the primary target, simply run:
```bash
make
```

To run the main executable:
```bash
./run.sh
```

To run with debugging enabled (which may provide memory tracking and assert checks):
```bash
./run_debug.sh
```

## Memory Management & Ownership

A critical design pattern in this library is how it handles memory and ownership of allocated numbers:

- **Operations Consume Inputs:** Most arithmetic operations (e.g., `num_add`, `num_mul`, `num_pow`) **consume** their input arguments. They take ownership of the pointers and will automatically free them or reuse their memory. If you need to retain a variable for later use, you must explicitly pass a copy using `num_copy()`.
- **Comparisons & Display Do Not Consume:** Read-only operations, such as comparisons (`num_cmp`), display functions (`num_display_dec`), and checks (`num_is_zero`), **do not** consume their inputs.

## Usage Example

Operations typically involve wrapping primitive types or allocating strings, performing arithmetic, and explicitly freeing the allocated objects. Check `src/main.c` for practical demonstrations like generating Pi or large Fibonacci strings.

```c
#include "lib/num/header.h"

int main() {
    // Wrap a small integer into a BigInt structure
    num_p a = num_wrap(2);
    
    // Raise to a power (e.g., generating a large number)
    num_p b = num_pow(a, 100); 
    
    // Display in decimal format
    num_display_dec(b);
    
    // Memory cleanup
    num_free(b);
    return 0;
}
```
