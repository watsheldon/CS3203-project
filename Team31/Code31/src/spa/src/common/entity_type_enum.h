#ifndef SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_
#define SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_

enum class QueryEntityType { kProc = 0, kVar, kConst, kStmt };

enum class SetEntityType { kProc, kVar, kConst, kStmt, kStmtLst, kNotation };

enum class StmtType { kAll = 0, kRead, kPrint, kCall, kWhile, kIf, kAssign };

enum class ArgPos { kFirst, kSecond };

#endif  // SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_
