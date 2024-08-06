#include <iostream>
#include "Kind.h"
#include "Book.h"
#include "BorrowedBook.h"
#include "Library.h"
#include "DatabaseException.h"

using std::cout;
using std::cin;
using std::endl;

int main() {

    Library library_1("library.db");

    // Book book1(1, "Hobbit", "J.R.R Tolkien", fantasy, 1900, 400, 5);
    // Book book2(2, "The Lord of the rings", "J.R.R Tolkien", fantasy, 1901, 1000, 3);
    // Book book3(3, "Eden", "Stanislaw Lem", science_fiction, 1970, 350, 1);

    // library_1.addBook(book1);
    // library_1.addBook(book2);
    // library_1.addBook(book3); 

    // BorrowedBook borrowed_1(1, book1, "2024-07-03", false); 
    // library_1.borrowBook(borrowed_1); 
    
    bool is_open = true;

    do {
        cout << " \tLIBRARY MANAGEMENT SYSTEM" << endl << endl;
        cout << "Hello, select option below!" << endl;
        cout << "1 - Add book." << endl;
        cout << "2 - Borrow book." << endl;
        cout << "3 - Return book." << endl;
        cout << "4 - Remove book from library." << endl;
        cout << "5 - Remove borrowed book." << endl;
        cout << "6 - Display all books." << endl;
        cout << "7 - Display all borrowed books." << endl;
        cout << "8 - Display books by length." << endl;
        cout << "9 - Count all books." << endl;
        cout << "10 - Count books by author." << endl;
        cout << "11 - Count books by kind." << endl;
        cout << "12 - Search books by author." << endl;
        cout << "13 - Search books by title." << endl;
        cout << "14 - Search books by kind." << endl;
        cout << "15 - Clear database." << endl;
        cout << "0 - Exit." << endl << endl;
        cout << "Enter option." << endl;

        int option;
        cin >> option;

        cin.ignore();

        switch (option) {
            case 1: {
                string title, author, kind;
                int year, pages, amount;

                cout << "Enter title: ";
                getline(cin, title);

                cout << "Enter author: ";
                getline(cin, author);

                Kind book_kind = Kind::selectKind();

                cout << "Enter year: ";
                cin >> year;

                cout << "Enter pages: ";
                cin >> pages;

                cout << "Enter amount: ";
                cin >> amount;
                cin.ignore();
                
                Book new_book(0, title, author, book_kind, year, pages, amount);
                
                library_1.addBook(new_book);
                cout << "Book added successfully." << endl;
            }
                break;
            case 2: {
                string title, dueDate;
                cout << "Enter title of book to borrow: ";
                getline(cin, title);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDate);

                Book book = library_1.getBookByTitle(title); 
                BorrowedBook borrowedBook(0, book, dueDate, false);

                library_1.borrowBook(borrowedBook);
                cout << "Book borrowed successfully." << endl;
            }
                break;
            case 3: {
                string title, dueDate;
                cout << "Enter title of book to return: ";
                getline(cin, title);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDate);

                Book book = library_1.getBookByTitle(title);
                library_1.returnBook(book.getId());
            }
                break;
            case 4:

                break;
            case 5:
                break;
            case 6:
                library_1.displayAllBooks();
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 14:
                break;
            case 0:
                is_open = false;
                break;
            default:
                cout << "Please try again." << endl;
                break;
        }

    } while(is_open);

    return 0;
}