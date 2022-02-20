file(GLOB_RECURSE srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/**.cpp")
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/**.h" "src/**.hpp")
add_executable(unit_testing ${srcs})

target_link_libraries(unit_testing spa)