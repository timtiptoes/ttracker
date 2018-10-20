# Simple example of reading the MCP3008 analog input channels and printing
# them all out.
# Author: Tony DiCola
# License: Public Domain
import time

# Import SPI library (for hardware SPI) and MCP3008 library.
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import time
import numpy as np
from scipy import stats
activities=['doing nothing','walking','running']
from tabulate import tabulate
# Software SPI configuration:
#CLK  = 18
#MISO = 23
#MOSI = 24
#CS   = 25
#mcp = Adafruit_MCP3008.MCP3008(clk=CLK, cs=CS, miso=MISO, mosi=MOSI)

# Hardware SPI configuration:
SPI_PORT   = 0
SPI_DEVICE = 0
mcp = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))

while True:
	print("[name,activity_code,speed] or q")
	value=input()
	if str(value)=="q":
		break
	else:
		name,activity_code,speed=value.split(',')
		if activity_code not in ['1','2','3']:
			print("activity code not recognized")
			break
		print("About to record %s %s at %.1f mph, okay? Y to continue" % (name,activities[int(activity_code)-1],float(speed)))
		b=input()
		if b!="Y":
			break
		data_to_dump=[]
		start = time.time()
		for i in range(1000):
		    # Read all the ADC channel values in a list.
		    values = [0]*6
		    for i in range(6):
		        # The read_adc function will get the value of the specified channel (0-7).
		        values[i] = str(mcp.read_adc(i))
		    # Print the ADC values.
		    #print('| {0:>4} | {1:>4} | {2:>4} | {3:>4} | {4:>4} | {5:>4} | {6:>4} | {7:>4} |'.format(*values))
		    # Pause for half a second.
		    time.sleep(0.009)
		    data_to_dump.append(values)
		done=time.time()
		elapsed=done-start
		print("That took %.4f seconds which is %.4f Hz" %(elapsed,6000.0/float(elapsed)))
		f=open('datafile.csv','a')
		for data_row in data_to_dump:
			f.write(",".join([name,str(activity_code),str(speed)]+data_row)+"\n")
		f.close()
		
		b=np.array([np.array(xi) for xi in data_to_dump]).astype(np.float)
		desc = stats.describe(b)

		c=[[str(i),desc.mean[i],desc.variance[i],desc.minmax[0][i],desc.minmax[1][i]] for i in range(len(desc.mean))]
		
		print(tabulate(c, headers=['channel','mean','variance','min','max'], tablefmt='orgtbl'))
