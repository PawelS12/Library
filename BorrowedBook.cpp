#include "BorrowedBook.h"

using std::cout;
using std::endl;
using std::string;

BorrowedBook::BorrowedBook(int p_id, const Book& p_book, string p_dueDate, string p_email, bool p_returned)
    : id(p_id), book(p_book), due_date(p_dueDate), email(p_email), returned(p_returned) {}

int BorrowedBook::getId() const {
    return id;
}

const Book& BorrowedBook::getBook() const {
    return book;
}

string BorrowedBook::getDueDate() const {
    return due_date;
}

string BorrowedBook::getEmail() const {
    return email;
}

bool BorrowedBook::isReturned() const {
    return returned; 
}

void BorrowedBook::markReturned() {
    returned = true; 
}

void BorrowedBook::displayDetails() const {
    book.display(); 
    cout << "Due date: " << due_date << endl;
    cout << "Returned: " << (returned ? "Yes" : "No") << endl;
}