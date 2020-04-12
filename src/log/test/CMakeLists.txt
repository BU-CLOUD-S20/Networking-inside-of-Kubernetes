set(LOG_TEST_LIBS
    $<TARGET_OBJECTS:log_obj>)


niok_add_test(
    NAME    json_parsing_test
    SOURCES JsonParsingTest.cc
    OBJECTS ${LOG_TEST_LIBS}
    LIBRARIES ${GOOGLETEST_LIBRARIES}
)
