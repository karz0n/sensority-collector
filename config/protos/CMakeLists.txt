
grpc_generate(
    TARGET DataAccessServiceLib
    PROTO_PATH "${PROJECT_SOURCE_DIR}/config/protos"
    PROTO_FILE "DataAccessService.proto"
    PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/DataAccessService.pb.h"
    PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/DataAccessService.pb.cc"
    GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/DataAccessService.grpc.pb.h"
    GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/DataAccessService.grpc.pb.cc"
    OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}"
)
add_library("SensorityCollector::gRPC::DataAccessService" ALIAS DataAccessServiceLib)
