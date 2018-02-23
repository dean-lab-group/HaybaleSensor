import serial
from time import time, sleep
import pandas as pd
from datetime import datetime
from alerts import Alerts
import settings

def dump_the_pandas_data(data):
    headers = ['DateTime', 'Temperature', 'SensorTemp', 'SensorMoist']
    date_format = '%Y%m%d-%H%M%S'
    datestr = datetime.now().strftime(date_format)
    f = open('calibration_v2/HB_{DATE}.csv'.format(DATE=datestr), 'wb')
    df = pd.DataFrame(columns=headers, data=data)
    df['DateTime'] = pd.to_datetime(df['DateTime'])
    df.set_index('DateTime')
    df.to_csv(f)
    f.close()


def init_serial(serial_path, *args, **kwargs):
    print time(), "Initializing Serial to ", serial_path
    try:
        s = serial.Serial(serial_path, *args, **kwargs)
        s.write('\n\r')
        return s
    except serial.serialutil.SerialException as e:
        raise Exception("Serial port" + serial_path + " not available.")

output_data = []

thermocouple = "/dev/tty.usbserial-FTG96HDJ"
particle = '/dev/tty.usbmodem1411'


def read_from_serial():
    temp = init_serial(thermocouple, baudrate=115200)
    part = init_serial(particle, baudrate=115200)
    start = datetime.now()
    while True:
        try:
            dt = datetime.now()
            temp_output = temp.readline().strip()
            temp_c = temp_output
            particle_output = part.readline().strip()
            temp_hz, moist_hz = particle_output.split(', ')
            if temp_c and temp_hz and moist_hz:
                row = (dt, float(temp_c), float(temp_hz), float(moist_hz))
                print "%s, %f, %f, %f" % row
                output_data.append(row)
                if float(temp_c) < 23.0:
                    dump_the_pandas_data(output_data)
                    print "Completed Calibration"
                    return start

        except ValueError:
            pass
        except KeyboardInterrupt as e:
            print "Error:", e
            return start
        except Exception as e:
            dump_the_pandas_data(output_data)
            return start


if __name__ == '__main__':
    start = read_from_serial()
    elapsed = datetime.now() - start
    alert = Alerts(api_keys=settings.pushover_keys)
    msg = "Calibration program done. Time to complete: " + str(elapsed)
    print(msg)
    alert.send_pushover(msg)
    exit()
