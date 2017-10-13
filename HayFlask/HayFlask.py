import json
from flask import Flask, render_template

app = Flask(__name__)


@app.route('/')
def root():
    return render_template('index.html')


@app.route('/hay/sensor.json')
def hay():
    return json.dumps(
        dict(
            sensor_name='1',
            sensor_temperature=98.5,
            sensor_moisture=81.4
        ))


app.run(port=8080)
