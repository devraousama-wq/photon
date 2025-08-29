add_executable(photon_smoke_test smoke_test.cpp)
target_link_libraries(photon_smoke_test PRIVATE photon_core)
add_test(NAME photon_smoke_test COMMAND photon_smoke_test)

add_executable(photon_math_test math_test.cpp)
target_link_libraries(photon_math_test PRIVATE photon_core GTest::gtest_main)
add_test(NAME photon_math_test COMMAND photon_math_test)

add_executable(photon_geometry_test geometry_test.cpp)
target_link_libraries(photon_geometry_test PRIVATE photon_core GTest::gtest_main)
add_test(NAME photon_geometry_test COMMAND photon_geometry_test)

add_executable(photon_bvh_test bvh_test.cpp)
target_link_libraries(photon_bvh_test PRIVATE photon_core GTest::gtest_main)
add_test(NAME photon_bvh_test COMMAND photon_bvh_test)
