#include "Cic.hpp"
#include "iostream"
#include "vector"
#include "fstream"
#include "tuple"
#include "Core.hpp"
ProgramOptions ParseArgs(std::vector<std::string> args) {
    ProgramOptions options;
    int i = 1;
    while ((i) < (args.size())) {
        if (args[i] == "-o") {
            i++;
            options.output = args[i];
        } else if (args[i] == "-h" || args[i] == "--help") {
            options.help = true;
        } else {
            options.input = args[i];
        }
        i++;
    }
    return options;
}
auto ShowHelp() {
    std::cout << "Usage: cic [options] [input file]\n";
    std::cout << "Options:\n";
    std::cout << "  -o <dir>    Output directory\n";
    std::cout << "  -h --help  Show this help\n";
}
std::vector<std::string> ArgsToVector(int argc, char** argv) {
    std::vector<std::string> args;
    int i = 0;
    while ((i) < (argc)) {
        args.push_back(argv[i]);
        i++;
    }
    return args;
}
int main(int argc, char* argv[]) {
    std::vector<std::string> const args = ArgsToVector(argc, argv);
    ProgramOptions const options = ParseArgs(args);
    if (options.help) {
        ShowHelp();
        return 0;
    }
    if (options.input == "") {
        std::cout << "No input file specified" << std::endl;
        return 1;
    }
    IRNode* const ir = IRFromFile(options.input);
    SaveIRToFile(ir, options.output, options.input);
    delete ir;
    return 0;
}
