find_package(gRPC REQUIRED)

function(grpc_generate)
    set(noValues "")
    set(singleValues TARGET PROTO_PATH PROTO_FILE OUTPUT_PATH)
    set(multiValues PROTO_HDRS PROTO_SRCS GRPC_HDRS GRPC_SRCS)

    cmake_parse_arguments(
        PARSE_ARGV 0 ARG "${noValues}" "${singleValues}" "${multiValues}"
    )

    find_program(_PROTOBUF_PROTOC_EXE protoc REQUIRED)
    find_program(_GRPC_CPP_PLUGIN_EXE grpc_cpp_plugin REQUIRED)

    add_custom_command(
        OUTPUT ${ARG_PROTO_SRCS}
               ${ARG_PROTO_HDRS}
               ${ARG_GRPC_SRCS}
               ${ARG_GRPC_HDRS}
        COMMAND ${_PROTOBUF_PROTOC_EXE}
        ARGS --grpc_out "${ARG_OUTPUT_PATH}"
             --cpp_out "${ARG_OUTPUT_PATH}"
             --proto_path "${ARG_PROTO_PATH}"
             --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXE}
             "${ARG_PROTO_PATH}/${ARG_PROTO_FILE}"
        DEPENDS "${ARG_PROTO_PATH}/${ARG_PROTO_FILE}"
    )

    add_library(${ARG_TARGET}
        STATIC ${ARG_PROTO_SRCS}
               ${ARG_PROTO_HDRS}
               ${ARG_GRPC_SRCS}
               ${ARG_GRPC_HDRS}
    )

    target_include_directories(${ARG_TARGET}
        PUBLIC ${ARG_OUTPUT_PATH}
    )

    target_link_libraries(${ARG_TARGET}
        PUBLIC gRPC::grpc++_reflection
               gRPC::grpc++
               protobuf::libprotobuf
    )
endfunction()