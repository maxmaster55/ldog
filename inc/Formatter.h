#pragma once

#include <optional>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <LogMessage.h>
#include <log_types.h>


template<typename Policy_t>
constexpr bool is_valid_policy()
{
    return std::is_same_v<decltype(Policy_t::unit), const std::string_view> &&
        std::is_same_v<decltype(Policy_t::context), const TelemetrySrc>;
};


template <typename Policy_t>
class Formatter
{
    using string = std::string;
    static_assert(is_valid_policy<Policy_t>(), "Not a valid policy");
    
public:
    std::optional<LogMessage> formatDataToLogMsg(const std::string& raw)
    {
        try {
            float value = std::stof(raw);
            string name = (Policy_t::context == TelemetrySrc::CPU) ? "cpu app" :
                        (Policy_t::context == TelemetrySrc::GPU) ? "Gpu app" : "Ram app";

            LogMessage msg = LogMessage(
                name,
                std::string(magic_enum::enum_name(Policy_t::context)),
                currentTimeStamp(),
                Policy_t::inferSeverity(value),
                msgDescription(value)
            );

            return msg;
        }
        catch (std::exception e) {
            return std::nullopt;
        }
            
    }

    std::string msgDescription(float val)
    {
        string str_val = std::to_string(val);
        string str_unit(Policy_t::unit);

        switch (Policy_t::inferSeverity(val))
        {
        case SeverityLvl::CRITICAL :
            return "Critical value at " + str_val + str_unit + ", too late lil bro." ;
        case SeverityLvl::WARNING :
            return "Warning value is  " + str_val + str_unit + ", you can still do smth." ;
        case SeverityLvl::INFO :
            return "Normal value at " + str_val + str_unit + ", nothing to do." ;
        }
        throw std::invalid_argument("not a valid float");
    }    
    std::string currentTimeStamp()
    {
        auto n = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(n);
        std::tm tm = *std::localtime(&t); // convert to local time
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    ~Formatter() = default;
};

