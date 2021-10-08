@echo off

Setlocal EnableDelayedExpansion

rem Setup environment
SET PATH=%PATH%;../../OCC-Lib/MinGW_win64_gcc/bin;

rem Define path to project file
start "" "OCCT-QT-Demo_MinGW.pro"