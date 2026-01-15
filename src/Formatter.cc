#include <Formatter.h>


template<typename Policy_t>
std::optional<LogMessage>
Formatter<Policy_t>::formatDataToLogMsg(const std::string& raw)
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

template<typename Policy_t>
std::string
Formatter<Policy_t>::msgDescription(float val)
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


template<typename Policy_t>
std::string
Formatter<Policy_t>::currentTimeStamp()
{
    auto n = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(n);
    std::tm tm = *std::localtime(&t); // convert to local time
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

