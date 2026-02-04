#include <v1/ldog/gpu/GpuUsageDataStubDefault.hpp>
#include <CommonAPI/CommonAPI.hpp>
#include <random>
#include <string>
#include <thread>

using namespace std::chrono_literals;

constexpr std::string_view DOMAIN = "local";
constexpr std::string_view INSTANCE = "ldog.gpu.GpuUsageData";

class service_test : public v1::ldog::gpu::GpuUsageDataStubDefault
{
private:
    float randomFloat(float min = 0.0f, float max = 100.0f)
    {
        static std::random_device rd;  // Seed
        static std::mt19937 gen(rd()); // Mersenne Twister RNG
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    float getGpuUsage()
    {
        return randomFloat();
    }

public:
    void requestGpuUsageData(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        requestGpuUsageDataReply_t _reply) override
    {
        float usage = getGpuUsage(); // your logic
        std::cout << "Client requested GPU usage: " << usage << "%" << std::endl;
        _reply(usage); // send response
    }

    void broadcastGpuUsageChange()
    {
        float usage = getGpuUsage();
        std::cout << "Broadcasting GPU usage: " << usage << "%" << std::endl;
        fireNotifyGpuUsageDataChangeEvent(usage);
    }
};

int main(int argc, char const *argv[])
{
    auto runtime = CommonAPI::Runtime::get();
    std::shared_ptr<service_test> myService =
        std::make_shared<service_test>();

    runtime->registerService(std::string(DOMAIN), std::string(INSTANCE), myService);

    while (true)
    {
        std::this_thread::sleep_for(100ms);
        myService->broadcastGpuUsageChange();
        std::cout << "[Server] Still running..." << std::endl;
    }
    return 0;
}
