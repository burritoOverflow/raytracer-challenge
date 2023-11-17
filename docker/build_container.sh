#!/usr/bin/env bash

# assumes it's executed directly from this directory and 
# that a build directory "build_docker" does not exist (see Dockerfile)
cd ..

CONTAINER_TOOL=docker
if  command -v podman &> /dev/null
then
    echo "podman found; using podman"
    CONTAINER_TOOL=podman
fi

echo "building with ${CONTAINER_TOOL}"
$CONTAINER_TOOL build -t raytracer-challenge-container -f docker/Dockerfile .
