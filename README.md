[![build & test (clang, gcc, MSVC)](https://github.com/kamchatka-volcano/figcone_xml/actions/workflows/build_and_test.yml/badge.svg?branch=master)](https://github.com/kamchatka-volcano/figcone_xml/actions/workflows/build_and_test.yml)

**figcone_xml** - is a [`rapidxml`](https://github.com/dwd/rapidxml) configuration parser adapter for [`figcone`](https://github.com/kamchatka-volcano/figcone) library. 


## Installation
Download and link the library from your project's CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(figcone_xml
    GIT_REPOSITORY "https://github.com/kamchatka-volcano/figcone_xml.git"
    GIT_TAG "origin/master"
)
#uncomment if you need to install figcone_xml with your target
#set(INSTALL_FIGCONE_XML ON)
FetchContent_MakeAvailable(figcone_xml)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE figcone::figcone_xml)
```

For the system-wide installation use these commands:
```
git clone https://github.com/kamchatka-volcano/figcone_xml.git
cd figcone_xml
cmake -S . -B build
cmake --build build
cmake --install build
```

Afterwards, you can use find_package() command to make the installed library available inside your project:
```
find_package(figcone_xml 0.9.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE figcone::figcone_xml)
```

## Running tests
```
cd figcone_xml
cmake -S . -B build -DENABLE_TESTS=ON
cmake --build build
cd build/tests && ctest
```

## License
**figcone_xml** is licensed under the [MS-PL license](/LICENSE.md)  
