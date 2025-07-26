import requests
import time
import hashlib
import hmac
import json
import os
import random

TUYA_CLIENT_ID = "m8q3tttyrtaqvt5umueu"
TUYA_SECRET = "b874d4d2dcd14381b1b785d38273bc4c"
TUYA_API_ENDPOINT = "https://openapi.tuyaus.com"

FAKE_MODE = os.environ.get('TUYA_FAKE_MODE', '0') == '1'

class TuyaAPI:
    def __init__(self):
        self.access_token = None
        self.token_expire = 0

    def _get_headers(self, path, method="GET", body=""):
        t = str(int(time.time() * 1000))
        if not self.access_token or self.token_expire < time.time():
            self._refresh_token()
        sign_str = TUYA_CLIENT_ID + self.access_token + t
        if method == "POST" and body:
            sign_str += body
        sign = hmac.new(
            TUYA_SECRET.encode("utf-8"),
            msg=sign_str.encode("utf-8"),
            digestmod=hashlib.sha256
        ).hexdigest().upper()
        headers = {
            "client_id": TUYA_CLIENT_ID,
            "access_token": self.access_token,
            "sign": sign,
            "t": t,
            "sign_method": "HMAC-SHA256",
            "Content-Type": "application/json"
        }
        return headers

    def _refresh_token(self):
        if FAKE_MODE:
            # 假模式下伪造 token
            self.access_token = "fake_token"
            self.token_expire = time.time() + 3600
            return
        t = str(int(time.time() * 1000))
        sign_str = TUYA_CLIENT_ID + t
        sign = hmac.new(
            TUYA_SECRET.encode("utf-8"),
            msg=sign_str.encode("utf-8"),
            digestmod=hashlib.sha256
        ).hexdigest().upper()
        headers = {
            "client_id": TUYA_CLIENT_ID,
            "sign": sign,
            "t": t,
            "sign_method": "HMAC-SHA256"
        }
        url = f"{TUYA_API_ENDPOINT}/v1.0/token?grant_type=1"
        resp = requests.get(url, headers=headers)
        data = resp.json()
        self.access_token = data["result"]["access_token"]
        self.token_expire = time.time() + data["result"]["expire_time"] / 1000 - 60

    def get_devices(self):
        if FAKE_MODE:
            time.sleep(random.uniform(0.2, 0.6))  # 增加延迟
            if random.random() < 0.05:
                
                return {"code": 500, "msg": "Internal error", "result": None}
          
            return {
                "code": 0,
                "msg": "success",
                "result": {
                    "total": 2,
                    "devices": [
                        {"id": "dev123", "name": "水泵1号", "status": "online", "location": "A区"},
                        {"id": "dev456", "name": "水泵2号", "status": "offline", "location": "B区"}
                    ]
                }
            }
        url = f"{TUYA_API_ENDPOINT}/v1.3/iot-03/devices"
        headers = self._get_headers("/v1.3/iot-03/devices")
        resp = requests.get(url, headers=headers)
        return resp.json()

    def get_device_status(self, device_id):
        if FAKE_MODE:
            time.sleep(random.uniform(0.1, 0.4))
            if random.random() < 0.05:
                return {"code": 404, "msg": "Device not found", "result": None}
            return {
                "code": 0,
                "msg": "success",
                "result": {
                    "id": device_id,
                    "status": random.choice(["online", "offline"]),
                    "last_update": int(time.time())
                }
            }
        url = f"{TUYA_API_ENDPOINT}/v1.3/iot-03/devices/{device_id}/status"
        headers = self._get_headers(f"/v1.3/iot-03/devices/{device_id}/status")
        resp = requests.get(url, headers=headers)
        return resp.json()

    # 可扩展更多接口 