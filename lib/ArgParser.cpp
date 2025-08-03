#include "ArgParser.h"

#include <stdexcept>
#include <iostream>

ArgumentParser::ArgParser::ArgParser(const std::string& name)
    : name_(name) {}

ArgumentParser::ArgParser::~ArgParser() {
}

void ArgumentParser::ArgParser::AddHelp(char short_name, 
                                        const std::string& long_name, 
                                        const std::string& description) {
    short_help_command_ = std::string() + '-' + short_name;
    long_help_command_ = "--" + long_name;
    description_ = description;
}

bool ArgumentParser::ArgParser::Help() {
    return has_help_command_;
}

std::string ArgumentParser::ArgParser::HelpDescription() {
    std::string ans = name_ + "\n" +
                      description_ + "\n\n";
    for (auto& c : arguments) {
        ans += c.second->PrintInformation();
    }
    return ans;
}

int ArgumentParser::ArgParser::FindEqualSymbolInString(std::string_view arg) const {
    int i = 0;
    while (i < arg.size()) {
        if (arg[i] == '=') {
            return i;
        }
        i++;
    }
    return -1;
}

void ArgumentParser::ArgParser::ReadALotOfFlags(std::string_view arg) {
    for (char c : arg) {
        if (!short_to_long_name_.contains(c)) {
            is_ok = false;
        } else {
            arguments[short_to_long_name_[c]]->AddValue("true");
        }
    }
}

void ArgumentParser::ArgParser::ReadArgumentAndValue(const std::string& arg, 
                                                     const std::string& value) {
    if (arguments.contains(arg)) {
        arguments[arg]->AddValue(value);
    } else if (arg.size() == 1 && short_to_long_name_.contains(arg[0])) {
        ReadArgumentAndValue(short_to_long_name_[arg[0]], value);
    } else {
        is_ok = false;
    }
}

void ArgumentParser::ArgParser::ReadArgumentWithEqualSymbol(const std::string& argument, 
                                                            int index) {
    std::string arg = argument.substr(0, index);
    std::string value = argument.substr(index + 1, argument.size() - (index));
    ReadArgumentAndValue(arg, value);
}

void ArgumentParser::ArgParser::ReadPositionalArgument(const std::string& arg) {
    for (auto& c : arguments) {
        if (c.second->IsPositional()) {
            c.second->AddValue(arg);
            break;
        }
    }
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& args) {
    Clear();
    for (int i = 1; i < args.size(); i++) {
        auto arg = args[i];
        if (arg == short_help_command_ || arg == long_help_command_) {
            HelpDescription();
            has_help_command_ = true;
            return true;
        }
        
        int n = arg.size();
        if (arg[0] == '-' && arg[1] == '-') {
            // long argument
            std::string tmp = arg.substr(2, n - 2);
            int index = FindEqualSymbolInString(tmp);
            if (index == -1) {
                if (arguments.contains(tmp) && arguments[tmp]->Type() == "bool") {
                    arguments[tmp]->AddValue("true");
                } else {
                    ReadArgumentAndValue(tmp, args[++i]);
                }
            } else {
                ReadArgumentWithEqualSymbol(tmp, index);
            }
        } else if (arg[0] == '-') {
            // short argument
            std::string tmp = arg.substr(1, n - 1);
            int index = FindEqualSymbolInString(tmp);
            if (index == -1 && tmp.size() > 1) {
                ReadALotOfFlags(tmp);
            } else if (index == -1) {
                if (arguments.contains(short_to_long_name_[tmp[0]]) && 
                    arguments[short_to_long_name_[tmp[0]]]->Type() == "bool") {
                    arguments[short_to_long_name_[tmp[0]]]->AddValue("true");
                } else {
                    ReadArgumentAndValue(tmp, args[++i]);
                }
            } else {
                ReadArgumentWithEqualSymbol(tmp, index);
            }
        } else {
            ReadPositionalArgument(arg);
        }
    }
    Validate();
    return is_ok;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    return Parse(args);
}

void ArgumentParser::ArgParser::Validate() {
    for (auto& c : arguments) {
        if (c.second->Type() != "bool") {
            if (!c.second->IsOk()) {
                is_ok = false;
            }
        }
    }
}

void ArgumentParser::ArgParser::Clear() {
    for (auto& c : arguments) {
        c.second->Clear();
    }
}