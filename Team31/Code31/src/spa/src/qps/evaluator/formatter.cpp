//
// Created by vanessa on 1/2/22.
//

#include <vector>
#include "formatter.h"
#include <iostream>

std::string formatter::formatResult(std::vector<std::string> rawResult) {

    std::string formattedResult;

    if (rawResult.empty()) {
        return "";
    }

    formattedResult.push_back('[');
    for (int i = 0; i < rawResult.size(); i++) {
        std::cout << rawResult[i] << ', ';
    }
    formattedResult.push_back(']');

    return formattedResult;
}

// for minimal iteration only
void formatter::project(std::vector<std::string> result) {

    std::cout << '[';
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << ', ';
    }
    std::cout << ']';
}

