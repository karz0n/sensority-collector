#pragma once

#include <Poco/Util/Application.h>

class TestApplication : public Poco::Util::Application {
public:
    TestApplication() = default;

private:
    void
    initialize(Poco::Util::Application& self) override;

    int
    main(const std::vector<std::string>& args) override;
};