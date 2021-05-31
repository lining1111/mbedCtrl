#test_server
add_executable(test_server test_server.cpp)
target_link_libraries(test_server ${LINK_LIBS})
#test_client
add_executable(test_client test_client.cpp)
target_link_libraries(test_client ${LINK_LIBS})
#test_database
add_executable(test_database test_database.cpp)
target_link_libraries(test_database ${LINK_LIBS})
#test_jsoncpp
add_executable(test_jsoncpp test_jsoncpp.cpp)
target_link_libraries(test_jsoncpp ${LINK_LIBS})
#test_ping
add_executable(test_ping test_ping.cpp
        ../src/ping/ping.cpp)
target_link_libraries(test_ping ${LINK_LIBS})
#test_ota
add_executable(test_ota test_ota.cpp)
target_link_libraries(test_ota ${LINK_LIBS})
#test_mbedCtrl
add_executable(test_mbedCtrl test_mbedCtrl.cpp
        ../src/MbedCtrl.cpp
        ../src/dev_chargingPile/ChargingPile.cpp)
target_link_libraries(test_mbedCtrl ${LINK_LIBS})

add_executable(test_antsCalls test_antsCalls.cpp
        ../src/dev_chargingPile/AntsCalls.cpp)
target_link_libraries(test_antsCalls ${LINK_LIBS})