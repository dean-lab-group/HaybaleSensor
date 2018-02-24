#include "webpage.h"
const char style_css[] = "#container,.name,p{margin:auto}html{font-size:16px;font-family:\"Helvetica Neue\",Helvetica,Arial,serif;font-weight:100}body{display:-webkit-flex;display:-ms-flexbox;display:flex;-webkit-align-items:center;-ms-flex-align:center;align-items:center;-webkit-justify-content:center;-ms-flex-pack:center;justify-content:center;position:absolute;top:0;left:0;right:0;bottom:0;background-color:#363636}#container{text-align:center}.name{font-size:3rem;color:azure}.value{font-size:10rem;color:azure}.units{font-size:2rem;vertical-align:middle;letter-spacing:.35rem}";
const char sensor_js[] = "'use strict';$(document).ready(function(){get_values()});function get_values(){let a=document.getElementById('number'),b=document.getElementById('temp'),c=document.getElementById('moist');$.getJSON('/sensor.json',function(d){a.innerHTML=d.sensor_name,b.innerHTML=d.sensor_temperature,c.innerHTML=d.sensor_moisture})}let refreshId=setInterval(get_values,3e3);";
const char index_html[] = "<!DOCTYPE html><html lang=en><meta charset=UTF-8><title>Haybale Sensor</title><link href=style.css media=screen rel=stylesheet><script src=http://code.jquery.com/jquery-1.10.1.min.js></script><script src=sensor.js></script><div id=container><div class=name><span id=name>Haybale Sensor </span><span id=number>--</span></div><div class=value><p><span class=name>Temp: </span><span class=value id=temp>--</span> <span class=units>°F</span></div><div class=value><p><span class=name>Moist: </span><span class=value id=moist>--</span> <span class=units>%</span></div></div>";
const char sensor_json[] = "{\"sensor_name\": \"%s\", \"sensor_moisture\": \"%0.2f\", \"sensor_temperature\": \"%0.2f\"}";

Page myPages[] = {
     { "/style.css", "text/css", style_css },
     { "/sensor.js", "application/javascript", sensor_js},
     { "/sensor.json", "application/json", sensor_json},
     { "/index.html", "text/html", index_html },
     { nullptr }
};
HaySense hs;
void Main(const char* url, ResponseCallback* cb, void* cbArg, Reader* body, Writer* result, void* reserved){
    Serial.printlnf("handling page %s", url);
    if (strcmp(url,"/index")==0) {
        Serial.println("sending redirect");
        Header h("Location: /index.html\n");
        cb(cbArg, 0, 301, "text/plain", &h);
        return;
    }

    if(strcmp(url, myPages[2].url) == 0){
        cb(cbArg, 0, 200, myPages[2].mime_type, nullptr);
        char json_string[220];
        int ret = sprintf(json_string, myPages[2].data, (const char*)System.deviceID(), hs.get_temperature(), hs.get_moisture());
        result->write(json_string);
        return;
    }


    int8_t idx = 0;
    for (;;idx++) {
        Page& p = myPages[idx];
        if (!p.url) {
            idx = -1;
            break;
        }
        else if (strcmp(url, p.url)==0) {
            break;
        }
    }

    if (idx==-1) {
        cb(cbArg, 0, 404, nullptr, nullptr);
    }
    else {
        cb(cbArg, 0, 200, myPages[idx].mime_type, nullptr);
        result->write(myPages[idx].data);
    }
}
