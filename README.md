# Networking-inside-of-Kubernetes   

## Presentations:
[Demo 1](https://docs.google.com/presentation/d/1jBuHELqt4QC5a1PdA2_NT6TXpMmmihoYNoQsa29Ajbg/edit?usp=sharing)  
[Demo 2](https://docs.google.com/presentation/d/118YX5ALFa08UfWFQjXKwZok-rmvdTvqiiVNljjjsXfg/edit?usp=sharing)  
[Demo 3](https://docs.google.com/presentation/d/142T3vpOa8HJgMVeeW43rnJGhvw62su00DX_PcEBc43o/edit?usp=sharing)  
[Demo 4](https://docs.google.com/presentation/d/17R5dB9XDbUxkLGO7eynz5Ey_Jk6t1EQXyuoymtE9mOo/edit?usp=sharing)    
[Demo 5](https://docs.google.com/presentation/d/1ABRTniuiQnKOEnzjII-o9OAfOhxerbpnJR1S9sTKy1g/edit?usp=sharing)  
[GFS](https://docs.google.com/presentation/d/13toeO3h6mSGOpCpLhhtjaLkJJfPJ-Vc3zHOx81E8Ps0/edit?usp=sharing)  
## Contributors:   
Mentors:  
[Anish Gupta](github.com/nkanish2002)    
[Bowen Song](github.com/Bowenislandsong)  
Team:  
[Alexander Trinh](github.com/aktrinh17)	    
[Fuyao Wang](github.com/wfystx)	  
[Zhe Deng](github.com/thezhe)    
[Geng Song](github.com/827265717)   
[Zhou Shen](github.com/zhou-1)  	   

## 0. Get started
- Firstly, clone the project

  ```bash
  git clone https://github.com/BU-CLOUD-S20/Networking-inside-of-Kubernetes.git
  cd Networking-inside-of-Kubernetes
  ```

- Then move to docker folder, and run the following commands to run a container of our image. Then get into the container, move to /root/project/ directory which is the work directory.

  ```bash
  cd docker/
  docker run -it --name 528dev -v <your local path to this project>/Networking-inside-of-Kubernetes/:/root/project/ freddiefy/ec528networking-dev:v1 /bin/bash
  ```

- To build the project:

  ```bash
  mkdir build
  cd build
  cmake ..
  cmake --build .
  ```

- Then if you want to run unit tests:

  ```bash
  root@69870374073e:~/project/build# ctest
  Test project /root/project/build
      Start 1: level_engine_test
  1/1 Test #1: level_engine_test ................   Passed    0.02 sec
  
  100% tests passed, 0 tests failed out of 1
  
  Label Time Summary:
  kvstore    =   0.02 sec*proc (1 test)
  
  Total Test time (real) =   0.11 sec
  ```

- Or directly using the specific test you want to run:

  ```bash
  root@69870374073e:~/project/build# src/kvstore/test/_build/level_engine_test
  [==========] Running 2 tests from 1 test case.
  [----------] Global test environment set-up.
  [----------] 2 tests from LevelEngineTest
  [ RUN      ] LevelEngineTest.SimpleTest
  keyval
  Release leveldb on /tmp/leveldb_engine_SimpleTest.XXXXXX0
  [       OK ] LevelEngineTest.SimpleTest (10 ms)
  [ RUN      ] LevelEngineTest.RemoveTest
  keyval
  Get: key Not Found
  Release leveldb on /tmp/rocksdb_engine_RemoveTest.XXXXXX0
  [       OK ] LevelEngineTest.RemoveTest (5 ms)
  [----------] 2 tests from LevelEngineTest (15 ms total)
  
  [----------] Global test environment tear-down
  [==========] 2 tests from 1 test case ran. (15 ms total)
  [  PASSED  ] 2 tests.
  ```
  
## 1. Vision and Goals Of The Project
etcd is a distributed database that stores master node metadata inside of Kubernetes. etcd implements the RAFT consensus protocol which relies on a leader instance to distribute updates to a majority of follower instances for every sync. Unfortunately, this approach is not scalable. Using an alternative protocol saves the cost of time to leader election and log replication for opertions like 'put' (write) for large clusters.  

We plan to create an time efficient and scalable distriuted database using the “gossip protocol” via the following design decisions:    
+ Set reconciliation between neighbors. That is, nodes will only sync log differences with their neighbors.
+ High Availability. We assume success when neightbor nodes are synced.     
+ Log entries are idempotent and data reflects the latest log entry. We assume time is synced via an atomic clock.   

## 2. Users/Personas Of The Project
Our database targets Kubernetes developers using large clusters ([>> 1000 nodes in magnitude][1]). The gossip protocol accommodates hardware and design limitations (e.g. [hard disk speed][2], restricted cache availability) by reducing redundant writes and leader bottlenecks.     
This project will NOT:    
+ Target end-users of Kubernetes  
+ Replace RAFT in etcd
+ Sync time-sensitive data which requires strong consistency    
+ Store [more than a few gigabytes][3] of data (>8 GB) of data
+ Target [Horizontally scaling databases][4] (>1 cluster)

[1]: https://github.com/kubernetes/kubernetes/issues/20540
[2]: https://openai.com/blog/scaling-kubernetes-to-2500-nodes/
[3]: https://github.com/etcd-io/etcd/blob/master/Documentation/dev-guide/limit.md
[4]: https://github.com/etcd-io/etcd/blob/master/Documentation/learning/why.md

## 3. Scope and Features Of The Project
Our implementation will use the CPISync library to implement set reconciliation between neighbors nodes' logs. Furthermore, garbage collection can piggyback on these pings to remove old log entries.

We plan to implement a transparent API that is consistent with a subset of etcd's API:

We are implementing:
-put
-get
-del

We plan to implement (subject to change):
-snapshot
-watch
-member add/remove

## 4. Solution Concept:
A high-level outline of the solution:     
Global Architectural Structure of the Project:      

![alt text](https://upload-images.jianshu.io/upload_images/1452123-09556716dc29be12.gif?imageMogr2/auto-orient/strip|imageView2/2/format/gif)    
This gif illustrates the "gossip protocol" we are trying to implement. When a node receives a request, it updates its log and then sends it to its neighbors to reconcile. In this gif, once node 1 reconciles with its neighbors (2, 4, and 7), any client communicating with node 1 will see their request as a success. This process then repeats itself until the update propagates throughout the cluster and a consensus is achieved.       

## 5. Acceptance criteria
+ C++ implementation of the gossip protocol using [CPISync](https://github.com/trachten/cpisync) and [LevelDB](https://github.com/google/leveldb)
+ Garbage collection for old log entries

## 6. Stretch Goals 
+ Detecting failures during networking
+ Benchmark operations against etcd
+ Integrate our database into etcd and run with Kubernetes.  

## 7. Release Planning
- **First Step** (1~2 weeks)
  - Understand the basics of C++ and Gossip Protocol 
  - Get familiar with etcd's architecture
  - Learn how consensus protocol (RAFT) and what information syncs between nodes
- **Release 1** (1~2 weeks)
  - Implementation a simple example of Gossip reconciliation between two instances 
  - Utilize LevelDB and CPIsync (more specifically Interactive CPIsync)
- **Release 2** (4~6 weeks)
  - Define how logging works in our implementation
  - Implement gossip protocol to synchronize data multiple nodes
  - Write tests and benchmarks for both databases
- **Release 3** (1 week)
  - Debug and test some more
  - Work on stretch goals

## 8. Risks   
Potential risks for our project:   
+ Our implementation is not scalable. We do not know how well CPISync handles many long logs.
+ Our implementation may not have enough consistency or reliability to demonstrate a legitimate alternative to RAFT.
