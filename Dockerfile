FROM ubuntu:18.04
RUN apt-get update -y
RUN apt install wget build-essential libssl-dev libboost-chrono-dev libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-thread-dev libboost-test-dev qtbase5-dev qttools5-dev bison libexpat1-dev libdbus-1-dev libfontconfig-dev libfreetype6-dev libice-dev libsm-dev libx11-dev libxau-dev libxext-dev libevent-dev libxcb1-dev libxkbcommon-dev libminiupnpc-dev libprotobuf-dev libqrencode-dev xcb-proto x11proto-xext-dev x11proto-dev xtrans-dev zlib1g-dev libczmq-dev autoconf automake libtool protobuf-compiler -y
WORKDIR /opt/
RUN wget https://github.com/ProcyonCoin/ProcyonCoonCoin/releases/download/raccoon-v1.0.3/procyon-1.0.3-Linux64.tar.gz
RUN tar zxvf procyon-1.0.3-Linux64.tar.gz
RUN mv procyoncoin-1.0.3/usr/local/bin/procyon* /usr/bin/
RUN wget https://raw.githubusercontent.com/TheRetroMike/rmt-nomp/master/scripts/blocknotify.c
RUN gcc blocknotify.c -o /usr/bin/blocknotify
CMD /usr/bin/procyond -daemon;tail -f /root/.procyon/debug.log
