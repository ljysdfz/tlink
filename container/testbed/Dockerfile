FROM debian:buster-slim

RUN apt-get update && apt-get install -y build-essential git iproute2 tcpdump iptables iputils-ping nano
WORKDIR /iperf
#RUN git clone https://git.code.sf.net/p/iperf2/code iperf2-source
COPY iperf2-source /iperf/iperf2-source
WORKDIR /iperf/iperf2-source
RUN ./configure
RUN make
RUN make install

WORKDIR /root
