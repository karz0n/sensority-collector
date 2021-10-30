#pragma once

namespace topic {

inline constexpr auto Topic1 = "temperature";
inline constexpr auto Topic2 = "pressure";
inline constexpr auto Topic3 = "gas";
inline constexpr auto Topic4 = "humidity";
inline constexpr auto Topic5 = "iaq";
inline constexpr auto Topic6 = "eCO2";
inline constexpr auto Topic7 = "eVOC";
inline constexpr auto Topic8 = "TVOC";

} // namespace topic

namespace database {

inline constexpr auto Name = "scollector.db";

}

namespace property {

inline constexpr auto ServerUsername = "server.username";
inline constexpr auto ServerPassword = "server.password";
inline constexpr auto ServerHost = "server.host";
inline constexpr auto ServerPort = "server.port";
inline constexpr auto ServerSecure = "server.secure";

inline constexpr auto DatabasePath = "database.path";

inline constexpr auto DataTopicsTopic1 = "data.topics.topic1";
inline constexpr auto DataTopicsTopic2 = "data.topics.topic2";
inline constexpr auto DataTopicsTopic3 = "data.topics.topic3";
inline constexpr auto DataTopicsTopic4 = "data.topics.topic4";
inline constexpr auto DataTopicsTopic5 = "data.topics.topic5";
inline constexpr auto DataTopicsTopic6 = "data.topics.topic6";
inline constexpr auto DataTopicsTopic7 = "data.topics.topic7";
inline constexpr auto DataTopicsTopic8 = "data.topics.topic8";

} // namespace property