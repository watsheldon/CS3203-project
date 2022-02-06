//
// Created by vanessa on 1/2/22.
//

#include <vector>
#include "Formatter.h"
#include <iostream>
#include "PQLEnums.h"

std::string Formatter::formatResult(std::vector<std::vector<std::string> > rt, PQLEnums::TargetType targetType) {

    std::string result;
    //match column header
    //retur values
    return result;
}

// for minimal iteration only
void Formatter::project(std::vector<std::string> result) {

    std::cout << '[';
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << ', ';
    }
    std::cout << ']';
}

