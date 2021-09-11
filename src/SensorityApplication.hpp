/**
 * @file SensorityApplication.hpp
 *
 * @brief Service application declaration
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#pragma once

#include <Poco/Util/ServerApplication.h>

class SensorityApplication final : public Poco::Util::ServerApplication {
public:
    SensorityApplication();

private:
    void
    initialize(Poco::Util::Application& self) override;

    void
    defineOptions(Poco::Util::OptionSet& options) override;

    int
    main(const std::vector<std::string>& args) override;

private:
    void
    handleHelp(const std::string& name, const std::string& value);

    void
    displayHelp();

private:
    bool _helpRequested;
};