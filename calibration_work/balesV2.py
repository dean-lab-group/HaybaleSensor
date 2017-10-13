import os
import csv
import numpy as np
import matplotlib.pyplot as plt


def fit_data(x, y):
    p = np.polyfit(x, y, 1)
    # A = np.vstack([[x], np.ones(len(x))]).T
    # x = np.array(y)
    # a = np.linalg.lstsq(A, x)
    # c = a[0]
    # print "Polyfit"
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}".format(temp='temp', a=temp_fit[0], b=temp_fit[1])
    # print "Linear Regression"
    # print a
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}*{voltage} + {c:0.5f}".format(voltage='voltage', temp='temp',
    #                                                                                a=c[0], b=c[1], c=c[2])
    # print
    # return c, temp_fit
    return p


def float_array(x):
    float(x)


source_file = 'Sensor_Testing_Data/temperature_calibration_batch2.csv'
delimiter = ','

with open(source_file, 'r') as dest_f:
    data_iter = csv.reader(dest_f,
                           delimiter=delimiter,
                           quotechar='"')
    data = [data for data in data_iter]

# pprint(data)
# print data[2:]
# exit()

sensor_data_dict = {}
for sensor, voltage, temp, temp_freq_sense, moisture, moist_freq_sense, _nothing in data[2:]:
    if sensor not in sensor_data_dict:
        sensor_data_dict[sensor] = dict(voltage=[], temp=[], temp_freq_sense=[], moisture=[], moist_freq_sense=[])
    print sensor, voltage, temp, temp_freq_sense, moisture, moist_freq_sense
    sensor_data_dict[sensor]['voltage'].append(float(voltage))
    sensor_data_dict[sensor]['temp'].append(float(temp))
    sensor_data_dict[sensor]['temp_freq_sense'].append(float(temp_freq_sense))
    sensor_data_dict[sensor]['moisture'].append(float(moisture))
    sensor_data_dict[sensor]['moist_freq_sense'].append(float(moist_freq_sense))

for sensor, data in sensor_data_dict.items():
    print "# Sensor:", sensor

    voltage = np.array(data['voltage'])
    temp_freq_sense = np.array(data['temp_freq_sense'])
    moist_freq_sense = np.array(data['moist_freq_sense'])
    temp = np.array(data['temp'])
    moist = np.array(data['moisture'])
    temp_fit = fit_data(temp_freq_sense, temp)
    temp_estimate = temp_fit[0] * temp_freq_sense + temp_fit[1]

    moist_fit = fit_data(moist_freq_sense, moist)
    moist_estimate = moist_fit[0] * moist_freq_sense + moist_fit[1]

    print "# Temperature"
    # print "{temp} = {a:0.5f} * {temp}_freq + {b:0.5f}".format(temp='temp', a=temp_fit[0], b=temp_fit[1])
    # print "Linear Regression"
    # print "{temp} = {a:0.5f} * {temp}_freq + {c:0.5f}".format(temp='temp', a=c[0], b=c[1], c=c[2])
    print "#define TEMP_FREQ_COEF {a:f}".format(a=temp_fit[0])
    print "#define TEMP_SHIFT {c:f}".format(c=temp_fit[1])
    print
    print "# Moisture"
    # print "{moist} = {a:0.5f} * {moist}_freq + {b:0.5f}".format(moist='moist', a=temp_fit[0], b=temp_fit[1])
    print "#define MOIST_FREQ_COEF {a:f}".format(a=moist_fit[0])
    print "#define MOIST_SHIFT {c:f}".format(c=moist_fit[1])
    print
    print

    # Plot data
    plt.title(sensor)
    plot_margin = 0.20
    plt.plot(temp_freq_sense, temp, 'o', markersize=5, label='Temp Data')
    plt.plot(temp_freq_sense, temp_estimate)#, 'd', markersize=5, label='Temp Approx')
    plt.plot(moist_freq_sense, moist, 'o', markersize=5, label='Moist Data')
    plt.plot(moist_freq_sense, moist_estimate)#, 'd', markersize=5, label='Moist Approx')
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
    plt.savefig(os.path.join('old_images', sensor + '.png'))
    plt.close()
