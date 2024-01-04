include(FetchContent)
FetchContent_Declare(simple_config
        GIT_REPOSITORY https://github.com/joaquinbejar/simple_config.git
        GIT_TAG dev
)
FetchContent_MakeAvailable(simple_config)
if (CMAKE_DEBUG)
    message(STATUS "simple_logger/cmake SIMPLE_CONFIG_SOURCE_DIR ${simple_config_SOURCE_DIR}")
endif ()
set(SIMPLE_CONFIG_INCLUDE ${simple_config_SOURCE_DIR}/include CACHE INTERNAL "")