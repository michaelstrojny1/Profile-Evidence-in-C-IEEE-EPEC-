add_library(logprior logprior.cpp)
target_include_directories(logprior PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(logprior Eigen3::Eigen)

add_executable(test_logprior test_logprior.cpp)
target_link_libraries(test_logprior logprior)
