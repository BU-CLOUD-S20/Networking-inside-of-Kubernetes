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
  $ git clone https://github.com/BU-CLOUD-S20/Networking-inside-of-Kubernetes.git
  $ cd Networking-inside-of-Kubernetes
  ```

- Then move to docker folder, use the script to start a 3-node cluster

  ```bash
  $ cd docker/
  $ ./up.sh                                                                        *[master]
  Creating node1 ... done
  Creating node2 ... done
  Creating node3 ... done
  node1
  node2
  node3
  ```

- Now you can see that the nodes are running

  ```bash
  $ docker ps -a                                                                   *[master]
  CONTAINER ID        IMAGE                              COMMAND             CREATED             STATUS                     PORTS                NAMES
  fdeeeddf27c1        freddiefy/ec528networking-dev:v3   "/bin/bash"         35 seconds ago      Up 34 seconds              8001/tcp, 8003/tcp   node3
  e71b0165edcf        freddiefy/ec528networking-dev:v3   "/bin/bash"         35 seconds ago      Up 33 seconds              8001/tcp, 8003/tcp   node2
  40f43bfc4a24        freddiefy/ec528networking-dev:v3   "/bin/bash"         35 seconds ago      Up 33 seconds              8001/tcp, 8003/tcp   node1
  ```

- Set up three terminals to get into the nodes seperately:

  ```bash
  $ docker exec -it node1 bash
  ```

- Change to our working directory:

  ```bash
  $cd /root/project/src/CPISync/
  ```

- Now you can have fun with Niok and type `help` for instructions:

  ```bash
  root@node1:~/project/src/CPISync# ./niok
  node1: help
  Please use one of the following commands:
  1. put [key] [value]
  2. get [key]
  3. del [key]
  4. join [IPs]
  5. show (show neighbors of current node)
  6. exit
  ```
## 1. How to build

- Starting a docker container like the commands below or use the 3-node clust we just build.

  ```bash
  $ cd docker/
  $ docker run -it --name 528dev -v <your local path to this project>/Networking-inside-of-Kubernetes/:/root/project/ freddiefy/ec528networking-dev:v3 /bin/bash
  ```

- Get into work dir and build the project:

  ```bash
  $ cd /root/project/
  $ mkdir build
  $ cd build
  $ cmake ..
  $ cmake --build .
  ```

- Then if you want to run unit tests:

  ```bash
  root@node1:~/project/build# make test
  Running tests...
  Test project /root/project/build
      Start 1: level_engine_test
  1/3 Test #1: level_engine_test ................   Passed    0.02 sec
      Start 2: gossip_node_test
  2/3 Test #2: gossip_node_test .................   Passed    0.01 sec
      Start 3: json_parsing_test
  3/3 Test #3: json_parsing_test ................   Passed    0.02 sec
  
  100% tests passed, 0 tests failed out of 3
  
  Label Time Summary:
  CPISync    =   0.01 sec*proc (1 test)
  kvstore    =   0.02 sec*proc (1 test)
  log        =   0.02 sec*proc (1 test)
  
  Total Test time (real) =   0.13 sec
  ```

- The executable files are generated in:

  ```bash
  /root/project/build/src/CPIsync/_build
  ```
  
- Data are persistently stored in `/tmp/` by default.

## 2. Vision and Goals Of The Project

etcd is a distributed database that stores master node metadata inside of Kubernetes. etcd implements the RAFT consensus protocol which relies on a leader instance to distribute updates to a majority of follower instances for every sync. Unfortunately, this approach is not scalable. Using an alternative protocol saves the cost of time to leader election and log replication for opertions like 'put' (write) for large clusters.  

We plan to create an time efficient and scalable distriuted database using the “gossip protocol” via the following design decisions:    
+ Set reconciliation between neighbors. That is, nodes will only sync log differences with their neighbors.
+ High Availability. We assume success when neightbor nodes are synced.     
+ Log entries are idempotent and data reflects the latest log entry. We assume time is synced via an atomic clock.   

## 3. Users/Personas Of The Project
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

## 4. Scope and Features Of The Project
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

## 5. Solution Concept:
A high-level outline of the solution:     
Global Architectural Structure of the Project:      

![alt text](https://upload-images.jianshu.io/upload_images/1452123-09556716dc29be12.gif?imageMogr2/auto-orient/strip|imageView2/2/format/gif)    
This gif illustrates the "gossip protocol" we are trying to implement. When a node receives a request, it updates its log and then sends it to its neighbors to reconcile. In this gif, once node 1 reconciles with its neighbors (2, 4, and 7), any client communicating with node 1 will see their request as a success. This process then repeats itself until the update propagates throughout the cluster and a consensus is achieved.       

## 6. Acceptance criteria
+ C++ implementation of the gossip protocol using [CPISync](https://github.com/trachten/cpisync) and [LevelDB](https://github.com/google/leveldb)
+ Garbage collection for old log entries

## 7. Stretch Goals 
+ Detecting failures during networking
+ Benchmark operations against etcd
+ Integrate our database into etcd and run with Kubernetes.  

## 8. Release Planning
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

## 9. Risks   
Potential risks for our project:   
+ Our implementation is not scalable. We do not know how well CPISync handles many long logs.
+ Our implementation may not have enough consistency or reliability to demonstrate a legitimate alternative to RAFT.
