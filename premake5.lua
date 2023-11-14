-- premake5.lua
workspace "Life"
    configurations { "Debug", "Release" }
    startproject "LifeCmd"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"
    location "generated"
    include "yaml-cpp"
    targetdir "build/bin/%{prj.name}/%{cfg.longname}"
    objdir "build/obj/%{prj.name}/%{cfg.longname}"
    
   filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
   
   
project "LifeCore"
    kind "StaticLib"
    includedirs { "projects/LifeCore/include"}
    files { "projects/LifeCore/include/*.h", "projects/LifeCore/src/*.cpp" }
    links "yaml-cpp"
   
project "LifeCmd"
    kind "ConsoleApp"
    files { "projects/LifeCmd/include/*.h", "projects/LifeCmd/src/*.cpp" }
    
    includedirs "projects/LifeCore/include"
    links "LifeCore"
    
    
