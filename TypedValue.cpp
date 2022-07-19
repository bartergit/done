#include "TypedValue.h"

template<>
TypedValue TypedValue::cast<BOOLEAN>() {
    if (type == NUMBER)
        return TypedValue(unwrap<float>() == 0.0f);
    if (type == BOOLEAN)
        return *this;
    if (type == STRING)
        return TypedValue(std::strlen(unwrap<const char *>()) != 0);
    if (type == OBJECT)
        return TypedValue(true);
    if (type == NULLJS)
        return TypedValue(false);
    if (type == UNDEFINED)
        return TypedValue(false);
}

template<>
TypedValue TypedValue::cast<NUMBER>() {
    if (type == NUMBER)
        return *this;
    if (type == BOOLEAN)
        return TypedValue(unwrap<bool>() ? 1.0f : 0.0f);
    if (type == STRING) {
        auto str = unwrap<const char *>();
        try {
            return TypedValue(std::stof(str));
        } catch (...) {
            if (strlen(str) == 0) {
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
}

template<>
TypedValue TypedValue::cast<STRING>() {
    if (type == NUMBER)
        return TypedValue(std::to_string(unwrap<float>()).c_str());
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
}


TypedValue TypedValue::operator+(TypedValue other) {
    if (type == OBJECT or other.type == OBJECT or type == STRING or other.type == STRING) {
        auto first = cast<STRING>().unwrap<const char *>();
        auto second = other.cast<STRING>().unwrap<const char *>();
        return TypedValue("");
//        return TypedValue((std::string(first) + second).c_str());
    }
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() + casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
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
}


TypedValue TypedValue::get_attribute(const char *attribute_name) {
    if (type == OBJECT) {
        auto search = attributes.find(attribute_name);
        if (search != attributes.end()) {
            return search->second;
        } else {
            return TypedValue(UNDEFINED);
        }
    }
    return TypedValue(UNDEFINED);
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

TypedValue TypedValue::operator==(TypedValue other) {
    if ((type == NULLJS or type == UNDEFINED) and (other.type == NULLJS or other.type == UNDEFINED)) {
        return TypedValue(true);
    }
    if ((type == NULLJS or type == UNDEFINED) or (other.type == NULLJS or other.type == UNDEFINED)) {
        return TypedValue(false);
    }
    if (type == OBJECT and other.type == OBJECT) {
//        for(auto first=attributes.begin();it!=B.end();it++) {
//        }
        return TypedValue(false);
    }
    auto casted_first = cast<NUMBER>();
    auto casted_second = other.cast<NUMBER>();
    if (casted_first.type == NUMBER and casted_second.type == NUMBER) {
        return TypedValue(casted_first.unwrap<float>() == casted_second.unwrap<float>());
    }
    if (casted_first.type == NaN or casted_second.type == NaN) {
        return TypedValue(NaN);
    }
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
        default:
            return "unreachable";
    }
}

std::string TypedValue::str() {
    std::stringstream to_return;
    std::visit(overloaded{
            [&to_return, this](int arg) { to_return << type_to_str(type); },
            [&to_return](bool arg) { to_return << (arg ? "true" : "false"); },
            [&to_return](float arg) { to_return << std::setprecision(10) << arg; },
            [&to_return](const char *arg) { to_return << std::quoted(arg); }
    }, data);
    return to_return.str();
}

