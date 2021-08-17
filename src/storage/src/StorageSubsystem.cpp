/**
 * @file StorageSubsystem.cpp
 *
 * @brief Storage subsystem definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "storage/StorageSubsystem.hpp"

using Poco::Util::Application;

namespace storage {

const char* StorageSubsystem::name() const
{
    return "StorageSubsystem";
}

void StorageSubsystem::initialize(Application& app)
{
}

void StorageSubsystem::uninitialize()
{
}

}
