#include <iostream>

#include "urlcode.h"


namespace url {
    char findStringInMap(const std::string &stringSymbol) {
        for(auto it = urlSymbolsContainer.cbegin(); it != urlSymbolsContainer.cend(); ++it) {
            if(it->second == stringSymbol) {
                return it->first;
            }
        }
        return ' ';
    }

    std::string urlencode(const std::string &decodingString) {
        std::string finalString;
        for(int i=0; i != decodingString.length(); ++i) {
            auto temp = urlSymbolsContainer.find(decodingString[i]);
            if(temp != urlSymbolsContainer.cend()) {
                finalString += temp->second;
            }
            else {
                finalString.push_back(decodingString[i]);
            }
        }
        return finalString;
    }

    std::string urldecode(const std::string &encodingString) {
        std::string finalString;
        for(int i=0; i != encodingString.length(); ++i) {
            if(encodingString[i] == '%') {
                finalString.push_back(findStringInMap(encodingString.substr(i, 3)));
                i += 2;
            }
            else {
                finalString += encodingString[i];
            }
        }
        return finalString;
    }
}