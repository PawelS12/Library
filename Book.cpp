#include "Book.h"

using std::cout;
using std::endl;
using std::string;

Book::Book(int p_id, string p_title, string p_author, Kind p_kind, int p_year, int p_pages, int p_amount)
    : id(p_id), title(p_title), author(p_author), kind(p_kind), year(p_year), pages(p_pages), amount(p_amount) {}

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

Kind Book::getKind() const {
    return kind;
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
    cout << "Data: " << author << ", " << year << ", " << kind.getSelectedKind() << ", " << pages << " pages" << ", " << amount << endl;
}