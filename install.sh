#!/bin/bash

git clone https://github.com/ikalnytskyi/termcolor
mv termcolor/include ./
rm -r termcolor
mv include/termcolor ./
rm -r include
