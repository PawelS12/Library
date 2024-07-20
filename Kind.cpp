#include "Kind.h"

Kind::Kind(KindType kind) : selected_kind(kind) {}

Kind::Kind(const Kind& other) : selected_kind(other.selected_kind) {}

Kind& Kind::operator=(const Kind& other) {
    if (this != &other) {
        selected_kind = other.selected_kind;
    }
    return *this;
}

std::string Kind::getSelectedKind() const {
    return kinds[static_cast<int>(selected_kind)]; 
}
