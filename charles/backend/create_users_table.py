import pymysql
from flask import Flask, request, jsonify
import json
from pymysql.cursors import DictCursor


def get_connection():

    with open('con_data.json') as f:
        config = json.load(f)
    return pymysql.connect(**config['connect_data'])


def create_user():
    # create cursor
    conn = get_connection()
    # # MySQL 연결 함수

    with conn.cursor() as cursor:
        try:
            # Create table if it does not exist
            sql = """
            CREATE TABLE IF NOT EXISTS users
                (
                user_id INT PRIMARY KEY AUTO_INCREMENT,
                nickname VARCHAR(50) UNIQUE NOT NULL,
                password VARCHAR(255) NOT NULL,
                name VARCHAR(50) NOT NULL,
                age INT,
                email VARCHAR(50)
                )
                """
            cursor.execute("SET NAMES utf8mb4")  # Set character set to utf8mb4
            conn.commit()

            cursor.execute(sql)
            conn.commit()

        except Exception as e:
            print("Error occurred while creating table:", e)

        finally:
            cursor.close()
