/**
 * @file ConnectivitySubsystem.cpp
 *
 * @brief Connectivity subsystem definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "connectivity/ConnectivitySubsystem.hpp"

using Poco::Util::Application;

namespace connectivity {

const char*
ConnectivitySubsystem::name() const
{
    return "ConnectivitySubsystem";
}

void
ConnectivitySubsystem::initialize(Application&)
{
}

void
ConnectivitySubsystem::uninitialize()
{
}

} // namespace connectivity