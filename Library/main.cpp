#include <iostream>
#include "Library.h"
#include "Menu.h"

using std::cout;
using std::endl;

int main() {
    Library library_1("library.db"); 
    displayMenu(library_1);
    
    return 0;
}