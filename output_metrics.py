#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt
import sys
import numpy as np
from scipy.signal import find_peaks

def analyze_output(input_1,input_2,pop_type):
	try:
		# Read the CSV file into an array
		pop_data1 = np.loadtxt(input_1, delimiter=',', dtype=float)
		pop_data2 = np.loadtxt(input_2, delimiter=',', dtype=float) 
	

	except FileNotFoundError:
		print(f"File '{input_1}' not found.")
	except FileNotFoundError:
		print(f"File '{input_2}' not found.") 
	return pop_data1, pop_data2

def area_under_curve(y_values):
    x_values = np.arange(len(y_values))
    area = np.trapz(y_values, x_values)
    return round(area,2)
