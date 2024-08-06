#ifndef KIND_H
#define KIND_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Kind {
public:
    enum class KindType {
        CRIME, HORROR, FANTASY, FAIRY_TALE, REPORTAGE, CONTEMPORARY_LITERATURE, ROMANCE, SCIENTIFIC_LITERATURE, THRILLER, POETRY, SCI_FI 
    };

private:
    KindType selected_kind;
    static const vector<string> kinds;

public:
    Kind(KindType kind);
    Kind(const Kind& other);
    Kind& operator=(const Kind& other);
    string getSelectedKind() const;
    static Kind selectKind();
    static Kind fromInt(int kindType);
};

#endif // KIND_H