FROM ubuntu:18.04
RUN apt-get update -y
RUN apt-get install wget build-essential -y
WORKDIR /opt/
RUN wget https://github.com/ProcyonCoin/ProcyonCoonCoin/releases/download/raccoon-v1.0.3/procyon-1.0.3-Linux64.tar.gz
RUN tar zxvf procyon-1.0.3-Linux64.tar.gz
RUN mv procyoncoin-1.0.3/usr/local/bin/procyon* /usr/bin/
RUN wget https://raw.githubusercontent.com/TheRetroMike/rmt-nomp/master/scripts/blocknotify.c
RUN gcc blocknotify.c -o /usr/bin/blocknotify
CMD /usr/bin/procyond -daemon;tail -f /root/.procyon/debug.log
