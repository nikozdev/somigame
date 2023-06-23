stb_image - v2.28 - public domain image loader - http://nothings.org/stb

no warranty implied; use at your own risk

You can #define STBI_ASSERT(x) before the #include to avoid using assert.h.
And #define STBI_MALLOC, STBI_REALLOC, and STBI_FREE to avoid using malloc,realloc,free

QUICK NOTES:
    Primarily of interest to game developers and other people who can
    avoid problematic images and only need the trivial interface

    JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
    PNG 1/2/4/8/16-bit-per-channel

    TGA (not sure what subset, if a subset)
    BMP non-1bpp, non-RLE
    PSD (composited view only, no extra channels, 8/16 bit-per-channel)

    GIF (*comp always reports as 4-channel)
    HDR (radiance rgbE format)
    PIC (Softimage PIC)
    PNM (PPM and PGM binary only)

    Animated GIF still needs a proper API, but here's one way to do it:
        http://gist.github.com/urraka/685d9a6340b26b830d49

    - decode from memory or through FILE (define STBI_NO_STDIO to remove code)
    - decode from arbitrary I/O callbacks
    - SIMD acceleration on x86/x64 (SSE2) and ARM (NEON)

REVISION HISTORY:

    2.28  (2023-01-29) many error fixes, security errors, just tons of stuff
    2.27  (2021-07-11) document stbi_info better, 16-bit PNM support, bug fixes
    2.26  (2020-07-13) many minor fixes
    2.25  (2020-02-02) fix warnings
    2.24  (2020-02-02) fix warnings; thread-local failure_reason and flip_vertically
    2.23  (2019-08-11) fix clang static analysis warning
    2.22  (2019-03-04) gif fixes, fix warnings
    2.21  (2019-02-25) fix typo in comment
    2.20  (2019-02-07) support utf8 filenames in Windows; fix warnings and platform ifdefs
    2.19  (2018-02-11) fix warning
    2.18  (2018-01-30) fix warnings
    2.17  (2018-01-29) bugfix, 1-bit BMP, 16-bitness query, fix warnings
    2.16  (2017-07-23) all functions have 16-bit variants; optimizations; bugfixes
    2.15  (2017-03-18) fix png-1,2,4; all Imagenet JPGs; no runtime SSE detection on GCC
    2.14  (2017-03-03) remove deprecated STBI_JPEG_OLD; fixes for Imagenet JPGs
    2.13  (2016-12-04) experimental 16-bit API, only for PNG so far; fixes
    2.12  (2016-04-02) fix typo in 2.11 PSD fix that caused crashes
    2.11  (2016-04-02) 16-bit PNGS; enable SSE2 in non-gcc x64
                       RGB-format JPEG; remove white matting in PSD;
                       allocate large structures on the stack;
                       correct channel count for PNG & BMP
    2.10  (2016-01-22) avoid warning introduced in 2.09
    2.09  (2016-01-16) 16-bit TGA; comments in PNM files; STBI_REALLOC_SIZED

   rision history:
    2.20  (2019-02-07) support utf8 filenames in Windows; fix warnings and platform ifdefs
    2.19  (2018-02-11) fix warning
    2.18  (2018-01-30) fix warnings
    2.17  (2018-01-29) change sbti__shiftsigned to avoid clang -O2 bug
                       1-bit BMP
                       *_is_16_bit api
                       avoid warnings
    2.16  (2017-07-23) all functions have 16-bit variants;
                       STBI_NO_STDIO works again;
                       compilation fixes;
                       fix rounding in unpremultiply;
                       optimize vertical flip;
                       disable raw_len validation;
                       documentation fixes
    2.15  (2017-03-18) fix png-1,2,4 bug; now all Imagenet JPGs decode;
                       warning fixes; disable run-time SSE detection on gcc;
                       uniform handling of optional "return" values;
                       thread-safe initialization of zlib tables
    2.14  (2017-03-03) remove deprecated STBI_JPEG_OLD; fixes for Imagenet JPGs
    2.13  (2016-11-29) add 16-bit API, only supported for PNG right now
    2.12  (2016-04-02) fix typo in 2.11 PSD fix that caused crashes
    2.11  (2016-04-02) allocate large structures on the stack
                       remove white matting for transparent PSD
                       fix reported channel count for PNG & BMP
                       re-enable SSE2 in non-gcc 64-bit
                       support RGB-formatted JPEG
                       read 16-bit PNGs (only as 8-bit)
    2.10  (2016-01-22) avoid warning introduced in 2.09 by STBI_REALLOC_SIZED
    2.09  (2016-01-16) allow comments in PNM files
                       16-bit-per-pixel TGA (not bit-per-component)
                       info() for TGA could break due to .hdr handling
                       info() for BMP to shares code instead of sloppy parse
                       can use STBI_REALLOC_SIZED if allocator doesn't support realloc
                       code cleanup
    2.08  (2015-09-13) fix to 2.07 cleanup, reading RGB PSD as RGBA
    2.07  (2015-09-13) fix compiler warnings
                       partial animated GIF support
                       limited 16-bpc PSD support
                       #ifdef unused functions
                       bug with < 92 byte PIC,PNM,HDR,TGA
    2.06  (2015-04-19) fix bug where PSD returns wrong '*comp' value
    2.05  (2015-04-19) fix bug in progressive JPEG handling, fix warning
    2.04  (2015-04-15) try to re-enable SIMD on MinGW 64-bit
    2.03  (2015-04-12) extra corruption checking (mmozeiko)
                       stbi_set_flip_vertically_on_load (nguillemot)
                       fix NEON support; fix mingw support
    2.02  (2015-01-19) fix incorrect assert, fix warning
    2.01  (2015-01-17) fix various warnings; suppress SIMD on gcc 32-bit without -msse2
    2.00b (2014-12-25) fix STBI_MALLOC in progressive JPEG
    2.00  (2014-12-25) optimize JPG, including x86 SSE2 & NEON SIMD (ryg)
                       progressive JPEG (stb)
                       PGM/PPM support (Ken Miller)
                       STBI_MALLOC,STBI_REALLOC,STBI_FREE
                       GIF bugfix -- seemingly never worked
                       STBI_NO_*, STBI_ONLY_*
    1.48  (2014-12-14) fix incorrectly-named assert()
    1.47  (2014-12-14) 1/2/4-bit PNG support, both direct and paletted (Omar Cornut & stb)
                       optimize PNG (ryg)
                       fix bug in interlaced PNG with user-specified channel count (stb)
    1.46  (2014-08-26)
            fix broken tRNS chunk (colorkey-style transparency) in non-paletted PNG
    1.45  (2014-08-16)
            fix MSVC-ARM internal compiler error by wrapping malloc
    1.44  (2014-08-07)
            various warning fixes from Ronny Chevalier
    1.43  (2014-07-15)
            fix MSVC-only compiler problem in code changed in 1.42
    1.42  (2014-07-09)
            don't define _CRT_SECURE_NO_WARNINGS (affects user code)
            fixes to stbi__cleanup_jpeg path
            added STBI_ASSERT to avoid requiring assert.h
    1.41  (2014-06-25)
            fix search&replace from 1.36 that messed up comments/error messages
    1.40  (2014-06-22)
            fix gcc struct-initialization warning
    1.39  (2014-06-15)
            fix to TGA optimization when req_comp != number of components in TGA;
            fix to GIF loading because BMP wasn't rewinding (whoops, no GIFs in my test suite)
            add support for BMP version 5 (more ignored fields)
    1.38  (2014-06-06)
            suppress MSVC warnings on integer casts truncating values
            fix accidental rename of 'skip' field of I/O
    1.37  (2014-06-04)
            remove duplicate typedef
    1.36  (2014-06-03)
            convert to header file single-file library
            if de-iphone isn't set, load iphone images color-swapped instead of returning NULL
    1.35  (2014-05-27)
            various warnings
            fix broken STBI_SIMD path
            fix bug where stbi_load_from_file no longer left file pointer in correct place
            fix broken non-easy path for 32-bit BMP (possibly never used)
            TGA optimization by Arseny Kapoulkine
    1.34  (unknown)
            use STBI_NOTUSED in stbi__resample_row_generic(), fix one more leak in tga failure case
    1.33  (2011-07-14)
            make stbi_is_hdr work in STBI_NO_HDR (as specified), minor compiler-friendly improvements
    1.32  (2011-07-13)
            support for "info" function for all supported filetypes (SpartanJ)
    1.31  (2011-06-20)
            a few more leak fixes, bug in PNG handling (SpartanJ)
    1.30  (2011-06-11)
            added ability to load files via callbacks to accomidate custom input streams (Ben Wenger)
            removed deprecated format-specific test/load functions
            removed support for installable file formats (stbi_loader) -- would have been broken for IO callbacks anyway
            error cases in bmp and tga give messages and don't leak (Raymond Barbiero, grisha)
            fix inefficiency in decoding 32-bit BMP (David Woo)
    1.29  (2010-08-16)
            various warning fixes from Aurelien Pocheville
    1.28  (2010-08-01)
            fix bug in GIF palette transparency (SpartanJ)
    1.27  (2010-08-01)
            cast-to-stbi_uc to fix warnings
    1.26  (2010-07-24)
            fix bug in file buffering for PNG reported by SpartanJ
    1.25  (2010-07-17)
            refix trans_data warning (Won Chun)
    1.24  (2010-07-12)
            perf improvements reading from files on platforms with lock-heavy fgetc()
            minor perf improvements for jpeg
            deprecated type-specific functions so we'll get feedback if they're needed
            attempt to fix trans_data warning (Won Chun)
    1.23    fixed bug in iPhone support
    1.22  (2010-07-10)
            removed image *writing* support
            stbi_info support from Jetro Lauha
            GIF support from Jean-Marc Lienher
            iPhone PNG-extensions from James Brown
            warning-fixes from Nicolas Schulz and Janez Zemva (i.stbi__err. Janez (U+017D)emva)
    1.21    fix use of 'stbi_uc' in header (reported by jon blow)
    1.20    added support for Softimage PIC, by Tom Seddon
    1.19    bug in interlaced PNG corruption check (found by ryg)
    1.18  (2008-08-02)
            fix a threading bug (local mutable static)
    1.17    support interlaced PNG
    1.16    major bugfix - stbi__convert_format converted one too many pixels
    1.15    initialize some fields for thread safety
    1.14    fix threadsafe conversion bug
            header-file-only version (#define STBI_HEADER_FILE_ONLY before including)
    1.13    threadsafe
    1.12    const qualifiers in the API
    1.11    Support installable IDCT, colorspace conversion routines
    1.10    Fixes for 64-bit (don't use "unsigned long")
            optimized upsampling by Fabian "ryg" Giesen
    1.09    Fix format-conversion for PSD code (bad global variables!)
    1.08    Thatcher Ulrich's PSD code integrated by Nicolas Schulz
    1.07    attempt to fix C++ warning/errors again
    1.06    attempt to fix C++ warning/errors again
    1.05    fix TGA loading to return correct *comp and use good luminance calc
    1.04    default float alpha is 1, not 255; use 'void *' for stbi_image_free
    1.03    bugfixes to STBI_NO_STDIO, STBI_NO_HDR
    1.02    support for (subset of) HDR files, float interface for preferred access to them
    1.01    fix bug: possible bug in handling right-side up bmps... not sure
            fix bug: the stbi__bmp_load() and stbi__tga_load() functions didn't work at all
    1.00    interface to zlib that skips zlib header
    0.99    correct handling of alpha in palette
    0.98    TGA loader by lonesock; dynamically add loaders (untested)
    0.97    jpeg errors on too large a file; also catch another malloc failure
    0.96    fix detection of invalid v value - particleman@mollyrocket forum
    0.95    during header scan, seek to markers in case of padding
    0.94    STBI_NO_STDIO to disable stdio usage; rename all #defines the same
    0.93    handle jpegtran output; verbose errors
    0.92    read 4,8,16,24,32-bit BMP files of several formats
    0.91    output 24-bit Windows 3.0 BMP files
    0.90    fix a few more warnings; bump version number to approach 1.0
    0.61    bugfixes due to Marc LeBlanc, Christopher Lloyd
    0.60    fix compiling as c++
    0.59    fix warnings: merge Dave Moore's -Wall fixes
    0.58    fix bug: zlib uncompressed mode len/nlen was wrong endian
    0.57    fix bug: jpg last huffman symbol before marker was >9 bits but less than 16 available
    0.56    fix bug: zlib uncompressed mode len vs. nlen
    0.55    fix bug: restart_interval not initialized to 0
    0.54    allow NULL for 'int *comp'
    0.53    fix bug in png 3->4; speedup png decoding
    0.52    png handles req_comp=3,4 directly; minor cleanup; jpeg comments
    0.51    obey req_comp requests, 1-component jpegs return as 1-component,
            on 'test' only check type, not whether we support this variant
    0.50  (2006-11-19)
            first released version
