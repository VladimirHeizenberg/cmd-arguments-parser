#pragma once

#include <string>
#include <typeindex>
#include <map>

namespace Types {
    template<typename T>
    std::string GetTypename() {
        return std::string(typeid(T).name());
    }
    template<>
    inline std::string GetTypename<int>() {
        return "int";
    }
    template<>
    inline std::string GetTypename<bool>() {
        return "bool";
    }
    template<>
    inline std::string GetTypename<long>() {
        return "long";
    }
    template<>
    inline std::string GetTypename<long long>() {
        return "long long";
    }
    template<>
    inline std::string GetTypename<double>() {
        return "double";
    }
    template<>
    inline std::string GetTypename<std::string>() {
        return "string";
    }
}