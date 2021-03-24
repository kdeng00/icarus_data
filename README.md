# icarus_data

C++ 14 library used to interact with the icarus database.


## Dependencies

* libmysqlclient
* soci


## Getting started

```Bash
git clone --recursive https://github.com/kdeng00/icarus_data
cd icarus_data
```

Bootstrap vcpkg

```Bash
3rdparty/vcpkg/bootstrap.sh
```

or have your vcpkg root path in mind and assign it the cmake variable ``VCPKG_ROOT_PATH``


Install your dependencies (soci is added as a subdirectory)

```Bash
vcpkg install libmysql
```


Build the library

```Bash
mkdir build
cd build
cmake .. -DVCPKG_ROOT_PATH=/root/vcpkg/path
cmake --build . --config debug
```
