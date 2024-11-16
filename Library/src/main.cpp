#include <iostream>
#include <filesystem>
#include "Library.h"
#include "DatabaseException.h"
#include "DatabaseStatistics.h"
#include "Menu.h"
#include "DatabaseStatistics.h"

using std::cout;
using std::cerr;
using std::endl;

int main() {

    try {

        Library library_1("../Library/database/library.db"); 
        Menu menu_1;
        menu_1.handleMenu(library_1);

    } catch (const DatabaseException& e) {

        cerr << "Failed to open library manager: " << e.what() << endl;
        
    }
    
    return 0;
}