#include "Kind.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::cin;

const vector<string> Kind::kinds = {"Crime", "Horror", "Fantasy", "Fairy tale", "Reportage", "Contemporary literature", "Romance", "Scientific literature", "Thriller", "Poetry", "Science fiction"};

Kind::Kind(KindType kind) : selected_kind(kind) {}

Kind::Kind(const Kind& other) : selected_kind(other.selected_kind) {}

Kind& Kind::operator=(const Kind& other) {
    if (this != &other) {
        selected_kind = other.selected_kind;
    }
    return *this;
}

string Kind::getSelectedKind() const {
    return kinds[static_cast<int>(selected_kind)]; 
}

Kind Kind::fromInt(int kindType) {
    if (kindType < 0 || kindType >= kinds.size()) {
        throw std::invalid_argument("Invalid kind type");
    }
    return Kind(static_cast<KindType>(kindType));
}

Kind Kind::selectKind() {
    string kind_name;
    cout << "Enter the kind of book (crime, horror, fantasy, fairy tale, reportage, contemporary literature, romance, scientific literature, thriller, poetry, sci-fi): ";
    getline(cin, kind_name);

    std::transform(kind_name.begin(), kind_name.end(), kind_name.begin(), [](unsigned char c) { return std::tolower(c); });

    cout << "Processed kind_name: " << kind_name << endl;

    if (kind_name == "crime") {
        return Kind(KindType::CRIME);
    } else if (kind_name == "horror") {
        return Kind(KindType::HORROR);
    } else if (kind_name == "fantasy") {
        return Kind(KindType::FANTASY);
    } else if (kind_name == "fairy tale" || kind_name == "fairy_tale") {
        return Kind(KindType::FAIRY_TALE);
    } else if (kind_name == "reportage") {
        return Kind(KindType::REPORTAGE);
    } else if (kind_name == "contemporary literature" || kind_name == "contemporary_literature") {
        return Kind(KindType::CONTEMPORARY_LITERATURE);
    } else if (kind_name == "romance") {
        return Kind(KindType::ROMANCE);
    } else if (kind_name == "scientific literature" || kind_name == "scientific_literature") {
        return Kind(KindType::SCIENTIFIC_LITERATURE);
    } else if (kind_name == "thriller") {
        return Kind(KindType::THRILLER);
    } else if (kind_name == "poetry") {
        return Kind(KindType::POETRY);
    } else if (kind_name == "sci-fi" || kind_name == "science fiction" || kind_name == "science_fiction") {
        return Kind(KindType::SCI_FI);
    } else {
        cout << "Invalid kind entered. Defaulting to CRIME." << endl;
        return Kind(KindType::CRIME);
    }
}