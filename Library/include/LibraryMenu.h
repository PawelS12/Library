#ifndef LIBRARY_MENU_H
#define LIBRARY_MENU_H

#include "Library.h"

class LibraryMenu {
public:
    LibraryMenu() {};
    void displayMenu();
    void handleMenu(Library& library);
    void handleAddBook(Library& library);
    void handleBorrowBook(Library& library); 
    void handleReturnBook(Library& library);
    void handleRemoveBook(Library& library);
    void handleDisplayAllBooks(Library& library);
    void handleDisplayBorrowedBooks(Library& library);
    void handleDisplayBooksByLength(Library& library); 
    void handleCountBooks(Library& library);
    void handleCountByAuthor(Library& library); 
    void handleCountByGenre(Library& library);
    void handleSearchByAuthor(Library& library); 
    void handleSearchByTitle(Library& library); 
    void handleSearchByGenre(Library& library); 
    void handleSortByLength(Library& library); 
    void handleGenerateStatistics(Library& library); 
    void handleClearDatabase(Library& library); 
};

#endif // LIBRARY_MENU_H