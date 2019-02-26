# Notes

- Network Design considerations
  - Scalability
  - Performance
  - Resilience 弹性，承担错误的能力
  - Cost

Network Devices -> Interconnection Network Topology -> Application

## Network Topology

- Diameter(latency): the longest path in a graph
- Bisection(resilience): how many cuts needed to cut off the graph into two pieces
- Links(cost): how many links to connect the graph.
- Degree(scalability): #fan-in #fan-out

1. Linear Array
    P-1, 1, P-1, 2
    前两个属性表现很差
2. Ring
    p/2, 2, p, 2
3. Tree
   2$\log_2P$P, 1, 2(p-1), 3
4. 2-D mesh(2 维网格)
    2($\sqrt(p)-1$), $\sqrt(p)$, 2$\sqrt(p)(\sqrt(p)-1), 4
    mesh比较常见
5. 2-D torus
    $\sqrt(p) - 1$, $2\sqrt p$, 2p, 4
6. Hypercude
    $\log_2$p, p/2, p/2*$\log_2$p, $\log_2p$
    小的switch 内部， 没法做的很大

## Interconnection Network

## Storage

- Lustre file system, MPI-IO
- Burst buffering
