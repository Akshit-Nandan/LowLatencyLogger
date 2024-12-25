#pragma once

#include<iostream>

#define LIKELY(x) __builtin_expect(!!(x),1)
#define UNLIKELY(x) __builtin_expect(!!(x),0)

inline auto ASSERT(bool cond,const std::string &msg) noexcept {
    if(UNLIKELY(!cond)){
        std::cerr<<"ASSERT : "<<msg<<"\n";
        exit(EXIT_FAILURE);
    }
}

inline auto FATAL(const std::string& msg) noexcept {
    std::cerr<<"FATAL : "<<msg<<"\n";
    exit(EXIT_FAILURE);
}
