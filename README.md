# dumb\_lib.h

This is a single-file (or "header-only") MIT licensed library for C/C++,
meant primarily for:
+ A) My personal use, as a "C extension"/my own "standard library"
+ B) My own learning purposes

That means that the library is still very much "under construction",
and also, that I will most likely not accept any pull requests.
(I may change my mind on that, obviously)

Thus, I ask you: if you want to use it, **READ THROUGH THE CODE, VALIDATE IT,**
**TEST IT OUT YOURSELF AND TAKE THE "NO WARRANTY..." PART OF THE LICENSE SERIOUSLY.**

(For this reason, I may never take this lib over the v1.0 mark - we'll see)

### Tested on

[2025/09/05]
#### Windows 11:
- [x] Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35213 for x64
- [x] clang version 18.1.8 (Target: x86\_64-pc-windows-msvc)
- [x] gcc (x86\_64-win32-seh-rev0, Built by MinGW-Builds project) 15.2.0
- [x] tcc version 0.9.27 (x86\_64 Windows)

#### Linux:
MISSING

#### MacOS:
MISSING

### NOTES

If you're wondering why there are no "//" comments, it's because they are not part
of the C89 standart. For rationale to comply with C89 see [Dependable C](https://www.dependablec.org/)

If you are wondering why I decided to use single-header style for the library, see [Sean Barrett's explanation](https://github.com/nothings/stb?tab=readme-ov-file#why-single-file-headers)
