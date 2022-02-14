#ifndef SPA_SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_
#define SPA_SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_

enum class QueryEntityType { kProc, kVar, kConst, kStmt };

enum class SetEntityType { kProc, kVar, kConst, kStmt, kStmtLst, kNotation };

enum class StmtType { kRead = 0, kPrint, kCall, kWhile, kIf, kAssign };

enum class GetPos { kLeft, kRight };

#endif  // SPA_SRC_SPA_SRC_COMMON_ENTITY_TYPE_ENUM_H_
