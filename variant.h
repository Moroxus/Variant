#ifndef VARIANT_H
#define VARIANT_H

#include <list>
#include <map>
#include <memory>
#include <type_traits>
#include <variant>

namespace moroxus {

class Variant {
private:
    using VariantOwner =
        std::variant<int, double, std::unique_ptr<std::string>, std::unique_ptr<std::list<Variant>>,
                     std::unique_ptr<std::map<std::string, Variant>>>;
    VariantOwner data;

public:
    Variant() = default;
    Variant(int value): data(value) {}
    Variant(double value): data(value) {}
    Variant(const std::string& value): data(std::make_unique<std::string>(value)) {}

    Variant(const Variant& other) {
        const std::size_t index = other.data.index();
        switch (index) {
            case 0: data = std::get<0>(other.data); break;
            case 1: data = std::get<1>(other.data); break;
            case 2: data = std::make_unique<std::string>(*std::get<2>(other.data)); break;
            case 3: data = std::make_unique<std::list<Variant>>(*std::get<3>(other.data)); break;
            case 4:
                data = std::make_unique<std::map<std::string, Variant>>(*std::get<4>(other.data));
                break;
            default: break;
        }
    }

    Variant(Variant&& other) = default;
    ~Variant()               = default;

    friend void swap(Variant& first, Variant& second) {
        using std::swap;
        swap(first.data, second.data);
    }

    Variant& operator=(Variant other) {
        swap(*this, other);
        return *this;
    }

    Variant& operator=(int value) {
        data = value;
        return *this;
    }

    Variant& operator=(double value) {
        data = value;
        return *this;
    }

    Variant& operator=(const std::string& value) { return assign(value); }

    Variant& operator=(std::string&& value) { return assign(std::move(value)); }

    Variant& operator=(const std::list<Variant>& value) { return assign(value); }

    Variant& operator=(std::list<Variant>&& value) { return assign(std::move(value)); }

    Variant& operator=(const std::map<std::string, Variant>& value) { return assign(value); }

    Variant& operator=(std::map<std::string, Variant>&& value) { return assign(value); }

    int& toInt() { return std::get<0>(data); }

    double& toDouble() { return std::get<1>(data); }

    std::string& toString() { return *std::get<2>(data); }

    std::list<Variant> toList() { return *std::get<3>(data); }

    std::map<std::string, Variant> toMap() { return *std::get<4>(data); }

private:
    template <typename T>
    Variant& assign(T&& value) {
        if (std::get_if<std::unique_ptr<typename std::decay<T>::type>>(&data)) {
            *std::get<std::unique_ptr<typename std::decay<T>::type>>(data) = std::forward<T>(value);
        } else {
            data = std::make_unique<typename std::decay<T>::type>(std::forward<T>(value));
        }
        return *this;
    }
};

}    // namespace moroxus

#endif    // VARIANT_H
