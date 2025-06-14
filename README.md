# brainc
A simple (at the moment) Brainfuck compiler, that produces [MOS Technology 6502](https://en.wikipedia.org/wiki/MOS_Technology_6502) machine code.

To execute the emitted code, use a 6502 emulator (such as [Symon](https://github.com/sethm/symon)).
There's also an emulator in the works, but we'll see how it goes.

Features:
 - Fast (**TODO** bench :))
 - Supports all Brainfuck operations (including loops)
 - Supports keyboard I/O _that was hard_
 - Not recognized symbols are comments
 - Allows assembly preview of emitted code using the `-v` flag
 
Future plans:
 - Output code optimization (interesting research by [@matslina](https://github.com/matslina) et al.).
 - Multiple target platforms (maybe Z80, for example?)
 - Better display of emitted code (e.g. sections)

## Usage
Compile the compiler _you think you're funny_:

``` sh
$ gcc src/*.c -std=c99 -o brainc
```

And execute with an input and output file paths:

``` sh
$ ./brainc examples/mul_input.bf out.o65
```

Now the output is written to `out.o65` and you can feed it to your emulator.

For overall usage, just run `./brainc` to see the **Usage** page.

### Note on emulator support
Different emulators have different memory mappings (among other things).
By default, the compiler is checked to work with the Symon emulator.
However, other emulators should work just as well, as long as the memory map is set up correctly in the compiler source code (configuration options will be available in the future).

To verify / change the mapping, see `src/memory_map.h`, which should have all necessary memory mappings.

## Contributing
See [`CONTRIBUTING.md`](/CONTRIBUTING.md).
