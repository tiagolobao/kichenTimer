FROM alpine:3.18

# Based on https://github.com/lpodkalicki/avr-toolchain-docker
# for more info, check its README

LABEL maintainer="Tiago Lobao <tiago.blobao@gmail.com>"

# Prepare directory for tools
ARG TOOLS_PATH=/tools
RUN mkdir ${TOOLS_PATH}
WORKDIR ${TOOLS_PATH}

RUN apk --no-cache add ca-certificates wget make avrdude avr-libc gcc-avr gcc

WORKDIR /build
