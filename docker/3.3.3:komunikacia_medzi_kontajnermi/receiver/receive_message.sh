#!/bin/bash

while true; do
  nc -l 12345 | /usr/games/cowsay
done
