#ifndef NUCLEAR_COMPLIANCE_H
#define NUCLEAR_COMPLIANCE_H

#include <string>
#include <vector>

struct ComplianceResult {
    double score;
    bool compliant;
    std::vector<std::string> issues;
};

class NC {
public:
    static ComplianceResult verify_full_compliance();
};

#endif // NUCLEAR_COMPLIANCE_H
