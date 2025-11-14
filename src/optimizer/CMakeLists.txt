add_library(optimizer optimizer.cpp)
target_include_directories(optimizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(optimizer loglik logprior Eigen3::Eigen)

add_executable(test_optimizer test_optimizer.cpp)
target_link_libraries(test_optimizer optimizer)
