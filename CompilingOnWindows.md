Note: you need at least Visual Studio 2005 to compile all the libraries.

# 1. Download latest Glide64 sources #

From SVN:
svn checkout http://glidehqplusglitch64.googlecode.com/svn/trunk/

# 2. Build Glitch64 #

Simply use the Visual Studio solution files to compile.

# 3. Build Glide64 #

This is trickier.

Glide64 uses wxWidgets library. It is large and you must install it manually. Other libs necessary to build the project are included.

1. Download wxWidgets from http://www.wxwidgets.org You may find compiled builds, or build them from sources. Building from sources is quite simple:
Unpack the sources somewhere, e.g. to C:\wxWidgets\
Make sure that path to VisualC bin folder is included to system environment path variable. Try to type in console nmake. If system can’t find it, locate the path to VC98\Bin\ folder and add it to path variable, e.g.:
set path=%path%;C:\MSVC\VC98\Bin\
Go to C:\wxWidgets\build\msw\ folder and type:
nmake.exe makefile.vc
it will build everything. Just wait until it finished.

2. Create new environment variable WXDIR (my computer/properties/advanced/environment variables/create), and set path to wxWidgets folder as its value, e.g. C:\wxWidgets

3. Now you may build the project. Open VisualStudio and load n64Glide.sln solution file. Select debug/release and build the projects you need.

# 4. Build GlideHQ #

This step is optional. If you don't need texture enhancement and high-resolution textures, thats all you have to do.

You will need GNU Make to use the makefile.

GlideHQ requires the "boost" library, as well as libpng and zlib. Read carefully GlideHQ/Makefile.vc8 for instructions, how to get and build boost, as well as the other dependancies.
From GlideHQ folder type make -f Makefile.vc8 once you copied the needed headers and libs.