#include "nuclear_compliance.hpp"
#include <fstream>
#include <string>

ComplianceResult NC::verify_full_compliance() {
    ComplianceResult result;
    result.score = 0.0;
    result.compliant = false;

    bool psis_implemented = false;
    std::ifstream psis_file("../src/psis/psis.cpp");
    if (psis_file) {
        std::string content((std::istreambuf_iterator<char>(psis_file)), std::istreambuf_iterator<char>());
        if (content.find("k_hat") != std::string::npos && content.find("psis") != std::string::npos) {
            psis_implemented = true;
        }
    }
    if (psis_implemented) result.score += 40.0;
    else result.issues.push_back("PSIS implementation missing");

    bool audit_present = false;
    std::ifstream audit_file("../src/audit/audit.hpp");
    if (audit_file) {
        std::string content((std::istreambuf_iterator<char>(audit_file)), std::istreambuf_iterator<char>());
        if (content.find("AL") != std::string::npos) {
            audit_present = true;
        }
    }
    if (audit_present) result.score += 30.0;
    else result.issues.push_back("Audit logging not implemented");

    bool is_present = false;
    std::ifstream is_file("../src/importance_weights/importance_weights.cpp");
    if (is_file) {
        std::string content((std::istreambuf_iterator<char>(is_file)), std::istreambuf_iterator<char>());
        if (content.find("IW") != std::string::npos) {
            is_present = true;
        }
    }
    if (is_present) result.score += 20.0;
    else result.issues.push_back("Importance sampling not implemented");

    std::ifstream comp_file("../src/nuclear_compliance/nuclear_compliance.hpp");
    if (comp_file) result.score += 10.0;
    else result.issues.push_back("Nuclear compliance module missing");

    result.compliant = (result.score >= 95.0);

    return result;
}
