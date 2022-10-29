Frankenstein is a game engine written in C++ to develop simple 2D games
This is just for fun. 
Don't take it too serious. 
It's a spinoff of an old college project of mine.

Setup:

VS2019 v142 MSBuild Tools

Currently Release x86 and x64 builds

Application:
Frankenstein\bin\x64\Release\Frankenstein.exe

Use the arrow keys to move the dwarf fighter towards the skeleton

OutDir:
../../bin/x86/$(Configuration)

AdditionalIncludeDirectories:
..\..\dependencies\FMOD\inc;..\..\dependencies\SDL2\include;..\..\dependencies\GLEW\include

AdditionalLibraryDirectories:
..\..\dependencies\FMOD\lib;..\..\dependencies\SDL2\lib\x86;..\..\dependencies\GLEW\lib\Release\Win32

AdditionalDependencies:
fmod_vc.lib;glew32.lib;glu32.lib;SDL2.lib;opengl32.lib;%(AdditionalDependencies)

_CRT_SECURE_NO_WARNINGS add to C++ preprocessor

or

OutDir:
../../bin/x64/$(Configuration)

AdditionalIncludeDirectories:
..\..\dependencies\FMOD\inc;..\..\dependencies\SDL2\include;..\..\dependencies\GLEW\include

AdditionalLibraryDirectories:
..\..\dependencies\FMOD\lib;..\..\dependencies\SDL2\lib\x64;..\..\dependencies\GLEW\lib\Release\x64

AdditionalDependencies:
fmod64_vc.lib;glew32.lib;glu32.lib;SDL2.lib;SDL2main.lib;opengl32.lib;%(AdditionalDependencies)

_CRT_SECURE_NO_WARNINGS add to C++ preprocessor