//
// Created by vanessa on 5/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_PQLEVALUATOR_PQLENUMS_H_
#define INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_PQLEVALUATOR_PQLENUMS_H_

// relocate if necessary
namespace spa {

    /**enum TargetType {
        ALL, ASSIGN, IF, WHILE, PROCEDURE, READ, CALL, CONTAINER, PRINT, VARIABLE, CONSTANT
    };*/

    enum FilterType {
        FOLLOWS, FOLLOWS_T, PARENT, PARENT_T, MODIFIES, USES, PATTERN
    };

    enum ParamsType {
        NO_ARG, SINGLE_ARG, DOUBLE_ARG
    };

};

#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_PQLEVALUATOR_PQLENUMS_H_
