add_library(obs_model obs_model.cpp)
target_include_directories(obs_model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(obs_model Eigen3::Eigen model_dyn)

add_executable(test_obs_model test_obs_model.cpp)
target_link_libraries(test_obs_model obs_model)
