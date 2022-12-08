import re
from signal import signal
from flask import Flask, request, jsonify
from app import app
from flask_cors import CORS
import pymongo
import json
from pymongo import MongoClient
from bson.objectid import ObjectId
from bson.errors import InvalidId
from datetime import datetime, timedelta, timezone
from bson import json_util
import os
import time


CORS(app, supports_credentials=True, origins='*')
# while running docker compose
# app.config["MONGO_URI"] = 'mongodb://' + os.environ['MONGODB_USERNAME'] + \
# ':' + os.environ['MONGODB_PASSWORD'] + '@' + \
# os.environ['MONGODB_HOSTNAME'] + ':27017'

app.config['MONGO_URI'] = 'mongodb://kirthi:kirthi@localhost:27017'

client = pymongo.MongoClient(app.config["MONGO_URI"])

db = client['kirthidb']
# db = client[os.environ["MONGODB_DATABASE"]]
users = db.users

SIGNALS = {
    'BAD_REQUEST': 400,
    'UNAUTHORIZED': 401,
    'FORBIDDEN': 403,
    'NOT_FOUND': 404,
    'METHOD_NOT_ALLOWED': 405,
    'NOT_ACCEPTABLE': 406,
    'REQUEST_TIMEOUT': 408,
    'CONFLICT': 409,
    'GONE': 410,
    'OK': 200,
    'INTERNAL_SERVER_ERROR': 500,
}


@ app.route("/iot", methods=['POST'])
def func():
    try:
        mac = request.form['mac']
        request_time = request.form['time']
        # creates datetime object from string
        signal = request.form['signal']
        device = request.form['device']
        action = request.form['action']
        request_time = datetime.strptime(request_time, '%Y-%m-%d %H:%M:%S')

        result = db.data.insert_one({'mac': mac, 'time': request_time,
                                     'signal': signal, 'device': device, 'action': action})
        # wait for 1 second to populate the database
        time.sleep(1)
        # load the data inserted in last one second from same mac
        data = db.data.find(filter={'time': {
            # request_time -1 to request_time+1
            '$gte': request_time - timedelta(seconds=0, milliseconds=500)
        }, 'mac': mac}, sort=[
            ('signal', pymongo.ASCENDING),
            ('time', pymongo.ASCENDING)])

        data = list(data)
        print("DATA:", data)

        if data[0]['_id'] == result.inserted_id:
            return jsonify({"action": action})
        else:
            return jsonify({"action": "no action"})
    except Exception as e:
        return jsonify({"error": str(e)}), SIGNALS['BAD_REQUEST']


@ app.route("/", methods=['POST'])
def insertData():
    data = request.form['data']

    print("Data", data)
    return jsonify({"status": "success", "message": "data received", "data": data}), SIGNALS['OK']


@app.route("/hi")
def index():
    return {"message": "Welcome to the Dockerized Flask MongoDB API!"}, SIGNALS['OK']
