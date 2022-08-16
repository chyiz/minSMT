FROM ubuntu:20.04 AS build

ARG UID=1000
ARG GID=1000

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/NewYork
# Install tools required for building all programs
RUN apt-get update && \
    apt-get install -y build-essential gcc g++ m4 \
    pkg-config python-setuptools python-dev python3-pip unzip wget time \
    gcc-multilib g++-multilib libboost-dev \
    libtiff5-dev libbz2-dev libmp3lame-dev libxslt1-dev libxml2-dev \
    zlib1g-dev libxml-libxml-perl libgomp1 libgmp-dev libmpfr-dev \
    libmpc-dev libxi-dev libxmu-dev freeglut3-dev gettext \
    libunwind-dev libglib2.0-dev groff-base \
    sudo && \
    rm -rf /var/lib/apt/lists/*
RUN pip3 install numpy

# Add minSMT user
RUN groupadd -g $GID -o minsmt
RUN useradd -m -u ${UID} -g ${GID} -s /bin/bash -o minsmt
RUN usermod -a -G sudo minsmt
RUN echo "minsmt ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers


# Copy everything into the container
# COPY . /home/minsmt/minsmt-ae/
VOLUME /home/minsmt/minsmt-ae/
USER ${UID}:${GID}
WORKDIR /home/minsmt/minsmt-ae/
ENV XTERN_ROOT=/home/minsmt/minsmt-ae
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$XTERN_ROOT/dync_hook

# Build complete.
ENTRYPOINT ["/bin/bash"]

