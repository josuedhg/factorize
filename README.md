# Factorize

To compile run

```mkdir build && cd build && cmake .. && make```

To run server

```./server/factorizesrv```

To run client
```./client/factorizecli --nums=1,2,3,4```

To get help
```./client/factorizecli --help```
```./server/factorizesrv --help```

Compile Unit tests
```mkdir build && cd build && cmake -DTESTS=ON .. && make check```

Unittests require cmoka installed it could be found in ubuntu 10.04 as libcmocka-dev
