#ifndef BORROWEDBOOK_H
#define BORROWEDBOOK_H

#include <string>
#include "Book.h"

class BorrowedBook {
private:
    int id; 
    const Book& book;
    std::string due_date;
    bool returned;

public:
    BorrowedBook(int p_id, const Book& p_book, std::string p_dueDate, bool p_returned);

    int getId() const;
    const Book& getBook() const;
    std::string getDueDate() const;
    bool isReturned() const;
    void markReturned();
    void displayDetails() const;
};

#endif // BORROWEDBOOK_H