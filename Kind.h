#ifndef KIND_H
#define KIND_H

#include <string>
#include <vector>

class Kind {
public:
    enum class KindType {
        CRIME, HORROR, FANTASY, FAIRY_TALE, REPORTAGE, CONTEMPORARY_LITERATURE, ROMANCE, SCIENTIFIC_LITERATURE, THRILLER, POETRY, SCI_FI 
    };

private:
    KindType selected_kind;
    const std::vector<std::string> kinds = {"Crime", "Horror", "Fantasy", "Fairy tale", "Reportage", "Contemporary literature", "Romance", "Scientific literature", "Thriller", "Poetry", "Science fiction"};

public:
    Kind(KindType kind);
    Kind(const Kind& other);
    Kind& operator=(const Kind& other);
    std::string getSelectedKind() const;
};

#endif // KIND_H