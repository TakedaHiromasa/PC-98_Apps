@echo off
:foo
cls
Echo Start
MM128.EXE 64
CL.EXE
GOTO foo
