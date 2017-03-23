import os
import csv
from pprint import pprint

import numpy as np
import matplotlib.pyplot as plt

float_array = lambda x: float(x)
source_file = 'temperature_calibration_batch2.csv'
delimiter = ','

with open(source_file, 'r') as dest_f:
    data_iter = csv.reader(dest_f,
                           delimiter=delimiter,
                           quotechar='"')
    data = [data for data in data_iter]

#pprint(data)
#print data[2:]
#exit()
sensor_data_dict = {}
for sensor, voltage, temp, thsense, moisture, capsense, _nothing in data[2:]:
    if sensor not in sensor_data_dict:
        sensor_data_dict[sensor] = dict(voltage=[], temp=[], thsense=[], moisture=[], capsense=[])
    print sensor, voltage, temp, thsense, moisture, capsense
    sensor_data_dict[sensor]['voltage'].append(float_array(voltage))
    sensor_data_dict[sensor]['temp'].append(float_array(temp))
    sensor_data_dict[sensor]['thsense'].append(float_array(thsense))
    sensor_data_dict[sensor]['moisture'].append(float_array(moisture))
    sensor_data_dict[sensor]['capsense'].append(float_array(capsense))


def fit_data(x, y):
    p = np.polyfit(x, y, 1)
    #A = np.vstack([[x], np.ones(len(x))]).T
    #x = np.array(y)
    #a = np.linalg.lstsq(A, x)
    #c = a[0]
    # print "Polyfit"
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}".format(temp='temp', a=p[0], b=p[1])
    # print "Linear Regression"
    # print a
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}*{voltage} + {c:0.5f}".format(voltage='voltage', temp='temp',
    #                                                                                a=c[0], b=c[1], c=c[2])
    # print
    #return c, p
    return p


for sensor, data in sensor_data_dict.items():
    print "Sensor:", sensor
    temp = data['temp']
    voltage = data['voltage']
    capsense = data['capsense']
    thsense = data['thsense']
    moist = data['moisture']

    print "Temperature"
    p = fit_data(thsense, temp)
    print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}".format(temp='temp', a=p[0], b=p[1])
    #print "Linear Regression"
    #print "{temp} = {a:0.5f} * {temp}_freq + {c:0.5f}".format(temp='temp', a=c[0], b=c[1], c=c[2])
    print
    print "#define TEMP_FREQ_COEF {a:f}".format(a=p[0])
    #print "#define VOLT_COEF {b:f}".format(b=p[1])
    print "#define TEMP_SHIFT {c:f}".format(c=p[1])
    print
    print

    thsense = np.array(thsense)
    voltage = np.array(voltage)
    temp_estimate = p[0] * thsense + p[1] #* voltage + c[2]

    plt.title(sensor)
    plot_margin = 0.20
    plt.plot(thsense, temp, 'o', markersize=5, label='Empirical Data')
    plt.plot(thsense, temp_estimate, 'd', markersize=5, label='Linear Approximation')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Temperature (C)')

    x0, x1, y0, y1 = plt.axis()
    plt.axis((x0 - x0 * plot_margin,
              x1 + x1 * plot_margin,
              y0 - y0 * plot_margin,
              y1 + y1 * plot_margin))
    plt.legend()
    plt.grid()
    # plt.show()
    plt.savefig(os.path.join('images', sensor + '.png'))
    plt.close()

    print "Moisture"
    p = fit_data(capsense, moist)
    print "{moist} = {a:0.5f} * {moist}_freq + {b:0.5f}".format(moist='moist', a=p[0], b=p[1])
    # print "Linear Regression"
    # print "{moist} = {a:0.5f} * {moist}_freq + {c:0.5f}".format(moist='moist', a=c[0], b=c[1], c=c[2])
    print
    print "#define MOIST_FREQ_COEF {a:f}".format(a=p[0])
    # print "#define VOLT_COEF {b:f}".format(b=p[1])
    print "#define MOIST_SHIFT {c:f}".format(c=p[1])
    print
    print