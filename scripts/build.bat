@echo off
 
:: Create build dir
set buildDir=%~dp0..\build
if not exist %buildDir% mkdir %buildDir%
pushd %buildDir%
 
:: Create obj dir
set objDir=.\obj
if not exist %objDir% (
    mkdir %objDir%
    mkdir %objDir%\viewport
    mkdir %objDir%\utils
    mkdir %objDir%\raytracer
    mkdir %objDir%\raytracer\geo
    mkdir %objDir%\raytracer\material
    mkdir %objDir%\raytracer\utils
    mkdir %objDir%\math
)
 
:: Needed folders
set extDir=%~dp0..\external
set scriptDir=%~dp0..\scripts

set rendersDir=%~dp0..\renders
if not exist %rendersDir% (mkdir  %rendersDir%)
 
:: Use make to build default target
cd %scriptDir%\
mingw32-make
 
popd