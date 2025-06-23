from flask import Flask, request
import pymysql

def get_connection():
    return pymysql.connect(
        host='localhost',
        user='root',
        password='gmdtm457^^',
        db='create_account',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )

app = Flask(__name__)
# 유저 생성 및 생성 user_id 조회
@app.route('/create_user', methods=['POST'])
def user_generation():
    data = request.get_json()

    nickname = data.get('nickname')
    password = data.get('password')
    name = data.get('name')
    age = data.get('age')
    email = data.get('email')

    if((nickname is None) or (password is None) or (name is None)):
        return {"status": "failed", "reason": "nickname, password, name is must enter."}

    conn = get_connection()

    with conn.cursor() as cursor:
        try:
            sql = """insert into users(nickname, password, name, age, email) 
                    values(%s, %s, %s, %s, %s)"""
            cursor.execute(sql, (nickname, password, name, age, email))
            conn.commit()
        except pymysql.err.IntegrityError as e:
            return {"status": "failed", "reason": f"{str(e)}, The nickname is duplicated."}

    with conn.cursor() as cursor:
        sql = """select *
                 from users
                 where nickname = %s;"""
        cursor.execute(sql, (nickname,))
        rows = cursor.fetchall()
    
        return {"status": "Created", "user_id": rows[0]['user_id']}
    
# 사용자 로그인 및 유저 id 조회
@app.route('/Auth_users', methods=['POST'])
def authenticatioin_user():
    data = request.get_json()

    nickname = data.get('nickname')
    password = data.get('password')

    if((nickname is None) or (password is None)):
        return {"status": "failed", "reason": "enter both your nickname and password"}
    
    conn = get_connection()

    with conn.cursor() as cursor:
        sql = """select *
                from users
                where nickname = %s
                    and password = %s;"""
        cursor.execute(sql, (nickname, password))
        rows = cursor.fetchall()
        if len(rows) == 1:
            return {"status": "log_in Succenss", "user_id": f'{rows[0]['user_id']}'}
        elif len(rows) == 0:
            return {"status": "log_in failed", "reason": "nickname, password was unmatched"} 

# 사용자 정보 조회
@app.route('/users/<user_id>')
def get_info(user_id):
    conn = get_connection()
    with conn.cursor() as cursor:
        sql = """select *
                 from users
                 where user_id = %s;"""
        cursor.execute(sql, (user_id))
        rows = cursor.fetchall()

        return {"status": "selected", 
                "user_id": rows[0]['user_id'], 
                "nickname": rows[0]['nickname'], 
                "password": rows[0]['password'],
                "name": rows[0]['name'], 
                "age": rows[0]['age'], 
                "email": rows[0]['email']}
    
# 사용자 정보 수정
@app.route('/user_update/<user_id>', methods=['PUT'])
def update_info(user_id):
    info = get_info(user_id)
    data = request.get_json()

    auth_nickname = data.get('auth_nickname')
    auth_password = data.get('auth_password')

    if((auth_nickname is None) or (auth_password is None)):
        return {"status": "failed", "reason": "You will need to re-authenticate to edit your information."}
    
    for key_1, value_1 in data.items():
        info[key_1] = value_1
    
    conn = get_connection()

    with conn.cursor() as cursor:
        sql_1 = """select *
                   from users
                   where nickname = %s
                        and password = %s;"""
        cursor.execute(sql_1, (auth_nickname, auth_password))

        rows = cursor.fetchall()
        if len(rows) == 1:
            sql_2 = """update users
                        set nickname = %s, password = %s, email = %s
                        where user_id = %s;"""
            cursor.execute(sql_2, (info['nickname'], info['password'], info['email'], user_id))
            conn.commit()
            return {"status": "Update", "chg_nickname": info['nickname'], "chg_password": info['password'], "chg_email": info['email']}
        elif len(rows) == 0:
            return {"status": "failed", "reason": "auth_nickname, auth_password was unmatched"}

# 사용자 정보 삭제   
@app.route('/users/<user_id>', methods=['DELETE'])
def del_user(user_id):
    data = request.get_json()

    auth_nickname = data.get('auth_nickname')
    auth_password = data.get('auth_password')

    if (auth_nickname is None) or (auth_password is None):
        return {"status": "failed", "reason": "You must re-authenticate to delete account."}
    
    conn = get_connection()

    with conn.cursor() as cursor:
        sql_1 = """select *
                   from users
                   where nickname = %s
                        and password = %s;"""
        cursor.execute(sql_1, (auth_nickname, auth_password))

        rows = cursor.fetchall()
        if len(rows) == 1:
            sql = """delete from users
                    where user_id = %s;"""
            cursor.execute(sql, (user_id, ))
            conn.commit()
            return {"status": "deleted", "result": "user account was deleted"}
        elif len(rows) == 0:
            return {"status": "failed", "reason": "auth_nickname, auth_password was unmatched"}

app.run(debug=True, host='0.0.0.0', port=5000)