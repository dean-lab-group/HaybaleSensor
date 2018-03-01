import json
path = '../../sensor_inventory.csv'
lines = open(path).readlines()
for line in lines:
    Name, ID, t1, t2, m1, m2, m3, m4 = [x.strip() for x in line.strip().split(',')]
    try:
        num = int(Name.split('HB_')[1])
        #print Name, ID, t1, t2, m1, m2, m3, m4
        a = { "id": ID, "sensor_num": num, "temp": [float(t1), float(t2)], "y0": [float(m1), float(m2)], "y100": [float(m3), float(m4)]}
        fh = open("calibrate/"+ID+".json", "w")
        fh.write(json.dumps(a))
        fh.close()
    except:
        continue
    
