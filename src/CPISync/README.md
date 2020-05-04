- To compile niok.cc:

  ```bash
  g++ -std=c++14 niok.cc GossipNode.cc ../kvstore/LevelEngine.cc -o niok -lcpisync -lntl -lpthread -lgmp -lcppunit -lleveldb -ltacopie
  ```

  - Benchmark usage (run one nodes 1, 2 and 3):
   ```bash
   node1: kill -9 -1
   node1: ./bench s 172.28.1.2 172.28.1.3
   node2: ./bench c
   node3: ./bench c
   ```
