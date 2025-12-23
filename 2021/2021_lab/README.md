## Building
We use CMake as a build system (packages cmake and cmake-curses-gui are handy). To build a Debug version (that is **unobfuscated**) out of tree, do:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

And you can find all executables (task1, etc.) in `bin/`, while the shared library is in `lib/`. You can then switch to a Release (**obfuscated**!) build using `cmake -DCMAKE_BUILD_TYPE=Release ..` inside the build directory.

## Obfuscation
Currently, every task (A through E) contains unobfuscated code (`main.c`), the Tigress-obfuscated code (`obf.c`), and the edited obfuscated code (`obf-edited.c`) from which the Release version is actually built. Manual source-code obfuscations thus happen by editing `obf-edited.c`.

If at any time you want to re-apply the Tigress obfuscation (e.g., because `main.c` changed) you can do this through the following command: `make obfuscate-tasks` . As a result, every `obf.c` and `obf-edited.c` will be overwritten. **WARNING**: Back up your manual changes to the edited obfuscated code if you want to keep them.
