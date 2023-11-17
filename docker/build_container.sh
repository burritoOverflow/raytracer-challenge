#!/usr/bin/env bash

cd ..
docker build -t raytracer-challenge-container -f docker/Dockerfile .
