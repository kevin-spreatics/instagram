from flask import Flask, request, jsonify
import json
import traceback
import pymysql
import bcrypt


# # 사용자 생성

app = Flask(__name__)


def get_connection():
    with open('connect_data.json', encoding='utf-8') as f:
        config = json.load(f)
        cd = config['connect_data']
    return pymysql.connect(
        host=cd['host'],
        user=cd['user'],
        password=cd['password'],
        db=cd['db'],
        charset=cd.get('charset', 'utf8mb4'),
        cursorclass=pymysql.cursors.DictCursor    # ← 클래스 객체로 지정
    )


@app.route('/users', methods=['POST'])
def create_user():
    # 1. Content-Type 체크 (클라이언트가 application/json 으로 보냈는지)
    if not request.is_json:
        return jsonify({'error': 'Content-Type must be application/json'}), 415

    data = request.get_json()
    nickname = data.get('nickname')
    name = data.get('name')
    password = data.get('password')
    age = data.get('age')    # optional
    email = data.get('email')  # optional

    if not nickname or not name or not password:
        return jsonify({'error': 'Nickname, name, and password are required.'}), 400

    # 4. 비밀번호 해싱
    hashed_pw = bcrypt.hashpw(password.encode(
        'utf-8'), bcrypt.gensalt()).decode('utf-8')

    # 5. DB에 저장
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            sql = """
            INSERT INTO users (nickname, name, password, age, email)
            VALUES (%s, %s, %s, %s, %s)
            """
            cursor.execute(sql, (nickname, name, hashed_pw, age, email))
        conn.commit()
        user_id = cursor.lastrowid

    except pymysql.err.IntegrityError as ie:
        # 닉네임 중복 등 UNIQUE 위반
        return jsonify({'error': 'Nickname already exists.'}), 409

    except Exception as e:
        # 전체 스택 트레이스 문자열로 가져오기
        tb = traceback.format_exc()
        return jsonify({
            'error': str(e),
            'trace': tb.splitlines()
        }), 500

    finally:
        conn.close()

    # 6. 생성 성공 응답
    return jsonify({
        'status':   'created',
        'user_id':  user_id,
        'nickname': nickname
    }), 201


# # 사용자 인증 (로그인)

@app.route('/users/login', methods=['POST'])
def login_user():
    if not request.is_json:
        return jsonify({'error': 'Content-Type must be application/json'}), 415

    data = request.get_json()
    nickname = data.get('nickname')
    password = data.get('password')

    if not nickname or not password:
        return jsonify({'error': 'Nickname and password are required.'}), 400

    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute(
                "SELECT user_id, password FROM users WHERE nickname = %s",
                (nickname,)
            )
            user = cursor.fetchone()

        if not user:
            return jsonify({'status': 'denied', 'reason': 'Invalid nickname or password.'}), 401

        # DB에 저장된 해시 (문자열) → 바이트로 변환
        stored_hash = user['password'].encode('utf-8')
        # 입력한 비밀번호(str) → 바이트
        raw_pw_bytes = password.encode('utf-8')

        if not bcrypt.checkpw(raw_pw_bytes, stored_hash):
            return jsonify({'status': 'denied', 'reason': 'Invalid nickname or password.'}), 401

        return jsonify({'status': 'success', 'user_id': user['user_id']}), 200

    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500

    finally:
        conn.close()

# # 사용자 정보 조회


@app.route('/users/<int:user_no>', methods=['GET'])
def get_user(user_no):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            sql = """
            SELECT user_id, nickname, name, age, email 
            FROM users 
            WHERE user_id = %s
            """

            cursor.execute(sql, (user_no,))
            user = cursor.fetchone()

            if not user:
                return jsonify({'status': 'failed', 'reason': f'user_no {user_no} not found'}), 404

            return jsonify({
                'status': 'success',
                'user': {
                    'user_no': user['user_id'],
                    'nickname': user['nickname'],
                    'name': user['name'],
                    'age': user.get('age'),
                    'email': user.get('email')
                }
            }), 200

    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500

    finally:
        conn.close()

# # 사용자 정보 수정


@app.route('/users/<int:user_no>', methods=['PUT'])
def update_user(user_no):
    if not request.is_json:
        return jsonify({'error': 'Content-Type must be application/json'}), 415

    data = request.get_json()
    nickname = data.get('nickname')
    name = data.get('name')
    password = data.get('password')
    age = data.get('age')
    email = data.get('email')

    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            sql = """
            UPDATE users 
            SET nickname = COALESCE(%s, nickname),
                name = COALESCE(%s, name),
                password = COALESCE(%s, password),
                age = COALESCE(%s, age),
                email = COALESCE(%s, email)
            WHERE user_id = %s
            """
            hashed_pw = bcrypt.hashpw(password.encode(
                'utf-8'), bcrypt.gensalt()).decode('utf-8') if password else None
            cursor.execute(
                sql, (nickname, name, hashed_pw, age, email, user_no))
            if cursor.rowcount == 0:
                return jsonify({'status': 'failed', 'reason': f'user_no {user_no} not found'}), 404

        conn.commit()

        # 수정된 사용자 정보 조회
        cursor.execute(
            "SELECT user_id, nickname, name, age, email FROM users WHERE user_id = %s", (user_no,))
        user = cursor.fetchone()

        return jsonify({
            'status': 'updated',
            'user': {
                'user_no': user['user_id'],
                'nickname': user['nickname'],
                'name': user['name'],
                'age': user.get('age'),
                'email': user.get('email')
            }
        }), 200

    except pymysql.err.IntegrityError as ie:
        return jsonify({'error': str(ie)}), 409

    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500

    finally:
        conn.close()


# # 사용자 삭제

@app.route('/users/<int:user_no>', methods=['DELETE'])
def delete_user(user_no):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            sql = "DELETE FROM users WHERE user_id = %s"
            cursor.execute(sql, (user_no,))
            if cursor.rowcount == 0:
                return jsonify({'status': 'failed', 'reason': f'user_no {user_no} doesn\'t exist'}), 404

        conn.commit()
        return jsonify({'status': 'deleted'}), 200

    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500

    finally:
        conn.close()


if __name__ == '__main__':
    print("registered routes:", app.url_map)  # URL 경로 확인
    app.run(debug=True, use_reloader=False, host='0.0.0.0', port=5000)
