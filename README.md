# dumb\_lib

dumb\_lib(raries) is a collection of single-file (or "header-only") MIT licensed libraries for C/C++,
meant primarily for:
+ A) My personal use, as a "C extension"/my own "standard library"
+ B) My own learning purposes

That means that the libraries are still very much "under construction",
and also, that I will most likely not accept any pull requests with new functionality.
(I may change my mind on that, obviously.
Pull requests adding tests, typo-fixes etc.
are welcome & very much appreciated, though.)

Thus, I ask you: if you want to use any of the libraries,
**READ THROUGH THE CODE, VALIDATE IT, TEST IT OUT YOURSELF AND**
**TAKE THE "NO WARRANTY..." PART OF THE LICENSE SERIOUSLY.**

(For this reason, I may never take this lib collection over the v1.0 mark - we'll see)

### NOTES

- Originally, I pretty much tried to copy the stb libraries style
(i.e. "define your own allocator" etc.), but since these libraries are
meant basically just for my own use, I decided to make it more opinionated.

Once again, that basically means: DO NOT USE ANY OF THE LIBRARIES!
Of course you are free to do so, or to read through the source,
yank the parts you like out etc.

- If you're wondering why there are no "//" comments, it's because they are not part
of the C89 (ANSI C/ISO C99) standard.
For rationale to comply with C89 see [Dependable C](https://www.dependablec.org/)

- If you are wondering why I decided to use single-header style for the libraries, see
[Sean Barrett's explanation](https://github.com/nothings/stb?tab=readme-ov-file#why-single-file-headers)

## Libraries:

### dumb\_lib.h
The original library, what I consider to be the "core" - memcpy, memcmp & memset, Memory Arena, Dynamic Array, String.

Tests: [TEST\_LOG_\_dumb\_lib.txt](TEST_LOG_dumb_lib.txt)

### dumb\_file.h
Basic file I/O & utilities.

Tests: [TEST\_LOG_\_dumb\_file.txt](TEST_LOG_dumb_file.txt)
