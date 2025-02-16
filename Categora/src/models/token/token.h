#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token
{
    const std::string value;

public:
    Token(const std::string &value);

    const std::string getValue() const;

protected:

};

#endif // TOKEN_H
