set(KVSTORE_TEST_LIBS
    $<TARGET_OBJECTS:kvstore_obj>)


niok_add_test(
    NAME    level_engine_test
    SOURCES LevelEngineTest.cc
    OBJECTS ${KVSTORE_TEST_LIBS}
    LIBRARIES ${LEVELDB_LIBRARIES} ${GOOGLETEST_LIBRARIES} levelengine_lib
)
