#!/bin/bash

/home/cs2521/bin/3c -o textbuffer.o -c textbuffer.c
/home/cs2521/bin/3c -o test_textbuffer.o -c test_textbuffer.c
/home/cs2521/bin/3c +leak test_textbuffer.o textbuffer.o   -o test_textbuffer

