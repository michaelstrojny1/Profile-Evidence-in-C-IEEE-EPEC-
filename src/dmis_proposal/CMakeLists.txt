add_library(dmis_proposal dmis_proposal.cpp)
target_include_directories(dmis_proposal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(dmis_proposal Eigen3::Eigen)

add_executable(test_dmis_psis test_dmis_psis.cpp)
target_link_libraries(test_dmis_psis dmis_proposal importance_weights psis obs_model loglik logprior)
