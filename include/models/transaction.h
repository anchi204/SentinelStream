#pragma once

#include <cstdint>
#include <string>

namespace sentinel {

struct Transaction {
    std::string transaction_id;
    std::string user_id;
    double amount{};
    std::int64_t timestamp{};
    std::string merchant_id;
};

} // namespace sentinel
