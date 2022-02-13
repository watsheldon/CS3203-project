//
// Created by Yichen Gu on 13/2/22.
//

#ifndef SPA_SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_
#define SPA_SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_

enum class EntityType { kProc, kVar, kConst, kStmt, kStmtLst, kNotation };

enum class StmtType { kRead = 0, kPrint, kCall, kWhile, kIf, kAssign };

#endif  // SPA_SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_
