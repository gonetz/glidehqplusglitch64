## 1. Mupen64 Plus ##

Linux port of Glide64 is not compatible with binary release of MupenPlus 1.5 because of a bug in it. Thus, you must build MupenPlus from sources, and fix the bug if necessary:
In main\_gtk.c find
void gui\_init(int**argc, char** argv)
and replace
/**Initialize multi-threading support.**/
g\_thread\_init(NULL);
by
/**Initialize multi-threading support.**/
if (!g\_thread\_get\_initialized())
g\_thread\_init(NULL);


## 2. Download latest Glide64 sources ##

From SVN:
svn checkout http://glidehqplusglitch64.googlecode.com/svn/trunk/

## 3. Build Glitch64 ##

Glitch64 requires development version of SDL to be installed (http://www.libsdl.org)
You may tyoe sdl-config in a console to be sure that SDL is installed.

Enter Glitch64 folder and type
make -f Makefile.gcc

Then copy glide3x.so to Glide64/lib:
cp glide3x.so ../Glide64/lib

## 4. Build Glide64 ##

This is trickier.

- Glide64 uses wxWidgets for all platform-dependent tasks. You may install pre-build development version of it for your version of Linux, but I recommend you to build wxWidgets from sources: http://www.wxwidgets.org. wxWidgets can be build in many configurations: shared or static, ANSI or Unicode etc. Pre-build versions are usually 'Shared, Unicode'. Glide64 uses wxConfig class to work with ini file. For unknown for me reason Unicode version can't load ini file if it has section names for Japanese games. ANSI build does not have this issue. Thus, either build wxWidgets yourself or remove all Japanese games related sections from the ini. wxWidgets default configuration when build from sources is 'ANSI, Shared', which should be ok for most of people.

- Glide64 uses NASM to compile assembler code. Be sure that NASM is installed on your system.

- Glide64 is not compatible with GCC v4.3 It can be compiled by this version of the compiler, but resulted library will not work. I did not find why it happens. Thus, you must install previous version of GCC. 4.2 will be fine. The makefile already has the option to use v4.2

When all prerequisites are installed, enter Glide64 folder and type
make -f Makefile.gcc

## 5. Build GlideHQ ##

This step is optional. If you don't need texture enhancement and high-resolution textures, skip to next step.

GlideHQ requires "boost" library (http://www.boost.org) library. Read carefully GlideHQ/Makefile.gcc for instructions, how to get and build boost. When boost build is finished, go to GlideHQ/tc-1.1+ and run
make -f Makefile.gcc
cp libdxtn.a ../lib

Also, manually copy libz.a and libpng.a to GlideHQ/lib folder. Usually you can find them in /usr/lib

From GlideHQ folder type
make -f Makefile.gcc

## 6. Copy the result to MupenPlus ##

Copy Glide64.so and GlideHQ.so to MupenPlus/plugins
Copy glide3x.so to MupenPlus/lib
Copy Glide64.ini to MupenPlus/config

## 7. Setup ##

Read Glide64 help file to learn how to configure it.

## 8. Known issues. ##

- Besides common emulation issues, listed in 'Known issues' document, each non-windows port has its own set of issues. While Linux port can do most of things available for Windows, not all Win API functions used by the plugin have analogs in wxWidgets. Thus, some plugin's functionality is for Windows only. You may check the sources for #ifdef WINDOWS.

- Glide64 uses several hotkeys (check corresponding section of the readme). Windows port uses GetAsyncKeyState function to check that the key was pressed. wxWidgets does not provide portable analog for that. Thus, for Linux we have to use SDL. Since I don't want the plugin code to be dependent from this library, we implemented it as Glide3x extension in the wrapper, because it uses SDL anyway. We use SDL\_GetKeyState / SDL\_GetMouseState functions. The problem is that these functions are not asynchronous, as GetAsyncKeyState. They can say is the key/button currently pressed or not, while GetAsyncKeyState can say was the key pressed since the last call to this function. Thus, it is possible that user presses the hot key but nothing happens. It will be great if someone can suggest better solution.

- Glide64 works unstable when built with compiler optimization options. I tried -O2/-O3. Will be great if someone will find safe set of optimization options.

- "About" dialog lacks bitmaps. It should show Glide64 logo and country flags for all team members, but shows nothing.

## 9. Binary release ##

Binary release of Glide64 is possible. wxWidgets can be build as statically linked library. I use it for windows build. After build I get large dll file, which can work without wxWidgets installed. However, when I tried to do it on Linux, Glide64 worked unstable. May be it is a problem with my Linux, may be I did something wrong. Will be nice if somebody will experiment with it. The goal is to make release available for anyone.

Comments and corrections are welcome.