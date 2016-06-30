#!/usr/bin/env bash
python ./RMC_vasp.py ../REMARC/input/Fe2O3
python ./RMC_rate.py output_surface.txt
