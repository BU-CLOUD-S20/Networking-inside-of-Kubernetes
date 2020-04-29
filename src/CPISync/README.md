- To compile niok.cc:

  ```bash
  g++ -std=c++14 niok.cc GossipNode.cc ../kvstore/LevelEngine.cc -o niok -lcpisync -lntl -lpthread -lgmp -lcppunit -lleveldb -ltacopie
  ```

  - Benchmark usage (run one nodes 1, 2 and 3):
   ```bash
   ./bench s 172.28.1.1
    ./bench s 172.28.1.1
    ./bench s 172.28.1.1
   ```
