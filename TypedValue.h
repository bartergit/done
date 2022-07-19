#pragma once

#include <iostream>
#include <variant>
#include <cstring>
#include <unordered_map>
#include <iomanip>

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum TYPE {
    NUMBER,
    BOOLEAN,
    STRING,
    OBJECT,
    NaN,
    NULLJS,     // NULL is reserved word
    UNDEFINED
};

std::string type_to_str(TYPE type);


class TypedValue {
public:
    TYPE type;
    std::variant<bool, float, const char *, int> data;  // int for types UNDEFINED, NULLJS, NaN
    std::unordered_map<const char *, TypedValue> attributes{};

    explicit TypedValue(TYPE type) : type(type) {
        data = 0;
    }

    explicit TypedValue(std::string str) {
        auto * s = new std::string; // wtf is that
        *s = str;
        data = s->c_str();
        type = STRING;
    }

    explicit TypedValue(bool data) : data(data) {
        type = BOOLEAN;
    }

    explicit TypedValue(float data) : data(data) {
        type = NUMBER;
    }

    template<class T>
    T unwrap() {
        return std::get<T>(data);
    }

    TypedValue operator+(TypedValue other);

    TypedValue operator-(TypedValue other);

    TypedValue operator*(TypedValue other);

    TypedValue operator/(TypedValue other);

    TypedValue operator%(TypedValue other);

    TypedValue operator&&(TypedValue other);

    TypedValue operator||(TypedValue other);

    TypedValue operator==(TypedValue other);

    TypedValue get_attribute(const char *attribute_name);

    template<TYPE to>
    TypedValue cast();

    std::string str();
};