#!/bin/bash
echo "Compiling minSMT library..."
mkdir -p obj
pushd obj
make clean
../configure --prefix=$XTERN_ROOT/install
make && make install
popd

cd $XTERN_ROOT/apps
echo "Compiling ldap, sudo password maybe required..."
pushd ldap
./mk
popd

echo "Compiling openmp library..."
pushd openmp
./mk
popd

# echo "Compiling aget..."
# pushd aget
# ./mk
# popd

echo "Compiling ImageMagick..."
pushd imagick
./mk
popd

echo "Compiling mongoose..."
pushd mongoose
./mk
popd

echo "Compiling npb..."
pushd npb
./mk
popd

echo "Compiling parsec..."
pushd parsec
export PARSECPLAT=x86_64-linux.gcc
./setup-parsec
popd

echo "Compiling pfscan..."
pushd pfscan
./mk
popd

echo "Compiling redis..."
pushd redis
./mk
popd

echo "Compiling stl..."
pushd stl
./mk
popd

echo "Compiling bdb_bench3n..."
pushd bdb_rep
./mk
popd

echo "Compiling mplayer mencoder..."
pushd mplayer
./mk
popd

echo "Compiling pbzip2..."
pushd pbzip2
./mk
popd

echo "Compiling phoenix..."
pushd phoenix
./mk
popd

echo "Compiling splash2x..."
pushd splash2x
./mk
popd

echo "All compiled. If there are no errors, you may go to $XTERN_ROOT/eval folder to run evaluation."

