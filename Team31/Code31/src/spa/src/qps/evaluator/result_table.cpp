//
// Created by vanessa on 1/2/22.
//

#include <vector>
#include "result_table.h"
#include "pql_enums.h"

using namespace spa;

result_table::result_table(std::vector<std::string> headers) {

    this->table_header = headers;

}

// change this to iterate through the list of connected queries by pairs
void result_table::Merge(std::vector<std::string> result, spa::TargetType target) {

    for (int i = 0; i < this->table_header.size(); i++) {
        if (result.contains(table_header[i])) {  // match result type to table header
            for (int j = 0; j < result.size(); j++) {
                table_contents[i].push_back(result.getResult[j]);
            }
        }
    }
}

std::vector<std::vector<std::string> > result_table::GetTableContents() {
    return this->table_contents;
}
std::vector<std::string> result_table::GetHeaders() {
    return this->table_header;
}




