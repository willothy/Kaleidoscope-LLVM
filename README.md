# Kaleidoscope-LLVM

Following along with the "My First Language Frontend with LLVM" tutorial from LLVM.org.

Example usage (from LLVM.org)

    $ ./a.out
    ready> def foo(x y) x+foo(y, 4.0);
    Parsed a function definition.
    ready> def foo(x y) x+y y;
    Parsed a function definition.
    Parsed a top-level expr
    ready> def foo(x y) x+y );
    Parsed a function definition.
    Error: unknown token when expecting an expression
    ready> extern sin(a);
    ready> Parsed an extern
    ready> ^D
    $
