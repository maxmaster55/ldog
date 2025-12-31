#include <vector>
#include "LogMessage.h"
#include "ILogSink.h"

/* LogManager → a class that for now stores LogMessage objects into a private buffer
 *              and has a method that routes the messages into all sinks.
*/
class LogManager
{
private:
    std::vector<LogMessage> buffer;
    std::vector<ILogSink&> sinks;

public:
    LogManager() = default;
    void add_sink(ILogSink& sink);
    void write_to_all();
    ~LogManager() = default;
};
