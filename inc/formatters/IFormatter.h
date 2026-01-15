#pragma once

#include <optional>
#include <LogMessage.h>
#include <log_types.h>

template<typename Policy_t>
constexpr bool is_valid_policy()
{
    return std::is_same_v<decltype(Policy_t::unit), const std::string_view> &&
        std::is_same_v<decltype(Policy_t::context), const TelemetrySrc>;
};


template <typename Policy_t>
class IFormatter
{
    using string = std::string;
    static_assert(is_valid_policy<Policy_t>(), "Not a valid policy");
    
public:
    virtual std::optional<LogMessage> formatDataToLogMsg(const string& raw) = 0;
    virtual string msgDescription(float val) = 0;
    virtual string currentTimeStamp() = 0;
    virtual ~IFormatter() = default;
};

