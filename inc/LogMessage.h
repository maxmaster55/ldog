#include <string>
#include <chrono>


enum class message_sev{

};

class LogMessage
{
    using clock = std::chrono::system_clock;
    using time_point = clock::time_point;
    using string = std::string;

private:
    string app_name;
    string context;
    time_point time;
    message_sev severity;
    string text;

public:
    LogMessage(string app_name, string context, time_point time, message_sev severity, string text);
    ~LogMessage() = default;
};