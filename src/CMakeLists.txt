# -------------------------------------------------------------------------------

set(TARGET_NAME ${PROJECT_NAME}-app)

# -------------------------------------------------------------------------------
# Target Configuration
# -------------------------------------------------------------------------------

add_executable(${TARGET_NAME} "")

target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
