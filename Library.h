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
    static int callback(void* data, int argc, char** argv, char** azColName);
    static int callbackCount(void* data, int argc, char** argv, char** azColName);

public:
    Library(const string& db_name);
    ~Library();
    void addBook(const Book& book);
    void borrowBook(const BorrowedBook& borrowedBook);
    void returnBook(int bookId, const string& dueDate);
    void removeBook(const string& title, int amount);
    void removeBorrowedBook(const string& title, const string& dueDate);
    void clearDatabase();
    void displayAllBooks();
    void displayBorrowedBooks();
    void displayReturnedBooks();
    void displayBooksByLength() const;
    void countBooks() const;
    void searchByTitle(const string& p_title) const;
    void searchByAuthor(const string& p_author) const;
    void searchByKind(const Kind& p_kind) const;
    void countByKind(const Kind& p_kind) const;
    void countByAuthor(const string& p_author) const;
    Book getBookByTitle(const string& title) const;
};

#endif // LIBRARY_H