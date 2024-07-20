#include <iostream>
#include "Kind.h"
#include "Book.h"
#include "BorrowedBook.h"
#include "Library.h"

using std::cout;
using std::endl;

int main() {
    Kind crime(Kind::KindType::CRIME);
    Kind horror(Kind::KindType::HORROR);
    Kind fantasy(Kind::KindType::FANTASY);
    Kind fairy_tale(Kind::KindType::FAIRY_TALE);
    Kind reportage(Kind::KindType::REPORTAGE);
    Kind contemporary_literature(Kind::KindType::CONTEMPORARY_LITERATURE);
    Kind romance(Kind::KindType::ROMANCE);
    Kind scientific_literature(Kind::KindType::SCIENTIFIC_LITERATURE);
    Kind thriller(Kind::KindType::THRILLER);
    Kind poetry(Kind::KindType::POETRY);
    Kind science_fiction(Kind::KindType::SCI_FI);

    Library library_1("library.db");

    Book book1(1, "Crime and Punishment", "Fyodor Dostoevsky", crime, 1866, 430, 5);
    Book book2(2, "Dracula", "Bram Stoker", horror, 1897, 418, 3);
    Book book3(3, "The Hobbit", "J.R.R. Tolkien", fantasy, 1937, 310, 8);

    library_1.addBook(book1);
    library_1.addBook(book2);
    library_1.addBook(book3); 

    BorrowedBook borrowed_1(1, book1, "2024-07-03", false); 
    library_1.borrowBook(borrowed_1); 

    library_1.displayAllBooks();
    cout << endl;
    library_1.displayBorrowedBooks();
    cout << endl;
    library_1.displayBooksByLength();
    
    return 0;
}