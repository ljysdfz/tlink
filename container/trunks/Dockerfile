#FROM trunk_builder
FROM debian:bookworm-slim
WORKDIR /trunks
RUN apt-get update && DEBIAN_FRONTEND=non-interactive apt-get install -qy --no-install-recommends --no-install-suggests \
    iproute2 iptables procps tcpdump iputils-ping nano \
    && rm -rf /var/lib/apt/lists/*
#COPY --from=trunk_builder /go/bin/trunks /usr/local/sbin/trunks
COPY trunks /usr/local/sbin/trunks
ENTRYPOINT [ "trunks" ]
CMD [ "--help" ]
