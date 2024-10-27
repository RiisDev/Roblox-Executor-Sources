#pragma once

#include <optional>
#include <string>
#include <vector>
#include <string>
#include <map>

namespace http
{
    extern std::map<int, std::string> status_message;
    extern std::vector<std::string> last_headers;
    extern long http_code;

    struct parameter
    {
        std::string key;
        std::string value;
    };

    std::optional<std::string> get(const std::string&, const std::vector<parameter> & = {});
    std::optional<std::string> post(const std::string&, const std::vector<parameter> & = {});
    std::optional<std::string> send_request(const std::string&, std::string method, std::vector<std::string>&, const std::string&);
}