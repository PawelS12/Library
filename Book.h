#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include "Genre.h"

using std::string;

class Book {
private:
    int id; 
    string title;
    string author;
    Genre genre;
    int year;
    int pages;
    int amount;

public:
    Book(int p_id, string p_title, string p_author, Genre p_genre, int p_year, int p_pages, int p_amount);
    Book();
    
    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    int getYear() const;
    Genre getGenre() const;
    int getPages() const;
    int getAmount() const;
    void setAmount(int p_amount);
    void display() const;
};

#endif // BOOK_H