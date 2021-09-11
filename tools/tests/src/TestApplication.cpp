#include "TestApplication.hpp"

#include <Poco/Util/LoggingSubsystem.h>

#include <gtest/gtest.h>

using Poco::Util::Application;
using Poco::Util::LoggingSubsystem;

void
TestApplication::initialize(Application& self)
{
    loadConfiguration();

    addSubsystem(new LoggingSubsystem);

    Application::initialize(self);
}

int
TestApplication::main(const std::vector<std::string>& args)
{
    return RUN_ALL_TESTS();
}

int
main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    try {
        TestApplication app;
        app.init(argc, argv);
        return app.run();
    } catch (const Poco::Exception& e) {
        std::cerr << e.displayText() << std::endl;
        return Poco::Util::Application::EXIT_SOFTWARE;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return Poco::Util::Application::EXIT_SOFTWARE;
    } catch (...) {
        std::cerr << "Unhandled exception" << std::endl;
        return Poco::Util::Application::EXIT_SOFTWARE;
    }
}