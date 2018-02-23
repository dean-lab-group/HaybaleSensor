"use strict";

$(document).ready(
    function (){
        get_values();
    }
);


function get_values() {
    let number = document.getElementById('number');
    let temp = document.getElementById('temp');
    let moist = document.getElementById('moist');
    $.getJSON('/sensor.json', function (info) {
        /** @namespace info.sensor_name */
        number.innerHTML = info.sensor_name;
        /** @namespace info.sensor_temperature */
        temp.innerHTML = info.sensor_temperature;
        /** @namespace info.sensor_moisture */
        moist.innerHTML = info.sensor_moisture;

    });
}

let refreshId;
refreshId = setInterval(get_values, 3000);
