include(FeatureSummary)

option(SC_ENABLE_TESTS "Enable testing" ON)
add_feature_info(
    ENABLE_TESTS SC_ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
