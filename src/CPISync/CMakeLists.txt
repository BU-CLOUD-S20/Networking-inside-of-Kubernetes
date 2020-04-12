niok_add_library(
    cpisync_obj OBJECT
    GossipNode.cc
    ../kvstore/LevelEngine.cc
)

niok_add_subdirectory(test)