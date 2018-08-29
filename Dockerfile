FROM ubuntu:16.04
RUN apt-get update && apt-get -y install \
    build-essential \
    cmake \
    git \
    libhdf5-dev \
    libboost-all-dev \
    qtbase5-dev

RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 \
  --recv 0C49F3730359A14518585931BC711F9BA15703C6 \
  && echo "deb [ arch=amd64,arm64 ] http://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/3.4 \
  multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.4.list \
  && apt-get update \
  && apt-get install -y mongodb-org libmongoc-1.0.0

ENTRYPOINT ["/bin/bash"]