@echo off
set cb=python "../bic-lang/main.py"
set cc=gcc

%cb% src/Bic.bic -o ./cpp/
%cb% src/Token.bic -o ./cpp/
%cb% src/Lexer.bic -o ./cpp/
%cb% src/Parser.bic -o ./cpp/
%cb% src/AST.bic -o ./cpp/

rem create cpp folder

if not exist bin mkdir bin
if not exist cpp mkdir cpp
if not exist cpp\src mkdir cpp\src
if not exist cpp\obj mkdir cpp\obj
if not exist cpp\obj\src mkdir cpp\obj\src

rem compile cpp files

cc -c ./cpp/src/Bic.cpp -o ./cpp/obj/src/Bic.o
cc -c ./cpp/src/Token.cpp -o ./cpp/obj/src/Token.o
cc -c ./cpp/src/Lexer.cpp -o ./cpp/obj/src/Lexer.o
cc -c ./cpp/src/Parser.cpp -o ./cpp/obj/src/Parser.o
cc -c ./cpp/src/AST.cpp -o ./cpp/obj/src/AST.o

rem link

cc  ./cpp/obj/src/Bic.o ^
    ./cpp/obj/src/Token.o ^
    ./cpp/obj/src/Lexer.o ^
    ./cpp/obj/src/Parser.o ^
    ./cpp/obj/src/AST.o ^
    -o ./bin/Bic.exe ^
    -lstdc++