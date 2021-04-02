#!/bin/bash
g++ client.cpp -lpthread -o client -lrt
g++ server.cpp -lpthread -o server -lrt

