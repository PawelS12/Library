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
    bool is_open = true;

    do {
        cout << " \tLIBRARY MANAGEMENT SYSTEM" << endl << endl;
        cout << "Hello, select option below!" << endl;
        cout << "1 - Add book." << endl;
        cout << "2 - Borrow book." << endl;
        cout << "3 - Return book." << endl;
        cout << "4 - Remove book from library." << endl;
        cout << "5 - Remove borrowed book." << endl;
        cout << "6 - Display all available books." << endl;
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
                library_1.returnBook(book.getId(), dueDate);
                cout << "Borrowed book returned successfully." << endl;
            }
                break;
            case 4: {
                string title;
                int amount;
                cout << "Enter title of book to remove: ";
                getline(cin, title);

                cout << "Enter amount of book to remove: ";
                cin >> amount;

                library_1.removeBook(title, amount);
                cout << "Book removed successfully." << endl;
            }
                break;
            case 5: {
                string title, dueDate;
                cout << "Enter title of borrowed book to remove: ";
                getline(cin, title);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDate);

                library_1.removeBorrowedBook(title, dueDate);
                cout << "Borrowed book removed successfully." << endl;
            }
                break;
            case 6:
                library_1.displayAllBooks();
                break;
            case 7:
                library_1.displayBorrowedBooks();
                break;
            case 8:
                library_1.displayBooksByLength();
                break;
            case 9:
                library_1.countBooks();
                break;
            case 10: {
                string author;
                cout << "Enter author of books: ";
                getline(cin, author);
                library_1.countByAuthor(author);
            }
                break;
            case 11: {
                Kind book_kind = Kind::selectKind();
                library_1.countByKind(book_kind);
            }
                break;
            case 12: {
                string author;
                cout << "Enter author to search: ";
                getline(cin, author);
                library_1.searchByAuthor(author);
            }
                break;
            case 13: {
                string title;
                cout << "Enter title to search: ";
                getline(cin, title);
                library_1.searchByTitle(title);
            }
                break;
            case 14: {
                Kind book_kind = Kind::selectKind();
                library_1.searchByKind(book_kind);
            }
                break;
            case 15: {
                string answer;
                cout << "Are you sure to clear all data? (yes/no)" << endl;
                cin >> answer;
                if (answer == "yes") {
                    library_1.clearDatabase();
                } else {
                    break;
                }
            }
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