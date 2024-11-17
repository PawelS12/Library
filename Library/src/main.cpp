#include <iostream>
#include <filesystem>
#include "Library.h"
#include "DatabaseException.h"
#include "DatabaseStatistics.h"
#include "LibraryMenu.h"

using std::cout;
using std::cerr;
using std::endl;

int main() {

    try {

        Library library("../Library/database/library.db"); 
        LibraryMenu menu;
        menu.handleMenu(library);

    } catch (const DatabaseException& e) {

        cerr << "Failed to open library manager: " << e.what() << endl;
        
    }
    
    return 0;
}