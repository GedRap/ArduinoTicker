from ArduinoTicker.BTC_API import BTC_API
from ArduinoTicker.ArduinoTicker import ArduinoTicker
import time

symbols = ["btc_usd", "ltc_usd"]

api = BTC_API(symbols)
ticker = ArduinoTicker(symbols, api)

while True:
    print ticker.tick()
    time.sleep(120)