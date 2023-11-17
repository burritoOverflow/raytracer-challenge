#!/usr/bin/env bash

# NOTE: expectation is that this script is executed directly from this directory (see the hardcoded paths)
# the directory "/raytracer-challenge" is already copied to the container, so we'll just mount new one
# however this will require re-running the generator step; best to make container specific directories
# if this approach is to be used
cd ..
docker run -v `pwd`:/raytracer-challenge.current -it raytracer-challenge-container /bin/bash
