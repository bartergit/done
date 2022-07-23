#include "TypedValue.h"
#include <sstream>
#include <utility>

template<>
TypedValue TypedValue::cast<BOOLEAN>() {
    if (type == NUMBER)
        return TypedValue(unwrap<float>() == 0.0f);
    if (type == BOOLEAN)
        return *this;
    if (type == STRING)
        return TypedValue(!unwrap<std::string>().empty());
    if (type == OBJECT)
        return TypedValue(true);
    if (type == NULLJS)
        return TypedValue(false);
    if (type == UNDEFINED)
        return TypedValue(false);
    std::unreachable();
}

template<>
TypedValue TypedValue::cast<NUMBER>() {
    if (type == NUMBER)
        return *this;
    if (type == BOOLEAN)
        return TypedValue(unwrap<bool>() ? 1.0f : 0.0f);
    if (type == STRING) {
        auto str = unwrap<std::string>();
        try {
            return TypedValue(std::stof(str));
        } catch (...) {
            if (str.empty()) {
                return TypedValue(0.0f);
            }
            return TypedValue(NaN);
        }
    }
    if (type == OBJECT)
        return TypedValue(NaN);
    if (type == NULLJS)
        return TypedValue(0.0f);
    if (type == UNDEFINED)
        return TypedValue(0.0f);
    std::unreachable();
}

template<>
TypedValue TypedValue::cast<STRING>() {
    if (type == NUMBER)
        return TypedValue(std::to_string(unwrap<float>()));
    if (type == BOOLEAN)
        return TypedValue(unwrap<bool>() ? "true" : "false");
    if (type == STRING)
        return *this;
    if (type == OBJECT)
        return TypedValue("[Object object]");
    if (type == NULLJS)
        return TypedValue("null");
    if (type == UNDEFINED)
        return TypedValue("undefined");
    std::unreachable();
}


TypedValue TypedValue::operator+(TypedValue other) {
    if (type == OBJECT or other.type == OBJECT or type == STRING or other.type == STRING) {
        auto first = cast<STRING>().unwrap<std::string>();
        auto second = other.cast<STRING>().unwrap<std::string>();
        return TypedValue(first + second);
    }
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() + casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
    std::unreachable();
}

TypedValue TypedValue::operator-(TypedValue other) {
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() - casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
    std::unreachable();
}

TypedValue TypedValue::operator/(TypedValue other) {
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() / casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
    std::unreachable();
}

TypedValue TypedValue::operator*(TypedValue other) {
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() * casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
    std::unreachable();
}

TypedValue TypedValue::operator%(TypedValue other) {
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        auto first_value = casted_first.unwrap<float>();
        auto second_value = casted_second.unwrap<float>();
        return TypedValue(first_value - second_value * int(first_value / second_value));
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
    std::unreachable();
}

TypedValue TypedValue::operator&&(TypedValue other) {
    if (!cast<BOOLEAN>().unwrap<bool>()) {
        return *this;
    }
    return other;
}

TypedValue TypedValue::operator||(TypedValue other) {
    if (cast<BOOLEAN>().unwrap<bool>()) {
        return *this;
    }
    return other;
}

TypedValue TypedValue::eq(TypedValue other) {
    if ((type == NULLJS or type == UNDEFINED) and (other.type == NULLJS or other.type == UNDEFINED)) {
        return TypedValue(true);
    }
    if ((type == NULLJS or type == UNDEFINED) or (other.type == NULLJS or other.type == UNDEFINED)) {
        return TypedValue(false);
    }
    if (type == OBJECT and other.type == OBJECT) {
        auto attributes_1 = unwrap<Attributes>();
        auto attributes_2 = other.unwrap<Attributes>();
        if (attributes_1.size() != attributes_2.size()) {
            return TypedValue(false);
        }
        for (auto &[key, value]: attributes_1) {
            if (not(attributes_2.contains(key) && attributes_2.at(key) == (value))) {
                return TypedValue(false);
            }
        }
        return TypedValue(true);
    }
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() == casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(false);
    }
    std::unreachable();
}

TypedValue TypedValue::strict_eq(TypedValue other) {
    return TypedValue(*this == other);
}

bool TypedValue::operator==(const TypedValue &other) const {
    return type == other.type && data == other.data;
}

bool TypedValue::operator!=(const TypedValue &other) const {
    return *this == (other);
}


TypedValue TypedValue::get_attribute(TypedValue other) {
    auto casted = other.cast<STRING>();
    if (type == OBJECT) {
        auto attributes = unwrap<Attributes>();
        auto search = attributes.find(casted.str());
        if (search != attributes.end()) {
            return search->second;
        } else {
            return TypedValue(UNDEFINED);
        }
    }
    return TypedValue(UNDEFINED);
}


std::string type_to_str(TYPE type) {
    switch (type) {
        case NaN:
            return "NaN";
        case NULLJS:
            return "null";
        case UNDEFINED:
            return "undefined";
        case NUMBER:
            return "number";
        case BOOLEAN:
            return "boolean";
        case STRING:
            return "string";
        case OBJECT:
            return "object";
        default:
            std::unreachable();
    }
}

std::string TypedValue::str() {
    std::stringstream to_return;
    std::visit(overloaded{
            [&to_return](std::unordered_map<std::string, TypedValue> &arg) {
                to_return << "{";
                int i = 0;
                for (auto &[key, value]: arg) {
                    i++;
                    to_return << key << ":" << value.str() << (i == arg.size() ? "" : ",");
                }
                to_return << "}";
            },
            [&to_return, this](int arg) { to_return << type_to_str(type); },
            [&to_return](bool arg) { to_return << (arg ? "true" : "false"); },
            [&to_return](float arg) { to_return << std::setprecision(10) << arg; },
            [&to_return](std::string arg) { to_return << std::quoted(arg); }
    }, data);
    return to_return.str();
}

TypedValue TypedValue::neq(TypedValue other) {
    return TypedValue(not this->eq(std::move(other)).unwrap<bool>());
}

TypedValue TypedValue::strict_neq(TypedValue other) {
    return TypedValue(not this->strict_eq(std::move(other)).unwrap<bool>());
}





