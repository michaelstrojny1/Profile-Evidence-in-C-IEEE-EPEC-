cmake_minimum_required(VERSION 3.16)
project(BayesianEvidence CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(Eigen3 3.3 REQUIRED)
find_package(OpenMP REQUIRED)

# Include directories
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})

# Add subdirectories for modules
add_subdirectory(src/model_dyn)
add_subdirectory(src/obs_model)
add_subdirectory(src/loglik)
add_subdirectory(src/logprior)
add_subdirectory(src/optimizer)
add_subdirectory(src/hessian)
add_subdirectory(src/bridge_sampling)
add_subdirectory(src/cox_reid)
add_subdirectory(src/ais)
add_subdirectory(src/nuclear_compliance)
add_subdirectory(src/audit)
add_subdirectory(src/dmis_proposal)
add_subdirectory(src/importance_weights)
add_subdirectory(src/psis)
add_subdirectory(src/orchestration)

# Main executable
add_executable(bayesian_evidence main.cpp)
target_link_libraries(bayesian_evidence
    model_dyn
    obs_model
    loglik
    logprior
    optimizer
    hessian
    bridge_sampling
    cox_reid
    ais
    nuclear_compliance
    audit
    dmis_proposal
    importance_weights
    psis
    orchestration
    OpenMP::OpenMP_CXX
)
