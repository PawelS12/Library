#ifndef GENRE_H
#define GENRE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Genre {
public:
    enum class GenreType {
        CRIME, HORROR, FANTASY, FAIRY_TALE, REPORTAGE, CONTEMPORARY_LITERATURE, ROMANCE, SCIENTIFIC_LITERATURE, THRILLER, POETRY, SCI_FI 
    };

private:
    GenreType selected_genre;
    static const vector<string> genres;

public:
    Genre(GenreType genre);
    Genre(const Genre& other);
    Genre& operator=(const Genre& other);
    string getSelectedGenre() const;
    static Genre selectGenre();
    static Genre fromInt(int GenreType);
};

#endif // GENRE_H