include(FeatureSummary)

option(SC_ENABLE_TESTS "Enable testing" ON)
add_feature_info(
    ENABLE_TESTS SC_ENABLE_TESTS "Build project with tests"
)

option(SC_CODE_FORMAT "Enable code format" ON)
add_feature_info(
    ENABLE_CODE_FORMAT SC_CODE_FORMAT "Enable code formatting feature"
)

feature_summary(WHAT ALL)
