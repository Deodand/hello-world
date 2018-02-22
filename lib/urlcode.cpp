#include <iostream>

#include "urlcode.h"


namespace url {
    std::string findInContainer(char charSymbol) {
        for(auto it = urlSymbolsContainer.begin(); it != urlSymbolsContainer.end(); ++it) {
            if(it->first == charSymbol) {
                return it->second;
            }
        }
        return "";
    }

    char findInContainer(std::string stringSymbol) {
        for(auto it = urlSymbolsContainer.begin(); it != urlSymbolsContainer.end(); ++it) {
            if(it->second == stringSymbol) {
                return it->first;
            }
        }
        return ' ';
    }

    std::string urlencode(std::string decodingString) {
        std::string finalString;
        for(int i=0; i != decodingString.length(); ++i) {
            std::string temp;
            if((temp = findInContainer(decodingString[i])) != "") {
                finalString += temp;
            }
            else {
                finalString += std::string(1, decodingString[i]);
            }
        }
        return finalString;
    }

    std::string urldecode(std::string encodingString) {
        std::string finalString;
        for(int i=0; i != encodingString.length(); ++i) {
            if(encodingString[i] == '%') {
                finalString += findInContainer(encodingString.substr(i, 3));
                i += 2;
            }
            else {
                finalString += encodingString[i];
            }
        }
        return finalString;
    }
}