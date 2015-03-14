The very first version of build instructions.

# 1. Download latest Glide64 sources #

From SVN:
svn checkout http://glidehqplusglitch64.googlecode.com/svn/trunk/

# 2. Build Glitch64 #

Glitch64 requires development version of SDL to be installed (http://www.libsdl.org)
You may type sdl-config in a console to be sure that SDL is installed.

Enter Glitch64 folder and type
make -f Makefile.gcc

Then copy glide3x.dylib to Glide64/lib:
cp glide3x.dylib ../Glide64/lib

# 3. Build Glide64 #

This is trickier.

- Glide64 uses wxWidgets for all platform-dependent tasks. You may install pre-build development version of it, but I recommend you to build wxWidgets from sources: http://www.wxwidgets.org. wxWidgets can be build in many configurations: shared or static, ANSI or Unicode etc. Pre-build versions are usually 'Shared, Unicode'. Glide64 uses wxConfig class to work with ini file. For unknown for me reason Unicode version can't load ini file if it has section names for Japanese games. ANSI build does not have this issue. Thus, either build wxWidgets yourself or remove all Japanese games related sections from the ini. wxWidgets default configuration when build from sources is 'ANSI, Shared', which should be ok for most of people.

- Glide64 uses NASM to compile assembler code. Be sure that NASM is installed on your system.

- Glide64 is not compatible with GCC v4.3, at least for Linux. The Mac machine I used has GCC v4.1, so I had no problems there, but be warned.

When all prerequisites are installed, enter Glide64 folder and type
make -f Makefile.gcc

# 4. Build GlideHQ #

To do!

5. Copy the result to MupenPlus

Copy Glide64.so and GlideHQ.so to Mupen64Plus.app/Contents/Resources/plugins
Copy glide3x.dylib to Mupen64Plus.app/Contents/MacOS
Copy Glide64.ini to Mupen64Plus.app/Contents/Resources/config

# 6. Setup #

Read Glide64 help file to learn how to configure it.

# 7. Known issues. #

This is simple - the plugin just does not work. If you did everything right, you may select "Glide64 Napalm" as graphics plugin, but any attempt to configure it causes "Error, Failed to read config options". wxWidgets throws this message when it can't load the config file. I stuck there. Debugging tools are necessary to move further. I don't have time to learn development for Mac. Thus, a help from experienced Mac developer is needed. I hope that the problem is simple.


Comments and corrections are welcome.