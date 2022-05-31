FROM amd64/ubuntu:18.04 AS base

#If you found this docker image helpful please donate PRCO to the maintainer
LABEL maintainer="RV9zdNeUTQUToZUcRp9uNF8gwH5LzDFtan"

EXPOSE 8766/tcp
EXPOSE 8767/tcp

ENV DEBIAN_FRONTEND=noninteractive

#Add ppa:bitcoin/bitcoin repository so we can install libdb4.8 libdb4.8++
RUN apt-get update && \
	apt-get install -y software-properties-common && \
	add-apt-repository ppa:bitcoin/bitcoin

#Install runtime dependencies
RUN apt-get update && \
	apt-get install -y --no-install-recommends \
	bash net-tools libminiupnpc10 \
	libevent-2.1 libevent-pthreads-2.1 \
	libdb4.8 libdb4.8++ \
	libboost-system1.65 libboost-filesystem1.65 libboost-chrono1.65 \
	libboost-program-options1.65 libboost-thread1.65 \
	libzmq5 && \
	apt-get clean

FROM base AS build

#Install build dependencies
RUN apt-get update && \
	apt-get install -y --no-install-recommends \
	bash net-tools build-essential libtool autotools-dev automake \
	pkg-config libssl-dev libevent-dev bsdmainutils python3 \
	libboost-system1.65-dev libboost-filesystem1.65-dev libboost-chrono1.65-dev \
	libboost-program-options1.65-dev libboost-test1.65-dev libboost-thread1.65-dev \
	libzmq3-dev libminiupnpc-dev libdb4.8-dev libdb4.8++-dev && \
	apt-get clean

#Build ProcyonCoin from source
COPY . /home/procyon/build/ProcyonCoin/
WORKDIR /home/procyon/build/ProcyonCoin
RUN ./autogen.sh && ./configure --disable-tests --with-gui=no && make

FROM base AS final

#Add our service account user
RUN useradd -ms /bin/bash procyon && \
	mkdir /var/lib/procyon && \
	chown procyon:procyon /var/lib/procyon && \
	ln -s /var/lib/procyon /home/procyon/.procyon && \
	chown -h procyon:procyon /home/procyon/.procyon

VOLUME /var/lib/procyon

#Copy the compiled binaries from the build
COPY --from=build /home/procyon/build/ProcyonCoin/src/procyond /usr/local/bin/procyond
COPY --from=build /home/procyon/build/ProcyonCoin/src/procyon-cli /usr/local/bin/procyon-cli

WORKDIR /home/procyon
USER procyon

CMD /usr/local/bin/procyond -datadir=/var/lib/procyon -printtoconsole -onlynet=ipv4
