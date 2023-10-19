
add_executable(test_simple_logger test_simple_logger.cpp)
target_include_directories(test_simple_logger
        PRIVATE
        ${SIMPLE_LOGGER_INCLUDE}
)

target_link_libraries(test_simple_logger PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_logger PRIVATE simple_logger)
