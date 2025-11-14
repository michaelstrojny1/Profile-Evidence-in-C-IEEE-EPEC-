add_library(model_dyn model_dyn.cpp)
target_include_directories(model_dyn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(model_dyn Eigen3::Eigen)

add_executable(test_model_dyn test_model_dyn.cpp)
target_link_libraries(test_model_dyn model_dyn)
