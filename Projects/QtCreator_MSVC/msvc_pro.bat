@echo off

Setlocal EnableDelayedExpansion

rem Setup environment
SET PATH=%PATH%;../../OCC-Lib/MSVC_win64_vc14/bin;

rem Define path to project file
start "" "OCCT-QT-Demo_MSVC.pro"