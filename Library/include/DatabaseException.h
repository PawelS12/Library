#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <stdexcept>
#include <string>

using std::runtime_error;
using std::string;

class DatabaseException : public runtime_error {
public:
    explicit DatabaseException(const string& message);
};

#endif // DATABASEEXCEPTION_H