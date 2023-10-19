
add_executable(test_simple_logger test_simple_logger.cpp)
target_include_directories(test_simple_logger
        PRIVATE
        ${SIMPLE_LOGGER_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
)

target_link_libraries(test_simple_logger PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_logger PRIVATE
        simple_logger
        simple_color
        simple_config
)


add_executable(test_logger_multi_thread test_logger_multi_thread.cpp)
target_include_directories(test_logger_multi_thread
        PRIVATE
        ${SIMPLE_LOGGER_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
)

target_link_libraries(test_logger_multi_thread PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_logger_multi_thread PRIVATE
        simple_logger
        simple_color
        simple_config
)
