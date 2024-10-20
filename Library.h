#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include "Book.h"
#include "BorrowedBook.h"

using std::string;

class Library {
private:
    sqlite3* db;
    static int count_callback(void* data, int argc, char** argv, char** az_col_name);
    static int sort_callback(void* data, int argc, char** argv, char** az_col_name);
    static int display_callback(void* data, int argc, char** argv, char** az_col_name);

public:
    Library(const string& db_name);
    ~Library();
    void addBook(const Book& book);
    void borrowBook(const BorrowedBook& borrowed_book);
    void returnBook(int book_id, const string& due_date, const string& email);
    void removeBook(const string& title, const string& author, int amount);
    void removeBorrowedBook(const string& title, const string& due_date);
    void clearDatabase();
    void displayAllBooks();
    void displayBorrowedBooks();
    void displayReturnedBooks();
    void displayBooksByLength() const;
    void countBooks() const;
    void searchByTitle(const string& p_title) const;
    void searchByAuthor(const string& p_author) const;
    void searchByGenre(const Genre& p_genre) const;
    void countByGenre(const Genre& p_genre) const;
    void countByAuthor(const string& p_author) const;
    void sortByLength(const string& p_choice) const;
    void sqlExecute(const string& sql, int (*callback)(void*,int,char**,char**)) const;
    Book getBookByTitle(const string& title, const string& author) const;
};

#endif // LIBRARY_H