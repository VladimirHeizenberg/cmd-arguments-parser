#pragma once

#include <string>

class Converter {
public:
    static bool Convert(const std::string& string, int& result);
    static bool Convert(const std::string& string, long& result);
    static bool Convert(const std::string& string, long long& result);
    static bool Convert(const std::string& string, float& result);
    static bool Convert(const std::string& string, double& result);
    static bool Convert(const std::string& string, std::string& result);
    static bool Convert(const std::string& string, bool& result);

    static std::string ToString(const std::string& value);

    template<typename T>
    static std::string ToString(T value) {
        return std::to_string(value);
    }
};
