#!/bin/bash

git clone https://github.com/ikalnytskyi/termcolor
mv include ./
rm -r termcolor
mv include/termcolor ./
rm -r include
