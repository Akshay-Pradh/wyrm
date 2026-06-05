add_executable(wyrmd-tests
    test_net.cpp
    test_context.cpp
    ${CMAKE_SOURCE_DIR}/wyrmd/src/net.cpp
    ${CMAKE_SOURCE_DIR}/wyrmd/src/context.cpp
)

target_include_directories(wyrmd-tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/wyrmd/include
)

target_link_libraries(wyrmd-tests
    PRIVATE
        wyrm-cpp
        NatNet::NatNet
        zenohcxx::zenohc
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(wyrmd-tests)
