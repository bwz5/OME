include(FetchContent)
FetchContent_Declare(
  httplib 
  URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.53.1.zip
)
FetchContent_MakeAvailable(httplib)

# Create the ome_server executable 
add_executable(ome_server)
target_sources(ome_server PRIVATE server.cpp) 
target_link_libraries(ome_server PRIVATE ome_core httplib::httplib) 

FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)


# Create the ome_server executable 
add_executable(ome_server)
target_sources(ome_server PRIVATE server.cpp) 
target_link_libraries(ome_server PRIVATE ome_core httplib::httplib nlohmann_json::nlohmann_json) 
