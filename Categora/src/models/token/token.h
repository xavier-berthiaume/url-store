#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ctime>

class Token
{
    const std::string m_token_string;
    const std::time_t m_creation_date;

public:
    Token(const std::string &token_string, const std::time_t &creation_date);
    Token(const Token& other);

    const std::string getTokenString() const;
    const std::time_t getCreationDate() const;

};

#endif // TOKEN_H
