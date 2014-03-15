from BTC_API import BTC_API

class ArduinoTicker:
    def __init__(self, symbols, api, price_type = "last", avg_size = 3):
        self.symbols = symbols

        self.last_values = {}
        self.avg_size = avg_size

        for symbol in symbols:
            self.last_values[symbol] = []

        self.api = api

    def tick(self):
        result = {}

        for symbol in self.symbols:
            price = self.api.get_price(symbol)
            print symbol + ", " + str(price)
            self.last_values[symbol].append(price)
            if len(self.last_values[symbol]) > self.avg_size:
                del self.last_values[symbol][0]

            first_value = self.last_values[symbol][0]
            last_value = self.last_values[symbol][-1]

            change = 0;

            if first_value < last_value:
                change = 1

            if first_value > last_value:
                change = -1

            result[symbol] = {'price': price, 'change': change}

        return result
