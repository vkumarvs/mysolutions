import requests
from requests.auth import HTTPBasicAuth
import json
class Users():
def __init__(self, user, password, interval, count):
        self.base = "http://0.0.0.0:12345"
        self.route = "/api/v1/users.json"
        self.url = self.base + self.route
self.user = user
        self.password = password
self.interval = interval
        self.count = count
self.headers = {
            'Accept': 'application/json',
            'Content-type': 'application/json',
            'Cache-Control': 'no-cache',
            }
self.settings = {
                'interval': '{}'.format(self.interval),
                'count':'{}'.format(self.count)
                }
def list(self):
        x = requests.get(
            self.url,
            stream = True,
            params = self.settings,
            auth = HTTPBasicAuth(self.user, self.password),
            headers = self.headers,
)
        return x
