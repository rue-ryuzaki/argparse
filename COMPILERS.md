| Standard | Standard option `-std=` or `/std:` support | Library requires |
|-|-|-|
| C++98/C++03 | | gcc: 3.0 or higher<br />clang: 2.7 or higher |
| (C++0x) C++11 | gcc: (4.3-4.6.4) 4.7.1 or higher<br />clang: (2.7-2.9) 3.0 or higher | gcc: 4.4 or higher<br />clang: 3.3 or higher |
| (C++1y) C++14 | gcc: (4.8.1-4.8.5) 4.9.0 or higher<br />clang: (3.2-3.4.2) 3.5 or higher<br />msvc: v19.20 vs16.0 or higher | gcc: 4.8.1 or higher<br />clang: 3.4 or higher<br />msvc: v19.20 vs16.0 or higher |
| (C++1z/latest) C++17 | gcc: (5.1) 5.1 or higher<br />clang: (3.5-4.0.1) 5.0.0 or higher<br />msvc: v19.20 vs16.0 or higher | gcc: 7.1 or higher<br />clang: 5.0.0 or higher<br />msvc: v19.20 vs16.0 or higher |
| (C++2a/latest) C++20 | gcc: (8.1-9.5) 10.1 or higher<br />clang: (5.0.0-9.0.1) 10.0.0 or higher<br />msvc: (v19.20 vs16.0-v19.28 vs 16.9) v19.29 vs16.11 or higher | gcc: 8.1 or higher<br />clang: 5.0.0 or higher<br />msvc: v19.20 vs16.0 or higher |
| (C++2b/latest) C++23 | gcc: (11.1) 11.1 or higher<br />clang: (12.0.0-16.0.6) 17.0.1 or higher<br />msvc: (v19.29 vs16.10 or higher) | gcc: 11.1 or higher<br />clang: 12.0.0 or higher<br />msvc: v19.29 vs16.10 or higher |
| (C++2c/latest) C++26 | gcc: (14.1) 14.1 or higher<br />clang: (17.0.1) 17.0.1 or higher | gcc: 14.1 or higher<br />clang: 17.0.1 or higher |

msvc: versions starting from 19.20 (visual studio 2019) are tracked; doesn't support C++98 and C++11

other compilers: no actual data

### C++98/C++03 standard requirement
 - SFINAE
#### Required headers
 - stdint.h (current standard)
 - cctype
 - cstddef
 - cstdlib
 - deque
 - fstream
 - iostream
 - list
 - queue
 - sstream
 - stdexcept
 - string
 - utility
 - vector
### C++11 standard requirement
#### Required headers
 - array
 - cstdint
 - functional
 - initializer_list
 - memory
 - regex
 - tuple
 - type_traits
 - unordered_map
### C++14 standard requirement
### C++17 standard requirement
#### Required headers
 - optional
 - string_view
#### Optional headers
 - version
### C++20 standard requirement
#### Optional headers
 - span
### C++23 standard requirement
### C++26 standard requirement
