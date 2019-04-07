#!/bin/bash

# Will remove all docker containers and images
docker container prune
docker image prune -a
