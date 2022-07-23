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
    typedef std::unordered_map<std::string, TypedValue> Attributes;
    TYPE type;
    std::variant<bool, float, std::string, int, std::unordered_map<std::string, TypedValue>> data;
    // int for types UNDEFINED, NULLJS, NaN
    // map for objects

    explicit TypedValue(Attributes data) : data(data) {
        type = OBJECT;
    }

    explicit TypedValue(TYPE type) : type(type) {
        data = 0;
    }

    explicit TypedValue(const char *str) {
        data = str;
        type = STRING;
    }

    explicit TypedValue(std::string str) {
        data = str;
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

    bool operator==(const TypedValue &other) const;

    bool operator!=(const TypedValue & other) const;

    TypedValue eq(TypedValue other); // ==

    TypedValue strict_eq(TypedValue other); // ===

    TypedValue neq(TypedValue other); // ==

    TypedValue strict_neq(TypedValue other); // ===

    TypedValue get_attribute(TypedValue other);

    template<TYPE to>
    TypedValue cast();

    std::string str();
};