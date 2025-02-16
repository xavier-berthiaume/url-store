#include "token.h"

Token::Token(const std::string &token_string, const std::time_t &creation_date)
    : m_token_string(token_string)
    , m_creation_date(creation_date)
{

}

const std::string Token::getTokenString() const
{
    return m_token_string;
}

const std::time_t Token::getCreationDate() const
{
    return m_creation_date;
}
