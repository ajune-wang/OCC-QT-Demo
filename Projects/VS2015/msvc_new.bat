@echo off

Setlocal EnableDelayedExpansion

rem Setup environment
SET PATH=%PATH%;../../OCC-Lib/MSVC_win64_vc14/bin;
SET "CSF_OCCTIncludePath=../../OCC-Lib/inc"
SET "CSF_OCCTLibPath=../../OCC-Lib/MSVC_win64_vc14/lib"

rem Define path to project file
start "" "OCC.sln"
