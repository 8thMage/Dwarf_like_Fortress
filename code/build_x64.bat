@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

del "..\build\*.dll"
del "..\build\*.pdb"
del "..\build\*.ilk"
del "..\build\*.exp"
del "..\build\*.lib"
del "..\build\*.map"
del "..\build\*.psess"
rem -Dwrite_to_file
set prod_error_code=/wd4302 /wd4189 /wd4456 /wd4061 /wd4996
set error_code= /wd4820 /wd4091 /wd4668 /wd4201 /wd4100 /wd4505 /wd4311 /wd4311 /wd4711 /wd4710 /wd4324 /wd4350 /wd4514 /wd4310 %prod_error_code%
set common_compiler_flags=/Z7 /Od -Wall %error_code% /fp:fast -Ddebug -Ddebug_breakpoint=0 -Dconstant_memory -Dx64  -I ../misc/iaca /arch:AVX2
set common_linker_flags=/profile /INCREMENTAL:NO
cl game.cpp %common_compiler_flags% %no_tests% -I../code/include /openmp /link %common_linker_flags% /dll /out:../build/render3d.dll -PDB:..\build\render3d%random%.pdb /MAP:..\build\render3d%random%.map  -EXPORT:go_game  User32.lib Kernel32.lib 

echo "temp">>../build/temp.tmp
cl Winmain.cpp %common_compiler_flags% %no_tests% /wd4365 /wd4189 /wd4996 /wd4191  /link %common_linker_flags% /out:../build/main.exe User32.lib gdi32.lib Opengl32.lib Comdlg32.lib Shcore.lib
del "..\code\*.obj"
REM del "w:\code\*.pdb"
REM >>build_res.txt
@echo on 
