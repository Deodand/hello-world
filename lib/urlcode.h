#ifndef URLCODE_H
#define URLCODE_H

#include <string>
#include <map>



namespace url {
    std::string findInContainer(char);
    std::string urlencode(std::string);
    std::string urldecode(std::string);
    const std::map<char, std::string> urlSymbolsContainer = {
        { '!', "%21"},
        { '"', "%22"},
        { '#', "%23"},
        { '$', "%24"},
        { '%', "%25"},
        { '&', "%26"},
        { '\'', "%27"},
        { '*', "%2A"},
        { ',', "%2C"},
        { ':', "%3A"},
        { ';', "%3B"},
        { '<', "%3C"},
        { '=', "%3D"},
        { '>', "%3E"},
        { '?', "%3F"},
        { '[', "%5B"},
        { ']', "%5D"},
        { '^', "%5E"},
        { '`', "%60"},
        { '{', "%7B"},
        { '|', "%7C"},
        { '}', "%7D"},
        { ' ', "%20"}
    };
}

#endif