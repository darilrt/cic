@echo off
set cb="./bin/cic"
set cc=gcc

rem create cpp folder

if not exist bin mkdir bin
if not exist cpp mkdir cpp
if not exist cpp\src mkdir cpp\src
if not exist cpp\obj mkdir cpp\obj
if not exist cpp\obj\src mkdir cpp\obj\src

rem compile cic

%cb% src/Cic.cic -o ./cpp/src
%cb% src/Token.cic -o ./cpp/src
%cb% src/Lexer.cic -o ./cpp/src
%cb% src/Parser.cic -o ./cpp/src
%cb% src/AST.cic -o ./cpp/src
%cb% src/IR.cic -o ./cpp/src
%cb% src/Common.cic -o ./cpp/src
%cb% src/CodeGenerator.cic -o ./cpp/src
%cb% src/IRGenerator.cic -o ./cpp/src
%cb% src/Core.cic -o ./cpp/src

rem compile cpp files

cc -c ./cpp/src/Cic.cpp -o ./cpp/obj/src/Cic.o
cc -c ./cpp/src/Token.cpp -o ./cpp/obj/src/Token.o
cc -c ./cpp/src/Lexer.cpp -o ./cpp/obj/src/Lexer.o
cc -c ./cpp/src/Parser.cpp -o ./cpp/obj/src/Parser.o
cc -c ./cpp/src/AST.cpp -o ./cpp/obj/src/AST.o
cc -c ./cpp/src/IR.cpp -o ./cpp/obj/src/IR.o
cc -c ./cpp/src/Common.cpp -o ./cpp/obj/src/Common.o
cc -c ./cpp/src/CodeGenerator.cpp -o ./cpp/obj/src/CodeGenerator.o
cc -c ./cpp/src/IRGenerator.cpp -o ./cpp/obj/src/IRGenerator.o
cc -c ./cpp/src/Core.cpp -o ./cpp/obj/src/Core.o

rem link

cc  ./cpp/obj/src/Cic.o ^
    ./cpp/obj/src/Token.o ^
    ./cpp/obj/src/Lexer.o ^
    ./cpp/obj/src/Parser.o ^
    ./cpp/obj/src/AST.o ^
    ./cpp/obj/src/IR.o ^
    ./cpp/obj/src/Common.o ^
    ./cpp/obj/src/CodeGenerator.o ^
    ./cpp/obj/src/IRGenerator.o ^
    ./cpp/obj/src/Core.o ^
    -o ./bin/cic.exe ^
    -lstdc++