#include "variant.h"

namespace mo = moroxus;

int main() {
    mo::Variant mojVariant("hodnota");
    mo::Variant a("aaa");

    std::move(mojVariant);
    std::map<std::string, mo::Variant> mapa;

    mapa.emplace("kluc1", std::move(a));
    std::cerr << mapa["kluc1"].toString() << '\n';
}
