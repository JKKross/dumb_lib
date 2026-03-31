# dumb\_lib

dumb\_lib(raries) is a collection of single-file (or "header-only") MIT licensed libraries for C/C++,
meant primarily for:
+ A) My personal use, as a "C extension"/my own "standard library".
+ B) My own learning purposes.

That means that the libraries are still very much "under construction",
and also, that I will most likely not accept any pull requests with new functionality.
(I may change my mind on that, obviously.
Pull requests adding tests, typo-fixes etc.
are welcome & very much appreciated, though.)

Thus, I strongly recommend: **DO NOT USE THESE LIBRARIES!**
Instead, I would like to encourage anyone reading this
to go through the learning process & create their own utility libraries.

Recommended learning resources for those who would like to do so,
but don't know where to start are:

+ Read the ["K&R C" book](https://en.wikipedia.org/wiki/The_C_Programming_Language) - a
lot of the concepts & practices are outdated, but it's still a great resource for anything C, in my opinion.
+ Casey Muratori's ["Handmade Hero"](https://youtu.be/Ee3EtYb8d1o) series (first 20-50 episodes is all you need to get going).
+ Eskil Steenberg's ["How I program C"](https://youtu.be/443UNeGrFoM).
+ Ryan Fleury's blog ["Digital Grove"](https://substack.com/@rfleury/posts) &
[RadDebugger's source code](https://github.com/EpicGamesExt/raddebugger).

Also worth checking out:
+ Sean Barrett's ["stb libraries"](https://github.com/nothings/stb).
+ Jonathan Blow's ["Preventing the Collapse of Civilization"](https://www.youtube.com/watch?v=ZSRHeXYDLko) talk.
+ ["Better Software Conference"](https://youtube.com/@bettersoftwareconference) YouTube channel.
+ Stanford lecture series ["Programming Paradigms"](https://www.youtube.com/playlist?list=PLD28639E2FFC4B86A).

Alas, if you insist on using any of these libraries,
**READ THROUGH THE CODE, VALIDATE IT, TEST IT OUT YOURSELF AND**
**TAKE THE "NO WARRANTY..." PART OF THE LICENSE SERIOUSLY.**

(For these reasons, I may never take this lib collection over the v1.0 mark - we'll see)

## Notes

+ Originally, I pretty much tried to copy the stb libraries style
(i.e. "define your own allocator" etc.), but since these libraries are
meant just for my own use, I decided to make it more opinionated.

    Once again, that basically means: DO NOT USE ANY OF THE LIBRARIES!
    Of course you are free to do so, or to read through the source,
    yank the parts you like out etc.

+ If you're wondering why there are no "//" comments, it's because they are not part
of the C89 (ANSI C/ISO C89) standard.
Main reason to try to comply with C89 is to be able to compile on basically any platform with any compiler.
For more rationale to comply with C89 see [Dependable C](https://www.dependablec.org/)

+ If you are wondering why I decided to use single-header style for the libraries, see
[Sean Barrett's explanation](https://github.com/nothings/stb?tab=readme-ov-file#why-single-file-headers)

## Documentation

Since I'm still experimenting and the libs are not even close to being stable,
there's intentionally not much documentation at the moment.
What little documentation there is can be found in the source files themselves
(including how to properly include the desired library in the project).

## Libraries

### dumb\_lib.h
The original library, what I consider to be the "core" - memcpy, memcmp & memset, Memory Arena, Dynamic Array, String.

Tests: [TEST\_LOG\_dumb\_lib.txt](TEST_LOG_dumb_lib.txt)

### dumb\_file.h
Basic file I/O & utilities.

Tests: [TEST\_LOG\_dumb\_file.txt](TEST_LOG_dumb_file.txt)
