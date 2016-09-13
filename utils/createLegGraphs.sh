#!/bin/bash

NUMLEGS=24
while [ $NUMLEGS -gt 0 ] 
do
    python LegGraph.py $NUMLEGS 100 > ~/dev/QB/implied_engine/src/sandbox/graph_exs/CL_${NUMLEGS}.dat
    NUMLEGS=$(expr $NUMLEGS - 1)
done