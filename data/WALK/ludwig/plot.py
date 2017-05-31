from matplotlib import pyplot as plt
from csv import reader
import numpy as np

with open('1.csv', 'r') as f:
	data_run = list(reader(f))
	
accel_x = [i[2] for i in data_run[2::]]
accel_y = [i[3] for i in data_run[2::]]
accel_z = [i[4] for i in data_run[2::]]
gyro_x = [i[5] for i in data_run[2::]]
gyro_y = [i[6] for i in data_run[2::]]
gyro_z = [i[7] for i in data_run[2::]]
time = [i[0] for i in data_run[2::]]

fig1 = plt.figure(1)

plt.subplot(231)
plt.plot(range(len(gyro_z)), gyro_x, linewidth=0.5)
plt.title('Gyro X vs Time')

plt.subplot(232)
plt.plot(range(len(gyro_y)), gyro_y, linewidth=0.5)
plt.title('Gyro Y vs Time')

plt.subplot(233)
plt.plot(range(len(gyro_z)), gyro_z, linewidth=0.5)
plt.title('Gyro Z vs Time')

plt.subplot(234)
plt.plot(range(len(accel_x)), accel_x, linewidth=0.5)
plt.title('Accel X vs Time')

plt.subplot(235)
plt.plot(range(len(accel_y)), accel_y, linewidth=0.5)
plt.title('Accel Y vs Time')

plt.subplot(236)
plt.plot(range(len(accel_z)), accel_z, linewidth=0.5)
plt.title('Accel Z vs Time')
fig1.suptitle('Walk')

plt.show()
