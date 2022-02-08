//
// Created by vanessa on 1/2/22.
//

#include <vector>
#include "formatter.h"
#include <iostream>

using namespace spa;

// change this
std::string formatter::FormatResult(std::vector<std::string> raw_result) {

    std::string formattedResult;

    return formattedResult;
}

// MINIMAL ITERATION ONLY
void formatter::Project(std::pair<std::vector<int>, std::vector<std::string> > result) {

    if (!result.first.empty()) {
        for (int i: result.first) {
            std::cout << i << ' ';
        }
    } else {
        for (std::string i: result.second) {
            std::cout << i << ' ';
        }
    }
};


