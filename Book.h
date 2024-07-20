#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include "Kind.h"

class Book {
private:
    int id; 
    std::string title;
    std::string author;
    Kind kind;
    int year;
    int pages;
    int amount;

public:
    Book(int p_id, std::string p_title, std::string p_author, Kind p_kind, int p_year, int p_pages, int p_amount);

    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    Kind getKind() const;
    int getPages() const;
    int getAmount() const;
    void setAmount(int p_amount);
    void display() const;
};

#endif // BOOK_H