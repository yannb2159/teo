#include "Interpreter.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "[ERROR] No program provided !\n";
        std::cerr << "[ERROR] Usage : ./teo <my_program.bf>\n";
        return EXIT_FAILURE;
    }

    std::string programPath = std::string(argv[1]);

    Interpreter myInterpreter;

    try {
        myInterpreter.LoadProgramFromFile(programPath); 
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
        return EXIT_FAILURE;
    }

    myInterpreter.Interpret();

    return EXIT_SUCCESS;
}
