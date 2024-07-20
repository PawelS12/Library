#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include "Book.h"
#include "BorrowedBook.h"

class Library {
private:
    sqlite3* db;
    static int callback(void* data, int argc, char** argv, char** azColName);
    static int callbackCount(void* data, int argc, char** argv, char** azColName);

public:
    Library(const std::string& db_name);
    ~Library();
    void addBook(const Book& book);
    void borrowBook(const BorrowedBook& borrowedBook);
    void returnBook(int borrowedBookId);
    void removeBook(const std::string& title);
    void removeBorrowedBook(const std::string& title, const std::string& dueDate);
    void clearDatabase();
    void displayAllBooks();
    void displayBorrowedBooks();
    void displayReturnedBooks();
    void displayBooksByLength() const;
    void countBooks() const;
    void searchByTitle(const std::string& p_title) const;
    void searchByAuthor(const std::string& p_author) const;
    void searchByKind(const Kind& p_kind) const;
    void countByKind(const Kind& p_kind) const;
    void countByAuthor(const std::string& p_author) const;
};

#endif // LIBRARY_H