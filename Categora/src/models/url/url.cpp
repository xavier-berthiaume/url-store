#include "url.h"

Url::Url()
{

}

Url::Url(const std::string &url)
    : url(url)
{

}

std::string Url::getUrl() const
{
    return url;
}

std::vector<std::string> Url::getTags() const
{
    return tags;
}

std::string Url::getNote() const
{
    return note;
}

void Url::setUrl(const std::string &url)
{
    this->url = url;
}

void Url::setTags(const std::vector<std::string> &tags)
{
    this->tags = tags;
}

void Url::setTags(std::initializer_list<std::string> tags)
{
    this->tags = tags;
}

void Url::setNote(const std::string &note)
{
    this->note = note;
}
