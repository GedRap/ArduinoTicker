import urllib2
import json

class BTC_API:
    def __init__(self, symbols, price = "last"):
        self.symbols = symbols
        self.price = price

    def get_prices(self):
        prices = {}

        for symbol in self.symbols:
            prices[symbol] = self.get_price(symbol)

        return prices

    def get_price(self, symbol):
        url = self._get_url(symbol)
        # @TODO add error handling here
        response = urllib2.urlopen(url).read()

        decoder = json.JSONDecoder()
        decoded_response = decoder.decode(response)

        return decoded_response["ticker"][self.price]

    def _get_url(self, symbol):
        return "https://btc-e.com/api/2/" + symbol + "/ticker"