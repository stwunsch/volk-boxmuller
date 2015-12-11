# VOLK module with an implementation of the Box Muller algorithm

The Box Muller algorithm [1] transforms uniformly distributed random numbers in [0,1) to numbers, which follow the Gauss distribution with zero mean and variance of one.

The implementation runs about 3x to 3.5x faster than the generic implementation.

For all other questions, see the VOLK project [2].

[1] en.wikipedia.org/wiki/Box%E2%80%93Muller_transform  
[2] www.libvolk.org
