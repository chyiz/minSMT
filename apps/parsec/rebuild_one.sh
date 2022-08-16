LIBRARY_PATH=/usr/lib/x86_64-linux-gnu 
export LIBRARY_PATH 

PARSECDIR=$XTERN_ROOT/apps/parsec/parsec-3.0
PARSECPLAT=amd64-linux


[ -d "./parsec-3.0/pkgs/apps/$1" ] && rm -r ./parsec-3.0/pkgs/apps/$1/obj ./parsec-3.0/pkgs/apps/$1/inst
[ -d "./parsec-3.0/pkgs/kernels/$1" ] && rm -r ./parsec-3.0/pkgs/kernels/$1/obj ./parsec-3.0/pkgs/kernels/$1/inst

./parsec-3.0/bin/parsecmgmt -a build -p $1
./parsec-3.0/bin/parsecmgmt -a build -c gcc-openmp -p $1
