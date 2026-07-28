#!/bin/bash
for d in 2*; do python3 cnGraph.py "$d"; done
for d in 2*; do python3 posGraph.py "$d"; done
