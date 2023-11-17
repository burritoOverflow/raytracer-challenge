#!/usr/bin/env bash

# assumes it's executed directly from this directory and 
# that a build directory "build_docker" does not exist (see Dockerfile)
cd ..
docker build -t raytracer-challenge-container -f docker/Dockerfile .
