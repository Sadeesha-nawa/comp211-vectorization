# Compiler vectorization in a wave update

This COMP 211 Checkoff 3 experiment uses a simplified wave-field update related to walking-droplet simulations. At each sampled location, the program adds the contributions from the newest bounce to the decayed field left by previous bounces. This process is represented by the following formula.

```c
updated[i] = decayed[i] + bounce[i];
```

We use Clang as our compiler for this experiment since it supports vectorization. Here, the main idea is to use Clang to combine repeated SIMD instructions into a single instruction. SIMD stands for single instruction, multiple data.

## Run the experiment

On macOS or Linux with Clang installed, run the following from the repository root.

```sh
sh run.sh
```

This script compiles the wave update at the positions `-O0`, `-O1`, `-O2`, and `-O3`. It also compiles an `-O3` control with compiler vectorization disabled. The benchmark keeps the driver fixed at `-O2` and uses data with 262,147 floats per array. Each run warms up the function, times 500 calls, and checks every output element. The summary reports the median of five runs for each build.

The assembly is in `build/*.s`, and individual timings are in `build/timings.tsv`.

## What to look for

- On an Apple Silicon reference build, Clang reported a vector width of four at `-O2` and `-O3`. In the generated assembly, `fadd.4s` adds four pairs of single-precision values. A scalar `fadd` adds one pair.
- The reference build handles the remaining three elements with scalar instructions after all complete groups of four.
- Comparing the two `-O3` builds shows the timing effect of turning vectorization on for this loop while keeping the optimization level fixed. Timings vary with the machine and compiler.

## Presentation

[Checkoff slides](slides/vectorization.pptx)

## References

- [LLVM documentation on auto-vectorization](https://llvm.org/docs/Vectorizers.html)
- [A Wave Waltz of Droplets, APS Gallery of Fluid Motion](https://gfm.aps.org/meetings/dfd-2025/692e39eca7f805227b16ff6a)
