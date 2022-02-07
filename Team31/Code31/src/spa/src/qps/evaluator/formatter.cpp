//
// Created by vanessa on 1/2/22.
//

#include <vector>
#include "formatter.h"
#include <iostream>

using namespace spa;

std::string formatter::FormatResult(std::vector<std::string> raw_result) {

    std::string formattedResult;

    if (raw_result.empty()) {
        return "";
    }

    formattedResult.push_back('[');
    for (int i = 0; i < raw_result.size(); i++) {
        std::cout << raw_result[i] << ', ';
    }
    formattedResult.push_back(']');

    return formattedResult;
}

// for minimal iteration only
void formatter::Project(std::vector<std::string> result) {

    std::cout << '[';
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << ', ';
    }
    std::cout << ']';
};


