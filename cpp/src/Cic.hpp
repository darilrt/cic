#pragma once
#include "iostream"
#include "vector"
#include "fstream"
#include "tuple"
#include "Core.hpp"
class ProgramOptions {
public: std::string input = "";
public: std::string output = "";
public: bool help = false;
};
[[nodiscard]] ProgramOptions ParseArgs(std::vector<std::string> args);
auto ShowHelp();
[[nodiscard]] std::vector<std::string> ArgsToVector(int argc, char** argv);

