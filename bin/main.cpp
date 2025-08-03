#include <functional>
#include <lib/ArgParser.h>

#include <sstream>
#include <iostream>
#include <numeric>
#include <iterator>

int main(int argc, char** argv) {
    ArgumentParser::ArgParser p("My Parser");
    p.AddArgument<std::string>("from");
    p.AddArgument<std::string>("to");
    std::cout << p.Parse(argc, argv) << "";
}