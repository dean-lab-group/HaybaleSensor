import os
import csv
import numpy as np
import matplotlib.pyplot as plt

float_array = lambda x: float(x)
dest_file = 'sensor_data.csv'
delimiter = ','

with open(dest_file, 'r') as dest_f:
    data_iter = csv.reader(dest_f,
                           delimiter=delimiter,
                           quotechar='"')
    data = [data for data in data_iter]

# data_array = np.asarray(data)
# pprint(data_array)
boo = {}
for temp, voltage, sensor, capsense, thsense, bv in data[1:]:
    if sensor not in boo:
        boo[sensor] = dict(temp=[], voltage=[], capsense=[], thsense=[], BV=[])
    boo[sensor]['temp'].append(float_array(temp))
    boo[sensor]['voltage'].append(float_array(voltage))
    boo[sensor]['capsense'].append(float_array(capsense))
    boo[sensor]['thsense'].append(float_array(thsense))
    boo[sensor]['BV'].append(float_array(bv))


def fit_data(sensor, voltage, expected):
    p = np.polyfit(sensor, expected, 1)
    A = np.vstack([[sensor, voltage], np.ones(len(sensor))]).T
    x = np.array(expected)
    a = np.linalg.lstsq(A, x)
    c = a[0]
    # print "Polyfit"
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}".format(temp='temp', a=p[0], b=p[1])
    # print "Linear Regression"
    # print a
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}*{voltage} + {c:0.5f}".format(voltage='voltage', temp='temp',
    #                                                                                a=c[0], b=c[1], c=c[2])
    # print
    return c, p


for sensor, data in boo.items():
    print "Sensor:", sensor
    temp = data['temp']
    voltage = data['BV']
    capsense = data['capsense']
    thsense = data['thsense']

    print "Calibration:"
    # p = np.polyfit(thsense, temp, 1)
    # A = np.vstack([[thsense, voltage], np.ones(len(thsense))]).T
    # x = np.array(temp)
    # a = np.linalg.lstsq(A, x)
    # c = a[0]
    print "Temperature"
    c, p = fit_data(thsense, voltage, temp)
    print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}".format(temp='temp', a=p[0], b=p[1])
    print "Linear Regression"
    print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}*{voltage} + {c:0.5f}".format(voltage='voltage', temp='temp', a=c[0], b=c[1], c=c[2])
    print
    print "#define TEMP_FREQ_COEF {a:f}".format(a=c[0])
    print "#define VOLT_COEF {b:f}".format(b=c[1])
    print "#define TEMP_SHIFT {c:f}".format(c=c[2])
    print
    print

    thsense = np.array(thsense)
    voltage = np.array(voltage)
    temp_estimate = c[0]*thsense + c[1]*voltage + c[2]


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
    plt.show()
    plt.imsave(os.path.join('images', sensor + '.png'))
    #print "Moisture"
    #c, p = fit_data(capsense, voltage, moist)
