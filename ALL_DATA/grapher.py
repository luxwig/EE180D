import csv
import matplotlib.pyplot as plt
import numpy as np
import itertools
import statistics

time = []
accel_x = []
accel_y = []
accel_z = []
gyro_x = []
gyro_y = []
gyro_z = []

def switch(x):
    return {
        'accel_x': accel_x,
        'accel_y': accel_y,
        'accel_z': accel_z,
        'gyro_x' : gyro_x,
        'gyro_y' : gyro_y,
        'gyro_z' : gyro_z
    }[x]

def getAllData(file):
	with open(file) as csvfile:
		info = []
		reader = csv.DictReader(csvfile)
		for row in reader:
			for key in row:
				row[key] = float(row[key])
			info.append(row) 
		return info

def collect(data):
	for el in data:
		time.append(el['timestamp_before'])
		accel_x.append(el[' accel_x'])
		accel_y.append(el[' accel_y'])
		accel_z.append(el[' accel_z'])
		gyro_x.append(el[' gyro_x'])
		gyro_y.append(el[' gyro_y'])
		gyro_z.append(el[' gyro_z'])

#fields array of fields you wanna plot
def graph(fields):
	number = len(fields);
	for n in range(number):
		plt.subplot(number, 1, n+1)
		plt.plot(time, switch(fields[n]))
	plt.show()


#data = getAllData("./DESCEND/1.csv")
data = getAllData("./WALK/ludwig/1.csv")


collect(data)
graph(['accel_x', 'accel_y', 'accel_z', 'gyro_x', 'gyro_y', 'gyro_z'])
#graph('gyro_x')
