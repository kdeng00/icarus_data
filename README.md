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
```

### Linux

```Shell
cmake .. -DVCPKG_ROOT_PATH=/root/vcpkg/path
make -j
```

### Windows

Has not been implemented

```Shell
cmake -G "Visual Studio 14 2015 Win64" -DVCPKG_ROOT_PATH=D:/root/vcpkg/path
cmake --build . -j --config debug
```


