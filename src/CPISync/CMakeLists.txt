niok_add_library(
    cpisync_obj OBJECT
    GossipNode.cc
    ../kvstore/LevelEngine.cc
)
ADD_EXECUTABLE(Niok niok.cc GossipNode.cc ../kvstore/LevelEngine.cc)
TARGET_LINK_LIBRARIES(Niok cpisync ntl pthread gmp cppunit leveldb tacopie)

ADD_EXECUTABLE(sync sync.cc)
TARGET_LINK_LIBRARIES(sync cpisync ntl pthread gmp cppunit)

ADD_EXECUTABLE(bench benchmark.cc GossipNode.cc ../kvstore/LevelEngine.cc) 
TARGET_LINK_LIBRARIES(bench cpisync ntl pthread gmp cppunit leveldb tacopie)

niok_add_subdirectory(test)