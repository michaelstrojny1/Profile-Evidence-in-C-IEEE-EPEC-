#ifndef AUDIT_H
#define AUDIT_H

#include <string>
#include <iostream>

class AL {
public:
    static void log(const std::string& message) {
        std::cout << "[AUDIT] " << message << std::endl;
    }
};

#endif // AUDIT_H
