from flask import Flask, jsonify, request
from flask_cors import CORS
from tuya_api import TuyaAPI
import os
import joblib

app = Flask(__name__)
CORS(app)
tuya = TuyaAPI()

MODEL_DIR = os.path.join(os.path.dirname(__file__), 'models')
model_risk_classifier = joblib.load(os.path.join(MODEL_DIR, 'model_risk_classifier.pkl'))
model_waterlevel_30min = joblib.load(os.path.join(MODEL_DIR, 'model_waterlevel_30min.pkl'))
model_waterlevel_60min = joblib.load(os.path.join(MODEL_DIR, 'model_waterlevel_60min.pkl'))

@app.route('/api/devices', methods=['GET'])
def get_devices():
    data = tuya.get_devices()
    return jsonify(data)

@app.route('/api/device/<device_id>/status', methods=['GET'])
def get_device_status(device_id):
    data = tuya.get_device_status(device_id)
    return jsonify(data)

@app.route('/api/predict', methods=['POST'])
def predict():
    data = request.get_json()
    model_type = data.get('type')
    features = data.get('features')
    if not model_type or features is None:
        return jsonify({"error": "Missing type or features"}), 400
    if model_type == 'risk':
        model = model_risk_classifier
    elif model_type == 'waterlevel_30min':
        model = model_waterlevel_30min
    elif model_type == 'waterlevel_60min':
        model = model_waterlevel_60min
    else:
        return jsonify({"error": "Unknown model type"}), 400
    try:
        pred = model.predict([features])
        proba = model.predict_proba([features]).tolist() if hasattr(model, 'predict_proba') else None
        return jsonify({"prediction": pred[0], "proba": proba})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True) 