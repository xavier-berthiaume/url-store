#ifndef URL_H
#define URL_H

#include <vector>
#include <string>

class Url
{

    std::string url;
    std::vector<std::string> tags;
    std::string notes;

    // Function that sends the url to the LLM API,
    // then sets the tags based on the response.
    void fetchTags();

public:
    Url(const std::string &url);

    std::string getUrl() const;
    std::vector<std::string> getTags() const;

    // When setting a new url, make sure to fetch new tags for the url
    void setUrl(const std::string &url);
};

#endif // URL_H
