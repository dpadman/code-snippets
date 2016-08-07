cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Werror")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(PROTO_COMPILER protoc --proto_path=../ --cpp_out=. --python_out=. ../employeedb.proto)
set_source_files_properties(employeedb.pb.cc PROPERTIES GENERATED TRUE)
set_source_files_properties(employeedb_pb2.py PROPERTIES GENERATED TRUE)
add_executable(add_employee add_employee.cc employeedb.pb.cc)
add_dependencies(add_employee PROTO_COMPILER)
target_link_libraries(add_employee -lprotobuf)

execute_process(COMMAND touch __init__.py)
execute_process(COMMAND python -m compileall ../)
execute_process(COMMAND mv ../list_employee.pyc ${CMAKE_CURRENT_BINARY_DIR})
