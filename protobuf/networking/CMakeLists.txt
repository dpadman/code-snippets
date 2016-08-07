cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Werror")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(PROTO_COMPILER protoc --proto_path=../ --cpp_out=. --python_out=. ../employeedb.proto)
set_source_files_properties(employeedb.pb.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(employeedb_pb2.py PROPERTIES GENERATED TRUE)

add_executable(employeedb_client employeedb_client.cc employeedb.pb.cc)
add_dependencies(employeedb_client PROTO_COMPILER)
target_link_libraries(employeedb_client -lprotobuf)

add_executable(employeedb_server employeedb_server.cc employeedb.pb.cc)
add_dependencies(employeedb_server PROTO_COMPILER)
target_link_libraries(employeedb_server -lprotobuf -lpthread)
