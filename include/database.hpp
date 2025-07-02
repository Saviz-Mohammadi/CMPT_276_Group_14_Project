#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

class Database
{
public:
    explicit Database();
    ~Database();

public:
    void openConnection(const std::string& path, bool& isSuccessful, std::string& outcomeMessage);
    void cutConnection(bool& isSuccessful, std::string& outcomeMessage);
private:
    sqlite3* m_sqlite3;
};

#endif // DATABASE_H
