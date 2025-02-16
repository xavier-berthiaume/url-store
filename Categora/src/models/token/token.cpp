#include "token.h"

Token::Token(const std::string &value) : value(value) {

}

const std::string Token::getValue() const
{
    return value;
}
