# coding=utf-8
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os


class PlotData(object):
    def __init__(self, input_data=None, csv_file=None):
        self.csv_file = csv_file
        if self.csv_file:
            self.df = pd.read_csv(self.csv_file)
        elif input_data:
            self.df = input_data
        else:
            raise Exception("No data specified!")

        if 'Unnamed: 0' in self.df.columns:
            self.df = self.df.drop('Unnamed: 0', axis=1)

        self.df['DateTime'] = pd.to_datetime(self.df['DateTime'])
        self.df = self.df.set_index('DateTime')

    def plot_calibration(self):
        self.df.plot(grid=True, logy=True, kind='kde')
        plt.show()

    def plot_performance(self, x_real, y_real, y_est):
        if self.csv_file:
            title = os.path.split(self.csv_file)[-1]
            title = os.path.splitext(title)[0]
        else:
            title = 'Sensor'

        plt.title(title)
        plot_margin = 0.20
        plt.plot(x_real, y_real, 'o', markersize=1, label='Temp Data')
        plt.plot(x_real, y_est)  # , 'd', markersize=5, label='Temp Approx')
        plt.xlabel('Frequency (Hz)')
        plt.ylabel('Reading')
        x0, x1, y0, y1 = plt.axis()
        plt.axis((x0 - x0 * plot_margin,
                  x1 + x1 * plot_margin,
                  y0 - y0 * plot_margin,
                  y1 + y1 * plot_margin))
        plt.legend()
        plt.grid()
        plt.savefig(os.path.join('images', title + '_performance.png'))
        plt.close()

    def fit_data(self, x, y):
        p = np.polyfit(x, y, 2)
        return p


if __name__ == '__main__':
    for filename in os.listdir('calibration_v2'):
        p = PlotData(csv_file=os.path.join('calibration_v2/', filename))
        temp_real = p.df.ix[:, 0]
        temp_freq = p.df.ix[:, 1]
        temp_params = p.fit_data(temp_freq, temp_real)
        temp_est = temp_params[0] * temp_freq**2 + temp_params[1] * temp_freq + temp_params[2]
        p.plot_performance(temp_freq, temp_real, temp_est)
        sse = np.sum((temp_real - temp_est)**2)
        print sse
