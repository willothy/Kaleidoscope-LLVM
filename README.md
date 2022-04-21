# Kaleidoscope-LLVM

Following along with the "My First Language Frontend with LLVM" tutorial from LLVM.org.

Example usage (from LLVM.org)

    $ ready> 2 + 2;
    Read top-level expression:
    define double @__anon_expr() {
    entry:
      ret double 4.000000e+00
    }

    $ ready> extern test();
    Read extern:
    declare double @test()

    $ ready> def main(x) test() + x;
    Parsed a function definition:
    define double @main(double %x) {
    entry:
      %calltmp = call double @test()
      %addtmp = fadd double %calltmp, %x
      ret double %addtmp
    }
