#include "Genre.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::invalid_argument;
using std::transform;
using std::tolower;

const vector<string> Genre::genres = {"Crime", "Horror", "Fantasy", "Fairy tale", "Reportage", "Contemporary literature", "Romance", "Scientific literature", "Thriller", "Poetry", "Science fiction"};

Genre::Genre(GenreType Genre) : selected_genre(Genre) {}

Genre::Genre(const Genre& other) : selected_genre(other.selected_genre) {}

Genre& Genre::operator=(const Genre& other) {
    if (this != &other) {
        selected_genre = other.selected_genre;
    }
    return *this;
}

string Genre::getSelectedGenre() const {
    return genres[static_cast<int>(selected_genre)]; 
}

Genre Genre::fromInt(int genreType) {
    if (genreType < 0 || genreType >= genres.size()) {
        throw invalid_argument("Invalid genre type");
    }
    return Genre(static_cast<GenreType>(genreType));
}

Genre Genre::selectGenre() {
    string genre_name;
    cout << "Enter the genre of book (crime, horror, fantasy, fairy tale, reportage, contemporary literature, romance, scientific literature, thriller, poetry, sci-fi): ";
    getline(cin, genre_name);

    transform(genre_name.begin(), genre_name.end(), genre_name.begin(), [](unsigned char c) { return tolower(c); });

    if (genre_name == "crime") {
        return Genre(GenreType::CRIME);
    } else if (genre_name == "horror") {
        return Genre(GenreType::HORROR);
    } else if (genre_name == "fantasy") {
        return Genre(GenreType::FANTASY);
    } else if (genre_name == "fairy tale" || genre_name == "fairy_tale") {
        return Genre(GenreType::FAIRY_TALE);
    } else if (genre_name == "reportage") {
        return Genre(GenreType::REPORTAGE);
    } else if (genre_name == "contemporary literature" || genre_name == "contemporary_literature") {
        return Genre(GenreType::CONTEMPORARY_LITERATURE);
    } else if (genre_name == "romance") {
        return Genre(GenreType::ROMANCE);
    } else if (genre_name == "scientific literature" || genre_name == "scientific_literature") {
        return Genre(GenreType::SCIENTIFIC_LITERATURE);
    } else if (genre_name == "thriller") {
        return Genre(GenreType::THRILLER);
    } else if (genre_name == "poetry") {
        return Genre(GenreType::POETRY);
    } else if (genre_name == "sci-fi" || genre_name == "science fiction" || genre_name == "science_fiction") {
        return Genre(GenreType::SCI_FI);
    } else {
        cout << "Invalid genre entered. Defaulting to CRIME." << endl;
        return Genre(GenreType::CRIME);
    }
}