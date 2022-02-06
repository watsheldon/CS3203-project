//
// Created by vanessa on 1/2/22.
//

#include <vector>
#include "ResultTable.h"
#include "PQLEnums.h"

ResultTable::ResultTable(std::vector<std::string> headers) {

    this->tableHeader = headers;

}

void ResultTable::merge(std::string result, PQLEnums::TargetType target) {

    for (int i = 0; i < this->tableHeader.size(); i++) {
        if (result.contains(tableHeader[i])) {  // if result type exists in table header
            for (int j = 0; j < result.size(); j++) {
                tableContents[i].push_back(result.getResult[j]);
            }
        }
    }
}

std::vector<std::vector<std::string> > ResultTable::getTableContents() {
    return this->tableContents;
}




