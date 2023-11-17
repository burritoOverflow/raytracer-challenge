#!/usr/bin/env bash

# the directory "/raytracer-challenge" is already copied to the container, so we'll just mount new one
# however this will require re-running the generator step; best to make container specific directories
# if this approach is to be used
docker run -v ..:/raytracer-challenge.current -it raytracer-challenge-container /bin/bash
