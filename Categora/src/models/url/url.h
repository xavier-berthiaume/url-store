#ifndef URL_H
#define URL_H

#include <vector>
#include <string>

class Url
{

    std::string url;
    std::vector<std::string> tags;
    std::string note;

public:
    Url();
    Url(const std::string &url);

    std::string getUrl() const;
    std::vector<std::string> getTags() const;
    std::string getNote() const;

    // When setting a new url, make sure to fetch new tags for the url
    void setUrl(const std::string &url);
    void setTags(const std::vector<std::string> &tags);
    void setTags(std::initializer_list<std::string> tags);
    void setNote(const std::string &note);
};

#endif // URL_H
