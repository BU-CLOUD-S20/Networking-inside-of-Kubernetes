# Networking-inside-of-Kubernetes   

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
Etcd is a distributed database that stores concurrent cluster metadata such as node information inside of Kubernetes, and Etcd currently uses the Raft consensus algorithm which first updates go to a leader, then distributes updates to at least a majority of other Etcd cluster members before replying to the client.      
The Raft algorithm is not able to scale due to its single leader node architecture and causes slow replies to the client.     
Therefore, high level  goal of our projects is creating an alternative to the Raft algorithm with a bandwidth-efficient and faster “gossip protocol” which will include:    
+ A fast, stable algorithm that reply to the client in a shorter time. We assume a success when the neighbors of a node are written to.     
+ Use “gossip protocol” that sync to neighbouring nodes directly (set up “practical set reconciliation”) instead of updating to leader node first     
+ Enabling an eventually consistent service with minimal cost performance for large scale clusters     

## 2. Users/Personas Of The Project
The modified version of Etcd targets Kubernetes developers and large clusters (much more than the [current usable limit of 1000][1] nodes in magnitude) that depend on Etcd for handling metadata and coordination. The gossip protocol accommodates hardware and design limitations (e.g. [hard disk speed][2], restricted cache availability) by reducing redundant writes.     

This project will NOT:    
+ Target end-users of Kubernetes  
+ Replace RAFT in Etcd
+ Handle data that cannot tolerate eventual consistency    
+ Store [more than a few gigabytes] of data[3] (>8 GB) of data (use NewSQL instead)
+ Target [Horizontally scaling databases][4] (>1 cluster)

[1]: https://github.com/kubernetes/kubernetes/issues/20540
[2]: https://openai.com/blog/scaling-kubernetes-to-2500-nodes/
[3]: https://github.com/etcd-io/etcd/blob/master/Documentation/dev-guide/limit.md
[4]: https://github.com/etcd-io/etcd/blob/master/Documentation/learning/why.md

## 3. Scope and Features Of The Project
The scope of the project aims at large scale metadata stored in Etcd which need to be updated in time. Instead of Raft consensus algorithm, gossip protocol discards the “leader approach” and saves the cost of time to leader election and. Having one leader create a one point of failure not to mention that the leader has to communicate with all nodes for an update. Failure detection guarantee the reliability of the network from periodically pinging each node by its neighbors, which mitigates the  average error rate during the process of handshake.

Therefore, the main features are:
- Handling updates across global metadata in higher speed by gossip protocol
- Responding to client requests quicker 
- Failure detection ensures the robustness and reliability of the network

## 4. Solution Concept:
A high-level outline of the solution:     
Global Architectural Structure of the Project:      

![projectDiagram](images/Project_Diagram.png)

This diagram illustrates our global architectural design for the project. The Cluster is depicted as being updated with new information from the user. Each worker node is then shown communicating and updating its neighboring nodes. This information then propagates throughout the entire cluster, thus completing reconciliation using a “gossip protocol”. 

![alt text](https://upload-images.jianshu.io/upload_images/1452123-09556716dc29be12.gif?imageMogr2/auto-orient/strip|imageView2/2/format/gif)    

This gif illustrates the "gossip protocol" we are trying to implement. The idea is to have a node receive some sort of update, and having it communicate with its neighboring nodes. This process then repeats itself until the update propagates throughout the cluster and a consensus is achieved.

Design Implications and Discussion:      
This practical set reconciliation algorithm will be replacing the Raft consensus algorithm. Raft makes use of a master node which distributes updates to its cluster of nodes. While this algorithm is fairly consistent, it fails to remain efficient as the number of nodes scales up, e.g. large scale clusters with over 5000+ nodes. As a result, our gossip protocol based algorithm will solve the issue of inefficiency with large scale environments, at the cost of consistency.       

## 5. Acceptance criteria
Minimum acceptance criteria is that compared with current Raft algorithm, creating a faster and stable algorithm with “gossip protocol”. Stretch goals are:    
+ Deploying our implementation of the gossip protocol using [CPISync](5) and [LevelDB](6) to containers     
+ Shortening the time to respond to client request and benchmarking our implementation against etcd    
+ Detecting failures during networking

[5]: https://github.com/trachten/cpisync
[6]: https://github.com/google/leveldb

## 6. Release Planning

- **First Step** (1~2 weeks)
  - Team members should have the basics of C++ and Gossip Protocol. 
  - Everyone should also get familiar with etcd's source code and architecture.
  - Meantime, discuss about how consensus protocol(Raft) works in etcd and what information should be synchronized between instances to perform updates.
- **Release 1** (1~2 weeks)
  - A simple implementation of reconciling information between two instances of the new database. 
  - LevelDB, the key-value storage, CPIsync library, more specifically the interactive CPIsync, should be used to achieve this.
- **Release 2** (4~6 weeks)
  - Codes related to updating the key value store and logs (code piece such as electing leader, heatbeating) in etcd should be swapped out.
  - Implement gossip protocol to synchronize data between nodes.
  - Test cases are also expected to come together.
- **Release 3** (1 week)
  - Debug done, the new system should work.
- **Final Step** (Hopefully) (Class ends)
  - Use the stable released version to replace the metadata storage in Kubernetes.

## 7. Risks   
Potential risks for our project:   
+ Our implementation is not scalable.    
+ Our algorithm may not be able to have enough consistency or reliability to offer a legitimate alternative to the Raft algorithm

## General comments
Before implementing “gossip protocol”, the first step is to get practical set reconciliation between two instances, then test its performances in small groups in comparison to Raft. The result might be little in differences when group size is small, then we record the performance as an enlarging set.
