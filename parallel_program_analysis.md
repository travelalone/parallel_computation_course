# Parallel Program Analysis

## Speedup Factor

program speed up factor: $S(P) = \frac{T_s}{T_p}​$

- $T_s​$: execution time using the best sequential algorithm
- $T_p​$: execution time using p processor

Linear speedup: $S(p) = p​$

- ideal maximum speedup in theory
suplinear speedup: $s(p) > P​$
- occasioanlly happend in practice
- extra HW resource (momory)
- SW or HW optimization (caching)

System efficiency:
$$E(p) = \frac{T_s}{T_P \times p} = \frac{S(p)}{p}\times 100\%$$

difficule to reach ideal:

- not every part of a computation can be parallelized (result in processor idle)
- need extra computaions in the parallel version (synchronization cose)
- communication time between process(normally the major factor)

## Strong scalability

- Strong Scaling (比较难达到， 例如资源不够)
    1. The problem size stays fixed but the number of processing elements are increased
    2. it is used to find a "sweet spot" that allows the computation to complete in a reasonable amount of time, yet dose not waste too many cycles due to parallel overhead

- Weak Scaling
    1. The problem size assigned to each processing element stays fixed and additional processing elements are used to solve a larger total problem.
    2. it is a justification for programs that take a lof of memory or other system resources.

## Time complexity Analysis

$$T_p = T_{comp}+T_{comm}$$

- $T_P$ total execution time of parallel algorithm
- $T_{comp}$: computation part
- $T_{comm}$: communication part

$$T_{comm} = q(T_{start} + nT_{data})$$

- $T_{\text{startup}}$: Message latency
- $T_{\text{data}}$: Transmission time to send one data item
- $n$: number of data items in a message
- $q$: Number of message

## Cost-Optimal Algorithm

Defination: cost to solve a problem is proportional to the execution time on a single processor system.
$$O(T_p) \times N = O(T_s)​$$