from flask import Flask, request
import json
import pymysql
from pymysql.cursors import DictCursor

app = Flask(__name__)
# MySQL 연결 함수


@app.route('/connect', methods=['GET'])
def connect_db():
    try:
        with open('connect_data.json', encoding='utf-8') as f:
            config = json.load(f)
            cd = config['connect_data']
        conn = pymysql.connect(
            host=cd['host'],
            user=cd['user'],
            password=cd['password'],
            db=cd['db'],
            charset=cd.get('charset', 'utf8mb4'),
            cursorclass=DictCursor
        )
        return {'status': 'success', 'message': 'Database connection successful'}, 200
    except Exception as e:
        return {'status': 'error', 'message': str(e)}, 500


app.run(debug=True, host='0.0.0.0', port=5000)
