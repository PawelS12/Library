#include <iostream>
#include "LibraryMenu.h"
#include "Genre.h"
#include "Book.h"
#include "BorrowedBook.h"
#include "Library.h"
#include "DatabaseException.h"
#include "DatabaseStatistics.h"
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::numeric_limits;
using std::streamsize;

void Menu::displayMenu() {
    cout << "=============================================" << endl;
        cout << "           LIBRARY MANAGEMENT SYSTEM        " << endl;
        cout << "=============================================" << endl << endl;
        cout << "Hello, select option below!" << endl;
        cout << "1\t- Add a book." << endl;
        cout << "2\t- Borrow a book." << endl;
        cout << "3\t- Return a book." << endl;
        cout << "4\t- Remove a book from library." << endl;
        cout << "5\t- Display all available books." << endl;
        cout << "6\t- Display all borrowed books." << endl;
        cout << "7\t- Display books divided into long and short." << endl;
        cout << "8\t- Count all books." << endl;
        cout << "9\t- Count books by author." << endl;
        cout << "10\t- Count books by genre." << endl;
        cout << "11\t- Search books by author." << endl;
        cout << "12\t- Search books by title." << endl;
        cout << "13\t- Search books by genre." << endl;
        cout << "14\t- Sort books by length." << endl;
        cout << "15\t- Generate library statistics." << endl;
        cout << "16\t- Clear the database." << endl;
        cout << "0\t- Exit." << endl << endl;
        cout << "Enter option: ";
}

void Menu::handleMenu(Library& library) {
    bool is_open = true;

    do {
        displayMenu();

        int option;
        cin >> option;
        cin.ignore();

        cout << endl;

        switch (option) {
            case 1:
                handleAddBook(library);
                break;
            case 2:
                handleBorrowBook(library);
                break;
            case 3: 
                handleReturnBook(library);
                break;
            case 4: 
                handleReturnBook(library);
                break;
            case 5:
                handleDisplayAllBooks(library);
                break;
            case 6:
                handleDisplayBorrowedBooks(library);
                break;
            case 7:
                handleDisplayBooksByLength(library);
                break;
            case 8:
                handleCountBooks(library);
                break;
            case 9: 
                handleCountByAuthor(library);
                break;
            case 10: 
                handleCountByGenre(library);
                break;
            case 11: 
                handleSearchByAuthor(library);
                break;
            case 12: 
                handleSearchByTitle(library);
                break;
            case 13: 
                handleSearchByGenre(library);
                break;
            case 14: 
                handleSortByLength(library);
                break;
            case 15: 
                handleGenerateStatistics(library);
                break;
            case 16: 
                handleClearDatabase(library);
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

void Menu::handleAddBook(Library& library){
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

void Menu::handleBorrowBook(Library& library) {
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

void Menu::handleReturnBook(Library& library) {
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

void handleRemoveBook(Library& library) {
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

void Menu::handleDisplayAllBooks(Library& library) {
    library.displayAllBooks();
}

void Menu::handleDisplayBorrowedBooks(Library& library) {
    library.displayBorrowedBooks();
}

void Menu::handleDisplayBooksByLength(Library& library) {
    library.displayBooksByLength();
}

void Menu::handleCountBooks(Library& library) {
    library.countBooks();
}

void Menu::handleCountByAuthor(Library& library) {
    string author;
    cout << "Enter author of books: ";
    getline(cin, author);
    library.countByAuthor(author);
}

void Menu::handleCountByGenre(Library& library) {
    Genre book_genre = Genre::selectGenre();
    library.countByGenre(book_genre);
}

void Menu::handleSearchByAuthor(Library& library) {
    string author;
    cout << "Enter author to search: ";
    getline(cin, author);
    library.searchByAuthor(author);
}

void Menu::handleSearchByTitle(Library& library) {
    string title;
    cout << "Enter title to search: ";
    getline(cin, title);
    library.searchByTitle(title);
}

void Menu::handleSearchByGenre(Library& library) {
    Genre book_genre = Genre::selectGenre();
    library.searchByGenre(book_genre);
}

void Menu::handleSortByLength(Library& library) {
    string choice;
    cout << "Enter order (ascending/descending): ";
    getline(cin, choice);
    library.sortByLength(choice);
}

void Menu::handleGenerateStatistics(Library& library) {
    DatabaseStatistics statistics(library.getDatabase());
    statistics.generateStatistics("../Library/statistics/library_stats.txt", false);  
    statistics.generateStatistics("../Library/statistics/library_stats.csv", true);
    statistics.generateBooksDetails("../Library/statistics/books_details.txt", "../Library/statistics/books_details.csv");
}

void Menu::handleClearDatabase(Library& library) {
    string answer;
    cout << "Are you sure to clear all data? (yes/no)" << endl;
    cin >> answer;
    if (answer == "yes") {
        library.clearDatabase();
        cout << "The database has been successfully cleared." << endl << endl;
    } else {
        cout << "Database clearing operation canceled." << endl << endl;
    }
}