#!/bin/bash
#Намерете командите на 10-те най-стари процеси в системата.


ps -eo comm= --sort +start_time | head
