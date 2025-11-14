add_library(hessian hessian.cpp)
target_include_directories(hessian PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(hessian Eigen3::Eigen)
