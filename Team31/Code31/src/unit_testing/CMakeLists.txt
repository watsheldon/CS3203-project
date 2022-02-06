file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} ../spa/src/qps/evaluator/evaluator.cpp ../spa/src/qps/evaluator/evaluator.h ../spa/src/qps/evaluator/retriever.cpp ../spa/src/qps/evaluator/retriever.h ../spa/src/qps/evaluator/formatter.cpp ../spa/src/qps/evaluator/formatter.h ../spa/src/qps/evaluator/result_table.cpp ../spa/src/qps/evaluator/result_table.h ../spa/src/qps/evaluator/dependency_graph.cpp ../spa/src/qps/evaluator/dependency_graph.h ../spa/src/qps/evaluator/pql_enums.h)

target_link_libraries(unit_testing spa)

