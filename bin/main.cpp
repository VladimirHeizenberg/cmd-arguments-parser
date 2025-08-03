#include <iostream>

#include <lib/ArgParser.h>


int main(int argc, char** argv) {
    ArgumentParser::ArgParser p("My Parser");
    p.AddArgument<std::string>("from");
    p.AddArgument<std::string>("to");
    std::cout << p.Parse(argc, argv) << "";
}