#include "Book.h"

using std::cout;
using std::endl;
using std::string;

Book::Book(int p_id, string p_title, string p_author, Genre p_genre, int p_year, int p_pages, int p_amount)
    : id(p_id), title(p_title), author(p_author), genre(p_genre), year(p_year), pages(p_pages), amount(p_amount) {}

Book::Book() : id(0), title(""), author(""), genre(Genre(Genre::GenreType::CRIME)), year(0), pages(0), amount(0) {}

int Book::getId() const {
    return id;
}

string Book::getTitle() const {
    return title;
}

string Book::getAuthor() const {
    return author;
}

int Book::getYear() const {
    return year;
}

Genre Book::getGenre() const {
    return genre;
}

int Book::getPages() const {
    return pages;
}

int Book::getAmount() const {
    return amount;
}

void Book::setAmount(int p_amount) {
    amount = p_amount;
}

void Book::display() const {
    cout << "Book: " << title << endl;
    cout << "Data: " << author << ", " << year << ", " << genre.getSelectedGenre() << ", " << pages << " pages" << ", " << amount << endl;
}