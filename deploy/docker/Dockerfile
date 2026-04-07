FROM alpine:3.23 AS builder

COPY . /nanomq

RUN apk add gcc cmake ninja musl-dev g++

WORKDIR /nanomq/build

RUN cmake -G Ninja -DENABLE_PLUGIN=ON .. && ninja

# Compile plugins in the correct environment
WORKDIR /nanomq/nanomq/plugin
RUN gcc -I.. -I../../nng/include -fPIC -shared \
    plugin_timestamp.c -o plugin_timestamp.so

FROM alpine:3.23

COPY --from=builder /nanomq/build/nanomq/nanomq /usr/local/nanomq/
COPY --from=builder /nanomq/build/nanomq_cli/nanomq_cli /usr/local/nanomq/
COPY --from=builder /nanomq/nanomq/plugin/plugin_timestamp.so /usr/local/nanomq/plugin/
COPY --from=builder /usr/lib/libgcc_s.so.1 /usr/lib/
# COPY etc/nanomq.conf /etc/nanomq.conf
COPY deploy/docker/docker-entrypoint.sh /usr/bin/docker-entrypoint.sh

WORKDIR /usr/local/nanomq

RUN ln -s /usr/local/nanomq/nanomq /usr/bin/nanomq && \
    ln -s /usr/local/nanomq/nanomq_cli /usr/bin/nanomq_cli

EXPOSE 1883

ENTRYPOINT ["/usr/bin/docker-entrypoint.sh"]
