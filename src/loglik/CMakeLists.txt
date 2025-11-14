add_library(loglik loglik.cpp)
target_include_directories(loglik PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(loglik Eigen3::Eigen obs_model)

add_executable(test_loglik test_loglik.cpp)
target_link_libraries(test_loglik loglik)
