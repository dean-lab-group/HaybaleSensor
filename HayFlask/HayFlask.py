import json
from random import random

from flask import Flask, render_template

app = Flask(__name__)


@app.route('/')
def root():
    return render_template('index.html')


@app.route('/sensor.json')
def hay():
    json_stuff = json.dumps(
        dict(
            sensor_name='1',
            sensor_temperature="%0.3f" % random(),
            sensor_moisture="%0.3f" % random()
        ))
    print json_stuff
    return json_stuff


app.run(host='0.0.0.0', port=8080)
