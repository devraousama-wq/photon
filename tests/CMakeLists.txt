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

add_executable(photon_materials_test materials_test.cpp)
target_link_libraries(photon_materials_test PRIVATE photon_core GTest::gtest_main)
add_test(NAME photon_materials_test COMMAND photon_materials_test)

add_executable(photon_sampler_test sampler_test.cpp)
target_link_libraries(photon_sampler_test PRIVATE photon_core GTest::gtest_main)
add_test(NAME photon_sampler_test COMMAND photon_sampler_test)

add_executable(photon_lights_test lights_test.cpp)
target_link_libraries(photon_lights_test PRIVATE photon_core GTest::gtest_main)
add_test(NAME photon_lights_test COMMAND photon_lights_test)
