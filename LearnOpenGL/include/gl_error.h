#pragma once

#include <exception>
#include <string>

class gl_error : public std::exception
{
public:
    explicit gl_error(const char* message)
        : msg_(message) {}
    explicit gl_error(std::string& message)
        : msg_(message) {}
    explicit gl_error(std::string message)
        : msg_(message) {}

    virtual ~gl_error() noexcept {}

    virtual const char* what() const noexcept { return msg_.c_str(); }
protected:
    std::string msg_;
};

