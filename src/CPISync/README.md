- To compile niok.cc:

  ```bash
  g++ -std=c++14 niok.cc GossipNode.cc ../kvstore/LevelEngine.cc -o niok -lcpisync -lntl -lpthread -lgmp -lcppunit -lleveldb -ltacopie
  ```

  