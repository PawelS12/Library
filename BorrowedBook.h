#ifndef BORROWEDBOOK_H
#define BORROWEDBOOK_H

#include <string>
#include "Book.h"

using std::string;

class BorrowedBook {
private:
    int id; 
    const Book& book;
    string due_date;
    bool returned;

public:
    BorrowedBook(int p_id, const Book& p_book, string p_dueDate, bool p_returned);

    int getId() const;
    const Book& getBook() const;
    string getDueDate() const;
    bool isReturned() const;
    void markReturned();
    void displayDetails() const;
};

#endif // BORROWEDBOOK_H