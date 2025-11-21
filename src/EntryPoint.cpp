#include "../include/Application/Application.hpp"

using namespace lux;
#ifdef ENABLE_TESTING
#include <gtest/gtest.h>
#endif

int main(int argc, char** argv)
{
    Log::Init();

#if defined(ENABLE_TESTING) && defined(RUN_TESTS)
    std::cout << "Running tests..." << std::endl;

    testing::InitGoogleTest(&argc, argv);

    int testResult = RUN_ALL_TESTS();

    if (testResult != 0)
    {
        std::cerr << "All tests failed!" << std::endl;
        return testResult;
    }

    std::cout << "All tests passed!" << std::endl;
#endif

    Setup();
    Application app;
    app.Run();

    return 0;
}