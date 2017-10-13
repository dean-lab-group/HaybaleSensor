$(document).ready(
    function (){
        get_values();
    }
);


function get_values() {
    var number = document.getElementById('number');
    var temp = document.getElementById('temp');
    var moist = document.getElementById('moist');
    $.getJSON('/hay/sensor.json', function (info) {
        number.innerHTML = info.sensor_name;
        temp.innerHTML = info.sensor_temperature;
        moist.innerHTML = info.sensor_moisture;

    });
}

var refreshId;
refreshId = setInterval(get_values, 3000);
