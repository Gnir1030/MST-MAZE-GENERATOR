#include "util.h"

std::string exec::execname_;
int exec::status_ {EXIT_SUCCESS};

void exec::execname (const std::string &argv0) {
    execname_ = argv0;
}

void exec::status (int status) {
    if (status > status_)
        status_ = status;
}

std::string const& exec::execname() {
    return execname_;
}

int exec::status() {
    return status_;
}
