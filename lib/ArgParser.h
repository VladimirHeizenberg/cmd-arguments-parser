#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>

#include "Argument.h"

namespace ArgumentParser {

class ArgParser final {
public:
    bool Parse(const std::vector<std::string>& args);
    bool Parse(int argc, char** argv);


    template<typename T>
    Argument<T>& AddArgument(char short_name,
                            const std::string& long_name,
                            const std::string& description = "") {
        if (!short_to_long_name_.contains(short_name)) {
            short_to_long_name_[short_name] = long_name;
        }
        arguments[long_name] = std::make_unique<Argument<T>>(short_name, long_name, description);
        return (
            *(dynamic_cast<Argument<T>*>(arguments[long_name].get()))
        );
    }

    template<typename T>
    Argument<T>& AddArgument(const std::string& long_name,
                             const std::string& description = "") {
        arguments[long_name] = std::make_unique<Argument<T>>(long_name, description);
        return (
            *(dynamic_cast<Argument<T>*>(arguments[long_name].get()))
        );
    }

    template<typename T>
    T GetValue(const std::string& long_name, int index) {
        if (!arguments.contains(long_name)) {
            throw std::invalid_argument("No such argument");
        }
        return dynamic_cast<Argument<T>*>(arguments[long_name].get())->GetValue(index);
    }

    template<typename T>
    T GetValue(const std::string& long_name) {
        if (!arguments.contains(long_name)) {
            throw std::invalid_argument("No such argument");
        }
        return dynamic_cast<Argument<T>*>(arguments[long_name].get())->GetValue();
    }
    // IntArgument
    Argument<int>& AddIntArgument(char short_name,
                                  const std::string& long_name,
                                  const std::string& description = "") {
        return AddArgument<int>(short_name, long_name, description);
    }
    Argument<int>& AddIntArgument(const std::string& long_name,
                                  const std::string& description = "") {
        return AddArgument<int>(long_name, description);
    }

    int GetIntValue(const std::string& long_name, int index) {
        return GetValue<int>(long_name, index);
    }
    int GetIntValue(const std::string& long_name) {
        return GetValue<int>(long_name);
    }

    // Flags
    Argument<bool>& AddFlag(char short_name,
                            const std::string& long_name,
                            const std::string& description = "") {
        return AddArgument<bool>(short_name, long_name, description);
    }
    Argument<bool>& AddFlag(const std::string& long_name,
                            const std::string& description = "") {
        return AddArgument<bool>(long_name, description);
    }

    bool GetFlag(const std::string& long_name, int index) = delete;
    bool GetFlag(const std::string& long_name) {
        return GetValue<bool>(long_name);
    }
    
    // StringArgument
    Argument<std::string>& AddStringArgument(char short_name, 
                                             const std::string& long_name,
                                             const std::string& description = "") {
        return AddArgument<std::string>(short_name, long_name, description);
    }
    Argument<std::string>& AddStringArgument(const std::string& long_name,
                                             const std::string& description = "") {
        return AddArgument<std::string>(long_name, description);
    }
    std::string GetStringValue(const std::string& long_name, int index) {
        return GetValue<std::string>(long_name, index);
    }
    std::string GetStringValue(const std::string& long_name) {
        return GetValue<std::string>(long_name);
    }

    // Help
    void AddHelp(char short_name,
                 const std::string& long_name,
                 const std::string& description = "");
    bool Help();
    std::string HelpDescription();

    ArgParser(const std::string& name);
    ArgParser(ArgParser& other) = delete;
    ArgParser& operator=(ArgParser& other) = delete;
    ~ArgParser();
private:
    std::string name_;
    std::map<std::string, std::unique_ptr<BaseArgument>> arguments;
    std::map<char, std::string> short_to_long_name_;
    bool has_help_command_;
    std::string short_help_command_;
    std::string long_help_command_;
    std::string description_;
    bool is_ok = true;

    void Clear();
    void ReadArgumentAndValue(const std::string& arg, const std::string& value);
    void ReadArgumentWithEqualSymbol(const std::string& argument, int index);
    void ReadALotOfFlags(std::string_view argument);
    void ReadPositionalArgument(const std::string& arg);
    int FindEqualSymbolInString(std::string_view argument) const;
    void Validate();
};

template<>
bool ArgParser::GetValue(const std::string& long_name, int index) = delete;

} // namespace ArgumentParser