#!/usr/bin/env python3

from influxdb_client import InfluxDBClient
import pandas as pd
import sys

my_token = "wXrijWpcGjt_xjBitTOXPUOqXAxJXtfnanjIOiHklGMiEUNFZxhbeSoh-AtUt3tHLqnCz31XuwIoVhNf14CRqg=="
my_org = "StudHSKe"
bucket = "prosit"
client = InfluxDBClient(url="http://127.0.0.1:8086", token=my_token, org=my_org, debug=False)

print("write data in file...")
# print(f'arg1: {sys.argv[1]}')

query= f'''
from(bucket: "{bucket}")
|> range(start:-1000y, )
|> filter(fn: (r) => r._measurement == "{sys.argv[1].split('%')[0]}" and r._field != "Modus")'''

data = client.query_api().query_data_frame(org=my_org, query=query)
# print(data.head())

# Drop the unneeded collumns
data = data.drop('result', 1).drop('_start', 1).drop('_stop',1).drop('_measurement',1).drop('table',1)

data = data.set_index('_time')
data = data.sort_index()

data = data[['sensor', '_field', '_value']] # Rearrange the collumns

measurementIdentifier = sys.argv[1].split('%')[0]
measurementDescription = sys.argv[1].split('%')[1]

data.to_csv(f'/home/pi/Nextcloud/Mit_mir_geteilt/PROSIT/Messungen/{measurementIdentifier}-{measurementDescription}.csv')