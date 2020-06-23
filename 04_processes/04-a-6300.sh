#!/bin/bash
#Изведете командата на най-стария процес


ps -eo comm=  --sort +start_time | head -1
