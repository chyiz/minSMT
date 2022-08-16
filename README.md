Hardware and software requirements:
================
- Hardware: We conducted our experiments on a machine with a Xeon
    E5-2695 V4 processors and 64GB memory. While our code should be
    runnable on any multi-core machines, we recommend a CPU with at 
    least 16 logical cores and similar memory configurations.

    Building all benchmarks also takes about 70GB of disk space, so 
    make sure you have enough free disk space on the volume you use.

- OS: Ubuntu 20.04, 64 bit. This is the version we used to compile
    minSMT and all our benchmarks, and the scripts provided in this
    zip files have been tested on this OS version. 
    We also provide a Docker environment to help you run it under 
    other Linux distributions. Most distributions with recent kernel
    versions should work using Docker. 

- Other Software: Instructions to install extra libraries and tools are
    provided below.

A note on the name "XTERN" and copyright issues
----------------
Our library is developed based on Parrot from 
https://github.com/columbia/xtern
, and XTERN is the name used during Parrot's development. We did not 
change the code name. Also, we will add more copyright note into our 
code and other documents before publicly sharing our code.

Build minSMT and Benchmarks with Docker
================
We prepared a Ubuntu 20.04 Docker environment to help you compile and 
run our experiments.
Clone this repository onto your local system, and while in the 
repository root folder, run the following command:
```
$ sudo DOCKER_BUILDKIT=1 docker build --build-arg UID=$(id -u) --build-arg GID=$(id -g) -t minsmt-ae .
```
to build a docker image with all necessary packages needed to compile 
minSMT and all benchmarks. This step should take about 5 minutes with 
good internet speeds.

After that, a docker image will be available with tag minsmt-ae. 
Make sure you are still at project root folder, 
and run it with the following command:
```
$ sudo docker run -it -v `pwd`:/home/minsmt/minsmt-ae minsmt-ae:latest
```
You will enter an BASH shell at `/home/minsmt/minsmt-ae` folder, and the 
repository on your host machine is mounted on that path in the 
container. If you ever exit the docker environment, simply go back 
to the repository root folder and run the above command again.

This Docker container has all the neccessary libraries and packages needed 
to compile our library and all benchmarks we will use. Now just run
```
$ ./buildall-docker.sh
```
to start building them. This step will take about two hours. 
During this time, make sure you have stable internet connection, 
since the build script will also download benchmarks from their official website, 
as well as their input files.

After all is built, you can jump to the "Generate Evaluation Results" 
section.


Build without Docker
================
Alternatively, you may built the project directly. The build script are 
tested under Ubuntu 16.04 and may not work on other Linux distributions.

1. Install some libraries/tools.
```
$ sudo apt-get install build-essential gcc-5 g++-5 gcc-4.7 g++-4.7 m4 \
pkg-config python-pip python-setuptools python-dev python3-pip unzip git wget
$ sudo apt-get install gcc-multilib g++-multilib libboost-dev \
libtiff5-dev libbz2-dev libmp3lame-dev libxslt1-dev libxml2-dev \
zlib1g-dev libxml-libxml-perl libgomp1 libgmp-dev libmpfr-dev \
libmpc-dev libxi-dev libxmu-dev freeglut3-dev gettext \
libjasper-runtime libjasper-dev libunwind-dev
$ sudo pip3 install numpy
```

1. Add $XTERN_ROOT (the absolute path of "xtern") into environment
variables by running
```
$ source env.sh
```
in the minSMT root library.

2. Run the build script to build everything
```
$ cd $XTERN_ROOT
$ ./buildall.sh
```

Generate Evaluation Results
================
First, switch to the evaluation folder:
```
$ cd $XTERN_ROOT/eval
```
In $XTERN_ROOT/eval folder, simply run 
```
$ ./eval_policy.py all-compare-with-qithread.cfg
```
to test all benchmarks. This will run all benchmarks 50 times with 
QiThread, Null Policy, and minSMT configurations, and take the average 
of each configuration. 

Evaluating all benchmarks will take about a couple of days (around five
on our platform) to finish.

Alternatively, you may run
```
$ ./eval_policy.py all-compare-with-qithread-small.cfg
```
This will only run each benchmarks for 10 times and should finish in a 
day or two, or
```
$ ./eval_policy.py minSMT-subset-compare-with-qithread.cfg
```
which will only run a curated set of benchmarks that we have discussed in the paper. 

The evaluation results will be saved to the
`all-default-config<Execution Date and Time>_<git commit>` folder.
A symbolic link folder `current` is also modified to always point to the last
evaluation results.

To extract all results into one file, run
```
$ ./get_all_results.sh current/ > results.csv
```

Replace "current" with the folder name that contains evaluation results 
if you are exporting older evaluations. Do not forget the slash after the 
folder name.

The results is a comma separated values (csv) file that can then be
imported to a spreadsheet.

At last, to generate the graphs used in our paper, open 
generate-figure.ipynb in the eval folder with Jupyter Notebook.
It will load results.csv in the same folder by default. 

Customize Evaluation Configurations
================
Running an Individual Benchmark
----------------
You may extract a single test case from $XTERN_ROOT/eval/all-compare-with-qithread.cfg
and save to another file, and then run it with the same evaluation script:
```
$ ./eval_policy.py <your configuration file>
```

Adjust Policies
----------------
Each benchmark section in a evaluation configuration file (.cfg file) has a 
`RUN_CONFIGS` parameter. It is used to select which policies to run. 
In `all-compare-with-qithread.cfg` file mentioned above, three configurations 
are enabled: `qithread` means all the policies introduced in the QiThread paper, 
but nothing else; `null-policy` is the TOND-Sync mode introduced in our paper, and 
finally `all-policies` are all policies available to minSMT, including the ones from QiThread. `pcs-` can be added before `null-policy` and `all-policies` to enable CSFine policy. Other available options are: `no-hint` for pure round-robin scheduling, `hinted` for Parrot paper configurations, and `no-pcs-hint` for Parrot paper configurations without the PCS hint.

Fine Tune Parameters
----------------
You may fine tune minSMT library parameters using the `local.options` file, 
which will be read automatically from the same directory as the benchmark binary.
The easiest way to start is first use the above method to run a benchmark, 
then you will get the "results folder" in the form of 
`<Config File Name><Execution Date and Time>_<git commit>`. 
That folder contains subfolders named using each benchmark sections in your configuration file.
Enter one of the subfolders, and you will see the binary of that benchmark, 
inputs it used, outputs it generated during evaluation, 
and one or more `.options` files the evaluation script generated, each represents 
a `RUN_CONFIGS` designated in the configuration file, and a `local.options` file
is the last options file used.

You can rename existing `options` file to `local.options` so that it can be picked up by the next execution.
For the meaning of each parameter, please refer to the `default.options` file in the repository. 

After you finish adjusting the `local.options` file, you may run the benchmark using 
the following command
```
$ LD_PRELOAD=$XTERN_ROOT/dync_hook/interpose.so ./<benchmark_binary> <benchmark parameters>
```

Applying Delays
----------------
To apply delays to benchmarks, first follow the above instructions to first run a benchmark once, and get the results folder. 
Then `cd` into one of the benchmark inside the results folder,
and create a file called `app.time` alone side the `local.options` file, 
and edit `local.options` file to set `enforce_delays = 1`.
Now run the benchmark using the same command with `LD_PRELOAD` as above.



