#pragma once

#include <string>

class exec {
private:
    static std::string execname_;
    static int status_;
    static void execname (const std::string& argv0);
    friend int main(int,char**);
public:
    static void status (int status);
    static const std::string& execname();
    static int status();
};
