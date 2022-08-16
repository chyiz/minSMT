export PARSECDIR=$XTERN_ROOT/apps/splash2x/parsec-3.0
export PARSECPLAT=amd64-linux

[ -d "./parsec-3.0/ext/splash2x/apps/$@" ] && rm -r ./parsec-3.0/ext/splash2x/apps/$@/obj ./parsec-3.0/ext/splash2x/apps/$@/inst
[ -d "./parsec-3.0/ext/splash2x/kernels/$@" ] && rm -r ./parsec-3.0/ext/splash2x/kernels/$@/obj ./parsec-3.0/ext/splash2x/kernels/$@/inst

./parsec-3.0/bin/parsecmgmt -a build -p splash2x.$@
