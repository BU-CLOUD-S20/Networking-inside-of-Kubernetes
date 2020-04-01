set(CPISYNC_TEST_LIBS
    $<TARGET_OBJECTS:cpisync_obj>)


niok_add_test(
    NAME    gossip_node_test
    SOURCES GossipNodeTest.cc
    OBJECTS ${CPISYNC_TEST_LIBS}
    LIBRARIES 
        ${GOOGLETEST_LIBRARIES}
        cpisync_lib
        cpisync
        gmp 
        ntl 
)
