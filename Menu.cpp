#include <iostream>
#include "Menu.h"
#include "Genre.h"
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

void displayMenu(Library& library) {
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
        cout << "10\t- Count books by genre." << endl;
        cout << "11\t- Search books by author." << endl;
        cout << "12\t- Search books by title." << endl;
        cout << "13\t- Search books by genre." << endl;
        cout << "14\t- Sort books by length." << endl;
        cout << "15\t- Clear database." << endl;
        cout << "0\t- Exit." << endl << endl;
        cout << "Enter option." << endl;

        int option;
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1: {
                const size_t max_title_length = 100;  
                string title, author, genre;
                int year, pages, amount;

                cout << "Enter title: ";
                while (getline(cin, title)) {
                    if (title.length() > max_title_length) {
                        cout << "Title is too long. Maximum length is " << max_title_length << " characters. Please try again: ";
                    } else {
                        break;
                    }
                }
                
                cout << "Enter author: ";
                getline(cin, author);

                Genre book_genre = Genre::selectGenre();

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
                
                Book new_book(0, title, author, book_genre, year, pages, amount);
                library.addBook(new_book);
            }
                break;
            case 2: {
                string title, author, due_date, email;
                cout << "Enter title of book to borrow: ";
                getline(cin, title);

                cout << "Enter author of book to borrow: ";
                getline(cin, author);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, due_date);

                cout << "Enter your personal e-mail (to contact): ";
                getline(cin, email);

                Book book = library.getBookByTitle(title, author); 
                BorrowedBook borrowed_book(0, book, due_date, email, false);

                library.borrowBook(borrowed_book);
            }
                break;
            case 3: {
                string title, author, due_date, email;
                cout << "Enter title of book to return: ";
                getline(cin, title);

                cout << "Enter author of book to return: ";
                getline(cin, author);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, due_date);

                cout << "Enter email: ";
                getline(cin, email);

                Book book = library.getBookByTitle(title, author);
                library.returnBook(book.getId(), due_date, email);
            }
                break;
            case 4: {
                string title, author;
                int amount;
                cout << "Enter title of book to remove: ";
                getline(cin, title);

                cout << "Enter author of book to remove: ";
                getline(cin, author);

                cout << "Enter amount: ";
                while (!(cin >> amount)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number for amount: ";
                }

                library.removeBook(title, author, amount);
            }
                break;
            case 5:
                library.displayAllBooks();
                break;
            case 6:
                library.displayBorrowedBooks();
                break;
            case 7:
                library.displayBooksByLength();
                break;
            case 8:
                library.countBooks();
                break;
            case 9: {
                string author;
                cout << "Enter author of books: ";
                getline(cin, author);
                library.countByAuthor(author);
            }
                break;
            case 10: {
                Genre book_genre = Genre::selectGenre();
                library.countByGenre(book_genre);
            }
                break;
            case 11: {
                string author;
                cout << "Enter author to search: ";
                getline(cin, author);
                library.searchByAuthor(author);
            }
                break;
            case 12: {
                string title;
                cout << "Enter title to search: ";
                getline(cin, title);
                library.searchByTitle(title);
            }
                break;
            case 13: {
                Genre book_genre = Genre::selectGenre();
                library.searchByGenre(book_genre);
            }
                break;
            case 14: {
                string choice;
                cout << "Enter order (ascending/descending): ";
                getline(cin, choice);

                library.sortByLength(choice);
            }
                break;
            case 15: {
                string answer;
                cout << "Are you sure to clear all data? (yes/no)" << endl;
                cin >> answer;
                if (answer == "yes") {
                    library.clearDatabase();
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
}