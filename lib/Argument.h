#pragma once

#include <iostream>
#include <vector>

#include "Converter.h"
#include "Types.h"

namespace ArgumentParser {

enum class ArgumentParsingStatus {
    kNoArgument,
    kInsufficientArgument,
    kSuccess
};

class BaseArgument {
public:
    bool IsPositional() const {
        return is_positional_;
    };
    bool IsOk() const {
        return (
            status_ == ArgumentParsingStatus::kSuccess ||
            has_default_value_
        );
    }
    virtual std::string PrintInformation() const = 0;
    virtual void AddValue(const std::string& raw_value) = 0;
    virtual void Clear() = 0;
    virtual std::string Type() const = 0;
    BaseArgument(char short_name, 
                 const std::string& long_name,
                 const std::string& description) 
        : short_name_(short_name)
        , has_short_name_(true)
        , long_name_(long_name)
        , description_(description)
        , status_(ArgumentParsingStatus::kNoArgument)
        , has_default_value_(false)
        , is_positional_(false)
        , is_multi_value_(false) {}
    BaseArgument(const std::string& long_name,
                 const std::string& description) 
        : has_short_name_(false)
        , long_name_(long_name)
        , description_(description)
        , status_(ArgumentParsingStatus::kNoArgument)
        , has_default_value_(false)
        , is_positional_(false)
        , is_multi_value_(false) {}
    
    virtual ~BaseArgument() = default;
protected:
    char short_name_;
    bool has_short_name_;
    std::string long_name_;
    std::string description_;
    bool is_multi_value_;
    size_t min_values_count_;
    bool has_default_value_;
    bool is_positional_;
    ArgumentParsingStatus status_;
};

template <typename T>
class Argument final : public BaseArgument {
public:
    Argument& MultiValue(int min_num = 0) {
        is_multi_value_ = true;
        min_values_count_ = min_num;
        return *this;
    }
    Argument& Default(const T& default_value) {
        has_default_value_ = true;
        defualt_value_ = default_value;
        return *this;
    }
    Argument& StoreValue(T& storage) {
        ptr_to_value_ = &storage;
        return *this;
    }
    Argument& StoreValues(std::vector<T>& storage) {
        ptr_to_multi_value_ = &storage;
        return *this;
    }
    Argument& Positional() {
        is_positional_ = true;
        return *this;
    }

    T GetValue() const {
        if (is_multi_value_) {
            throw std::invalid_argument("Argument is multi-value");
        }
        if (status_ != ArgumentParsingStatus::kSuccess) {
            if (has_default_value_) {
                return defualt_value_;
            }
            throw std::runtime_error("Couldn't parse argument");
        }
        return *ptr_to_value_;
    }

    T GetValue(int index) const {
        if (!is_multi_value_) {
            if (has_default_value_) {
                return defualt_value_;
            }
            throw std::invalid_argument("Argument is not multi-value");
        }
        if (index < 0 || index >= ptr_to_multi_value_->size()) {
            if (has_default_value_) {
                return defualt_value_;
            }
            throw std::invalid_argument("Index out of range");
        }
        if (status_ != ArgumentParsingStatus::kSuccess) {
            if (has_default_value_) {
                return defualt_value_;
            }
            throw std::runtime_error("Couldn't parse argument" + long_name_);
        }
        return (*ptr_to_multi_value_)[index];
    }

    void AddValue(const std::string& raw_value) override {
        T value;
        bool result = Converter::Convert(raw_value, value);
        if (!result) {
            return;
        }
        if (is_multi_value_) {
            ptr_to_multi_value_->push_back(value);
            if (ptr_to_multi_value_->size() >= min_values_count_) {
                status_ = ArgumentParsingStatus::kSuccess;
            } else {
                status_ = ArgumentParsingStatus::kInsufficientArgument;
            }
            return;
        }
        *ptr_to_value_ = value;
        status_ = ArgumentParsingStatus::kSuccess;
    }

    void Clear() override {
        ptr_to_multi_value_->clear();
        status_ = ArgumentParsingStatus::kNoArgument;
    }
    std::string Type() const override {
        return Types::GetTypename<T>();
    }

    std::string PrintInformation() const override {
        std::string result = "";
        if (has_short_name_) {
            result += "-" + std::string(1, short_name_) + " ";
        } else {
            result += "   ";
        }
        result += "--" + long_name_;
        if (Type() != "bool") {
            result += "=<" + Type() + ">";
        }
        result += "  ";
        result += description_;
        result += "  ";
        if (has_default_value_) {
            result += "[default = " + Converter::ToString(defualt_value_) + "]  "; 
        }
        if (is_multi_value_) {
            result += "[repeated, min args = " + Converter::ToString(min_values_count_) + "]  ";
        }
        if (is_positional_) {
            result += "[positional]  ";
        }
        result += "\n";
        return result;
    }
    
    Argument(char short_name, 
             const std::string& long_name,
             const std::string& description)
        : BaseArgument(short_name, long_name, description)
        , ptr_to_value_(&value_)
        , ptr_to_multi_value_(&values_) {}
    Argument(const std::string& long_name,
             const std::string& description)
        : BaseArgument(long_name, description)
        , ptr_to_value_(&value_)
        , ptr_to_multi_value_(&values_) {}
    ~Argument() = default;
private:
    T defualt_value_;
    T value_;
    std::vector<T> values_;
    T* ptr_to_value_;
    std::vector<T>* ptr_to_multi_value_;
};

template<>
Argument<bool>& Argument<bool>::MultiValue(int min_num) = delete;

} // namespace ArgumentParser