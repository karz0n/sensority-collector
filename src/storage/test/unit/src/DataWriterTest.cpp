#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Poco/JSON/Parser.h>

#include "storage/DataWriter.hpp"

#include <iostream>

using namespace testing;
using namespace storage;

using Poco::JSON::Parser;
using Poco::JSON::Object;

class DataWriterTest : public Test {
public:
    Parser parser;
};

TEST_F(DataWriterTest, ParseTemperatureData)
{
    const std::string s{R"({"value": 13.5, "raw": 14.7})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        TemperatureDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParseHumidityData)
{
    const std::string s{R"({"value": 13.5, "raw": 14.7})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        HumidityDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParsePressureData)
{
    const std::string s{R"({"value": 11.5})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        PressureDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParseGasData)
{
    const std::string s{R"({"value": 1300.0})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        GasDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParseIaqData)
{
    const std::string s{R"({"value": 12.5, "raw": 14.7, "accuracy": 0.1})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        IaqDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParseCo2Data)
{
    const std::string s{R"({"value": 15.5, "equivalent": 17.7, "accuracy": 0.1})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        Co2DataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParseEvocData)
{
    const std::string s{R"({"value": 25.5, "equivalent": 25.1, "accuracy": 0.1})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        EvocDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}

TEST_F(DataWriterTest, ParseTvocData)
{
    const std::string s{R"({"value": 1113.5})"};

    const auto var = parser.parse(s);
    ASSERT_FALSE(var.isEmpty());

    EXPECT_NO_THROW({
        TvocDataWriter writer;
        writer.process(var.extract<Object::Ptr>());
    });
}