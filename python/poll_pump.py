import logging
import ssl
import time
import sys
import json
from urllib.request import urlopen
from datetime import datetime


ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

file1 = open('/media/ubuntu/Samsung_SSD/github/home_thermostat/temp_data.csv', 'a')

now = datetime.now()
dt_string = now.strftime("%Y-%m-%d %H:%M:%S")

with urlopen("http://192.168.68.175",context=ctx) as response:
    body = response.read()

response= json.loads(body.decode())
file1.write("{}\n".format(dt_string,response['pump_status']))
file1.close()
