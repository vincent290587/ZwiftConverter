

mkdir TDDV
cmake -G "Visual Studio 16 2019" -S . -B TDDV

mkdir TDDW
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug -S . -B TDDW

