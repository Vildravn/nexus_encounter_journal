FROM ubuntu:24.04 AS builder
RUN apt-get update && apt-get install -y \
    cmake \
    mingw-w64 \
    && rm -rf /var/lib/apt/lists/*

FROM builder as build-stage
WORKDIR /addon
COPY . .
RUN mkdir build
WORKDIR /addon/build
RUN cmake -DCMAKE_BUILD_TYPE:STRING=MinSizeRel ..
RUN cmake --build . --config MinSizeRel --target all --

FROM scratch AS export-stage
COPY --from=build-stage /addon/build/libnexus_encounter_journal.dll ./nexus_encounter_journal.dll