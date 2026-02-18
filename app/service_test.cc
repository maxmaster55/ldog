#include <v1/ldog/gpu/GpuUsageDataStubDefault.hpp>
#include <CommonAPI/CommonAPI.hpp>
#include <random>
#include <string>
#include <fstream>
#include <thread>

using namespace std::chrono_literals;

constexpr std::string_view DOMAIN = "local";
constexpr std::string_view INSTANCE = "ldog.gpu.GpuUsageData";
std::string filePath;

class service_test : public v1::ldog::gpu::GpuUsageDataStubDefault
{
private:
    std::ifstream fileStream_;
    int int_from_file()
    {
        if (!fileStream_.is_open())
        {
            std::cerr << "Failed to open file." << std::endl;
            return 0;
        }

        int value = 0;

        fileStream_.clear();  // clear EOF flag
        fileStream_.seekg(0); // rewind file

        if (!(fileStream_ >> value))
        {
            std::cerr << "Failed to read value." << std::endl;
            return 0;
        }

        return value;
    }

    int getReading()
    {
        return int_from_file();
    }

public:
    void requestGpuUsageData(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        requestGpuUsageDataReply_t _reply) override
    {
        int reading = getReading();
        std::cout << "Client requested reading: " << reading << std::endl;
        _reply(reading); // send response
    }

    void broadcastGpureadingChange()
    {
        fileStream_.open(filePath);
        float reading = getReading();
        std::cout << "Broadcasting reading: " << reading << std::endl;
        fireNotifyGpuUsageDataChangeEvent(reading);
    }
};

int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        filePath = "/sys/bus/iio/devices/iio:device0/in_temp_input";
    }
    else
    {
        filePath = argv[1];
    }

    std::cout << "File path: " << filePath << std::endl;

    auto runtime = CommonAPI::Runtime::get();
    std::shared_ptr<service_test> myService =
        std::make_shared<service_test>();

    bool succ = runtime->registerService(std::string(DOMAIN), std::string(INSTANCE), myService);

    if (!succ)
    {
        std::cerr << "Failed to register service." << std::endl;
        return 1;
    }

    while (true)
    {
        std::this_thread::sleep_for(100ms);
        myService->broadcastGpureadingChange();
        std::cout << "[Server] Still running..." << std::endl;
    }
    return 0;
}
