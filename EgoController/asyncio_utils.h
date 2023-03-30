#pragma once

#include <string>
#include <algorithm>
#include <iostream>

 
template <typename... Args>
std::string format(const std::string &fmt, Args... args)
{
    // C++11 specify that string store elements continously
    std::string ret;

    auto sz = std::snprintf(nullptr, 0, fmt.c_str(), args...);
    ret.reserve(sz + 1);
    ret.resize(sz); // to be sure there have room for \0
    std::snprintf(&ret.front(), ret.capacity() + 1, fmt.c_str(), args...);
    return ret;
}

template <typename... Args>
bool set_this_thread_name(const std::string &name, Args &&... args)
{
    auto new_name = format(name, std::forward<Args>(args)...);

#ifdef __APPLE__
    return pthread_setname_np(new_name.c_str()) == 0;
#else
    pthread_t pth = pthread_self();
    return pthread_setname_np(pth, new_name.c_str()) == 0;
#endif
}

/**
 * Parse host:port pairs
 */
void url_parse_host(std::string host,
                    std::string &host_out, int &port_out,
                    const std::string def_host, const int def_port);

/**
 * Parse ?ids=sid,cid
 */
void url_parse_query(std::string query);
 