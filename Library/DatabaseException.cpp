#include "DatabaseException.h"

using std::runtime_error;
using std::string;

DatabaseException::DatabaseException(const string& message) : runtime_error(message) {}