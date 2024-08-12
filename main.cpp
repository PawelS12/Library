#include <iostream>
#include "Kind.h"
#include "Book.h"
#include "BorrowedBook.h"
#include "Library.h"
#include "DatabaseException.h"
#include <limits> 

using std::cout;
using std::cin;
using std::endl;
using std::numeric_limits;
using std::streamsize;

int main() {

    Library library_1("library.db"); 
    bool is_open = true;

    do {
        cout << " \tLIBRARY MANAGEMENT SYSTEM" << endl << endl;
        cout << "Hello, select option below!" << endl;
        cout << "1\t- Add book." << endl;
        cout << "2\t- Borrow book." << endl;
        cout << "3\t- Return book." << endl;
        cout << "4\t- Remove book from library." << endl;
        cout << "5\t- Display all available books." << endl;
        cout << "6\t- Display all borrowed books." << endl;
        cout << "7\t- Display books by length." << endl;
        cout << "8\t- Count all books." << endl;
        cout << "9\t- Count books by author." << endl;
        cout << "10\t- Count books by kind." << endl;
        cout << "11\t- Search books by author." << endl;
        cout << "12\t- Search books by title." << endl;
        cout << "13\t- Search books by kind." << endl;
        cout << "14\t- Sort books by lenght." << endl;
        cout << "15\t- Clear database." << endl;
        cout << "0\t- Exit." << endl << endl;
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
                while (!(cin >> year)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "Invalid input. Please enter a number for year: ";
                }

                cout << "Enter pages: ";
                while (!(cin >> pages)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "Invalid input. Please enter a number for pages: ";
                }

                cout << "Enter amount: ";
                while (!(cin >> amount)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number for amount: ";
                }
                
                Book new_book(0, title, author, book_kind, year, pages, amount);
                
                library_1.addBook(new_book);
            }
                break;
            case 2: {
                string title, due_date, email;
                cout << "Enter title of book to borrow: ";
                getline(cin, title);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, due_date);

                cout << "Enter your personal e-mail (to contact): ";
                getline(cin, email);

                Book book = library_1.getBookByTitle(title); 
                BorrowedBook borrowed_book(0, book, due_date, email, false);

                library_1.borrowBook(borrowed_book);
            }
                break;
            case 3: {
                string title, due_date, email;
                cout << "Enter title of book to return: ";
                getline(cin, title);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, due_date);

                cout << "Enter email: ";
                getline(cin, email);

                Book book = library_1.getBookByTitle(title);
                library_1.returnBook(book.getId(), due_date, email);
            }
                break;
            case 4: {
                string title;
                int amount;
                cout << "Enter title of book to remove: ";
                getline(cin, title);

                cout << "Enter amount: ";
                while (!(cin >> amount)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number for amount: ";
                }

                library_1.removeBook(title, amount);
            }
                break;
            case 5:
                library_1.displayAllBooks();
                break;
            case 6:
                library_1.displayBorrowedBooks();
                break;
            case 7:
                library_1.displayBooksByLength();
                break;
            case 8:
                library_1.countBooks();
                break;
            case 9: {
                string author;
                cout << "Enter author of books: ";
                getline(cin, author);
                library_1.countByAuthor(author);
            }
                break;
            case 10: {
                Kind book_kind = Kind::selectKind();
                library_1.countByKind(book_kind);
            }
                break;
            case 11: {
                string author;
                cout << "Enter author to search: ";
                getline(cin, author);
                library_1.searchByAuthor(author);
            }
                break;
            case 12: {
                string title;
                cout << "Enter title to search: ";
                getline(cin, title);
                library_1.searchByTitle(title);
            }
                break;
            case 13: {
                Kind book_kind = Kind::selectKind();
                library_1.searchByKind(book_kind);
            }
                break;
            case 14: {
                string choice;
                cout << "Enter order (ascending/descending): ";
                getline(cin, choice);

                library_1.sortByLength(choice);
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