add_library(orchestration orchestration.cpp)
target_include_directories(orchestration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(orchestration optimizer hessian bridge_sampling dmis_proposal importance_weights psis Eigen3::Eigen)

add_executable(test_orchestration test_orchestration.cpp)
target_link_libraries(test_orchestration orchestration)
