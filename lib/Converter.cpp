#include "Converter.h"

bool Converter::Convert(const std::string& string, int& result) {
    try {
        result = std::stoi(string);
    } catch (std::exception& e) {
        result = 0;
        return false;
    }

    return true;
}

bool Converter::Convert(const std::string& string, long& result) {
    try {
        result = std::stol(string);
    } catch (std::exception& e) {
        result = 0;
        return false;
    }

    return true;
}

bool Converter::Convert(const std::string& string, long long& result) {
    try {
        result = std::stoll(string);
    } catch (std::exception& e) {
        result = 0;
        return false;
    }

    return true;
}

bool Converter::Convert(const std::string& string, float& result) {
    try {
        result = std::stof(string);
    } catch (std::exception& e) {
        result = 0.0;
        return false;
    }

    return true;
}

bool Converter::Convert(const std::string& string, double& result) {
    try {
        result = std::stod(string);
    } catch (std::exception& e) {
        result = 0.0;
        return false;
    }

    return true;
}

bool Converter::Convert(const std::string& string, std::string& result) {
    result = string;
    return true;
}

bool Converter::Convert(const std::string& string, bool& result) {
    if (string == "true") {
        result = true;
    } else {
        result = false;
    }

    return true;
}

std::string Converter::ToString(const std::string& value) {
    return value;
}