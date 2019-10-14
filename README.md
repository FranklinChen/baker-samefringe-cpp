# C++ implementation of Henry Baker's same-fringe code

[![Build Status](https://travis-ci.org/FranklinChen/baker-samefringe-cpp.svg)](https://travis-ci.org/FranklinChen/baker-samefringe-cpp)

I use Boost's [variant](https://theboostcpplibraries.com/boost.variant)
because C++17's [`std::variant`](https://en.cppreference.com/w/cpp/utility/variant) does not support recursive variant types, and it's not worth [working around that](https://stackoverflow.com/questions/39454347/using-stdvariant-with-recursion-without-using-boostrecursive-wrapper).

Check out my implementations in other languages:

- [Rust](https://github.com/FranklinChen/baker-samefringe-rust)
- (others to come)
