import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import numpy
import time

SPI_PORT   = 0
SPI_DEVICE = 0

class Surk():

  def __init__(self):
    self.mcp = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))

  def sample_one_second(self):
    values = numpy.array([[0.0]]*100)
    for i in range(100):
      values[i][0] = self.mcp.read_adc(0)
      time.sleep(0.01)
    return values