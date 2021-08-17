/**
 * @file SensorityApplication.cpp
 *
 * @brief Service application definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "SensorityApplication.hpp"

#include <Poco/Util/HelpFormatter.h>

#include "storage/StorageSubsystem.hpp"
#include "connectivity/ConnectivitySubsystem.hpp"

using Poco::Util::Application;
using Poco::Util::OptionSet;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

using connectivity::ConnectivitySubsystem;
using storage::StorageSubsystem;

SensorityApplication::SensorityApplication()
    : _helpRequested(false)
{
}

void
SensorityApplication::initialize(Application& self)
{
    loadConfiguration();

    addSubsystem(new StorageSubsystem);
    addSubsystem(new ConnectivitySubsystem);

    ServerApplication::initialize(self);
}

void
SensorityApplication::defineOptions(OptionSet& options)
{
    ServerApplication::defineOptions(options);

    // clang-format off
    options.addOption(
        Option("help", "h", "Display argument help information")
        .required(false)
        .repeatable(false)
        .callback(
            OptionCallback<SensorityApplication>(this, &SensorityApplication::handleHelp)));
    // clang-format on
}

void
SensorityApplication::handleHelp(const std::string&, const std::string&)
{
    _helpRequested = true;

    displayHelp();
    stopOptionsProcessing();
}

int
SensorityApplication::main(const std::vector<std::string>&)
{
    if (_helpRequested) {
        return ServerApplication::EXIT_OK;
    }

    waitForTerminationRequest();

    return Application::EXIT_OK;
}

void
SensorityApplication::displayHelp()
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Sensority Collector Service");
    helpFormatter.format(std::cout);
}

POCO_SERVER_MAIN(SensorityApplication)