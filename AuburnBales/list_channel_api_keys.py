import requests

url = "https://api.thingspeak.com/channels.json"

querystring = {"api_key":"W9BIUVUSQ75EWU21"}

headers = {
    'Cache-Control': "no-cache",
    'Postman-Token': "93ce35f7-0d49-4eaa-b3fb-5804f3013a9e"
    }

response = requests.request("GET", url, headers=headers, params=querystring).json()

for channel in sorted(response, key=lambda x: x['name']):
    print "//", channel['name']
    print "String api_key = \"" + str([x['api_key'] for x in channel['api_keys'] if x['write_flag'] is True][0]) + "\";"
    print
