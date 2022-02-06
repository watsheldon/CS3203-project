file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} ../spa/src/PQLEvaluator/Evaluator.cpp ../spa/src/PQLEvaluator/Evaluator.h ../spa/src/PQLEvaluator/Retriever.cpp ../spa/src/PQLEvaluator/Retriever.h ../spa/src/PQLEvaluator/Formatter.cpp ../spa/src/PQLEvaluator/Formatter.h ../spa/src/PQLEvaluator/ResultTable.cpp ../spa/src/PQLEvaluator/ResultTable.h ../spa/src/PQLEvaluator/DependencyGraph.cpp ../spa/src/PQLEvaluator/DependencyGraph.h ../spa/src/PQLEvaluator/PQLEnums.h)

target_link_libraries(unit_testing spa)

