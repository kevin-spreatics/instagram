from flask import Flask, request
import pymysql
from pymysql.err import IntegrityError

def get_connection():
    return pymysql.connect(
        host='database-1.cts2qeeg0ot5.ap-northeast-2.rds.amazonaws.com',
        user='kevin',
        password='spreatics*',
        db='instagram',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )

app = Flask(__name__)

#사용자 생성 
@app.route('/user/create', methods = ['POST'])
def create_user():

    #user data 받아오기
    data = request.get_json()
    
    nickname = data['nickname']
    pw = data['pw']
    name = data['name']
    age = data.get('age')
    email = data.get('email')

    if not nickname or not pw or not name:
        return {
            "status" : "failed",
            "reason" : "nickname, password, name is mandatory."
            }


    #sql insert data
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            sql_create = """
            Insert into users (nickname, password, name, age, email) Values
            (%s, %s, %s, %s, %s)
            """
            cursor.execute(sql_create, (nickname, pw, name, age, email))
            conn.commit()

            new_user_id = cursor.lastrowid
            return {
                "status" : "user create success",
                "new user id1" : new_user_id
            }
    
    except IntegrityError as e:
        return {
             "status": "user create failed", 
             "reason": str(e) 
        }

#로그인
@app.route('/user/login', methods = ['POST'])
def login():
    data = request.get_json()

    nickname = data.get('nickname')
    pw = data.get('pw')

    if not nickname or not pw:
        return {
            "status" : "login error",
            "reason" : "nickname and password required"
        }     
    
    conn = get_connection()

    #nickname과 password로 user_id 반환
    try:
        with conn.cursor() as cursor: 
            sql_check = """
            select user_id from users
            where nickname = %s and password = %s
            """
            cursor.execute(sql_check,(nickname, pw))
            row = cursor.fetchone()
            conn.commit()
            
            if row:
                return {
                    "status" : "login success",
                    "login user" : row['user_id']
                }
            else:
                return {
                    "status": "login failed",
                    "reason": "no user found. please create an account"
                }
            
    except Exception as e:
        return {
            "status" : "login failed",
            "error" : str(e)
        }

#사용자 비밀번호 변경 
@app.route('/user/<int:user_id>/password_change', methods = ['PATCH'])
def change_pw(user_id):
    data = request.get_json()

    pw = data.get('pw')  
    new_pw = data.get('newPW') 
    
    if not pw or not new_pw:
        return {
            "status": "password change failed",
            "reason": "password and new password required"
        }

    conn = get_connection()

    with conn.cursor() as cursor:
        #기존 pw 확인 - user validation 
        sql_check_pw = """
        select password from users
        where user_id = %s
        """
        cursor.execute(sql_check_pw, (user_id,))
        row =  cursor.fetchone()

        #새로운 패스워드로 바꾸기
        if row['password'] == pw:
            try:
                sql_change_pw = """
                update users 
                set password = %s
                where user_id = %s
                """
                cursor.execute(sql_change_pw, (new_pw, user_id))
                conn.commit()
                return {
                    "status" : "password change success"
                }

            except Exception as e:
                return {
                    "status" : "password change failed",
                    "reason" : str(e)
                }
        else: 
            return {
                "status" : "password change failed",
                "reason" : "incorrect password"
            }

## 사용자 조회       
@app.route('/user/check', methods = ['GET'])
def user_check():

    user_id = request.args.get("user_id")

    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            sql = """
            Select nickname, password, name 
            from users 
            where user_id = %s"""
            cursor.execute(sql, (user_id,))
            user = cursor.fetchone()
            conn.commit()

            if user:
                return {
                    "status" : "user get success",
                    "nickname" : user["nickname"],
                    "password" : user["password"],
                    "name" : user["name"]
                }
            else:
                return {
                    "status" : "user get failed",
                    "reason" : "user not found"
                }

    except Exception as e:
        return {
            "status" : "user get failed",
            "reason" : str(e)
        }

#user 삭제 user_id 활용
@app.route ('/user/<int:user_id>/delete', methods = ['DELETE'])
def user_delete_id(user_id):
    #sql 연결
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            #user 찾기 
            sql_check = """
            select * from users 
            where user_id = %s
            """
            cursor.execute(sql_check, (user_id,))
            rows = cursor.fetchall()

            if not rows:
                return {
                    "status" : "delete fail",
                    "error" : "user not found"
                }
            
            #user 삭제 
            sql_delete = """
            delete from users
            where user_id = %s
            """
            cursor.execute(sql_delete, (user_id, ))
            conn.commit()
        
        return {
            "status" : "delete success",
            "deleted user" : user_id
        }

    except Exception as e:
        return {
            "status" : "delete failed",
            "error" : str(e)
        }


#user 삭제 nickname 활용
@app.route ('/user/<string:nickname>/delete', methods = ['DELETE'])
def user_delete_nickname(nickname):
    #sql 연결
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            #user 찾기 
            sql_check = """
            select * from users 
            where nickname = %s
            """
            cursor.execute(sql_check, (nickname,))
            row = cursor.fetchone()

            if not row:
                return {
                    "status" : "delete fail",
                    "error" : "user not found"
                }
            
            #user 삭제 
            sql_delete = """
            delete from users
            where nickname = %s
            """
            cursor.execute(sql_delete, (nickname, ))
            conn.commit()
        
        return {
            "status" : "delete success",
        }

    except Exception as e:
        return {
            "status" : "delete failed",
            "error" : str(e)
        }


#포스팅 생성 
@app.route('/posting/<int:user_id>/create', methods = ['POST'])
def create_post(user_id):

    #user data 받아오기
    data = request.get_json()
    
    post_title = data['title']
    post_text= data['post_text']

    #sql insert data
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            sql_create = """
                INSERT INTO posts (title, text, user_id)
                VALUES (%s, %s, %s)
                """
            cursor.execute(sql_create, (post_title, post_text, user_id))
            new_post_id = cursor.lastrowid
            conn.commit()

            return {
                "status": "posting success",
                "post_id": new_post_id
            }

    except Exception as e:
            return {
                "status": "posting failed",
                "reason": str(e)
            }
    
#포스팅 삭제    
@app.route('/posting/delete', methods = ['DELETE'])
def delete_post():

    #user data 받아오기
    data = request.get_json()
    
    post_id = data['post_id']

    #sql insert data
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            sql_create = """
            delete from posts
            where post_id = %s
            """
            cursor.execute(sql_create, (post_id,))
            delete_row = cursor.rowcount
            conn.commit()
        
            if delete_row > 0:
                return {
                    "status": "posting delete success",
                }
            else:
                return {
                    "status" : "posting delete failed",
                    "reason" : "no title was found"
                }
    except Exception as e:
        return {
            "status": "posting delete failed",
            "reason" : str(e)
        }
@app.route('/posting/check', methods = ['GET'])
def post_check():

    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            sql = """
            Select * 
            from posts 
            """
            cursor.execute(sql)
            rows = cursor.fetchall()
            conn.commit()

            if rows:
                results = []
                for row in rows:
                    results.append({
                        "status" : "post get success",
                        "post_id" : row["post_id"],
                        "title" : row["title"],
                        "text" : row["text"]
                    }) 
                return {
                    "status" : "post get success",
                    "result" : results
                }  
            else:
                return {
                     "status" : "post get failed",
                    "reason" : "post not found"
                }

    except Exception as e:
        return {
            "status" : "user get failed",
            "reason" : str(e)
        }

# 코멘트 달기 
@app.route('/comment/<int:user_id>', methods = ['POST'])
def comment_create(user_id):

    #코멘트를 달 포스트 data 받아오기
    data = request.get_json()
    
    post_id = data['post_id']
    text = data['text']


    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            sql = """
            Select * 
            from posts 
            where post_id = %s
            """
            cursor.execute(sql, (post_id,))
            row = cursor.fetchone()
            conn.commit()

            if row:
                sql = """
                Insert into comments (user_id, post_id, text) Values
                (%s, %s, %s)
                """
                cursor.execute(sql, (user_id, post_id, text))
                conn.commit() 

                return {
                    "status" : "comment success"
                }    
            else:
                return {
                     "status" : "comment failed",
                    "reason" : "post not found"
                }

    except Exception as e:
        return {
            "status" : "comment failed",
            "reason" : str(e)
        }


# 코멘트 가져오기  
@app.route('/comment/check', methods = ['GET'])
def comment_check():

    post_id = request.args.get('post_id')

    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            sql = """
            Select * 
            from comments 
            where post_id = %s
            """
            cursor.execute(sql, (post_id,))
            rows = cursor.fetchall()
            conn.commit()

            if rows:
                results = [
                    {
                        "user_id": row["user_id"],
                        "text": row["text"]
                    }
                    for row in rows
                ]
                return {
                    "status": "comment check success",
                    "result": results
                }
            else:
                return {
                    "status": "comment check failed",
                    "reason": "no comments found"
                }

    except Exception as e:
        return {
            "status" : "comment check failed",
            "reason" : str(e)
        }





app.run(debug=True, host='0.0.0.0', port=5001)



# @app.route('./students/<student_id>/enrolls/<course_id>')
# def enrollment(student_id, course_id):
#     conn = get_connection()

#     with conn.cursor() as cursor:
#         sql = """
#         Insert into 
#         """

#         cursor.execute(sql, ('alex', ;))

# @app.route('/enrollment', methods = ['POST'])
# def enrollment():

#     data = request.get_json() #body type check (default 'text')
#     print(data)

#     student_name = data['student_name']
#     course_name = data['course_name']

#     #sql - insert
#     return {'a': student_name, 'b': course_name}


# @app.route('/students/<student_id>')
# def students(student_id):
#         # 0. connection 생성
#         conn = get_connection()

#         # 1. 커서 생성
#         with conn.cursor() as cursor:

#             # 2. SQL 실행
#             sql = "SELECT * FROM student where student_id = %s"
#             cursor.execute(sql, (student_id,))

#             #3. 결과 가져오기
#             rows = cursor.fetchall()
#             return rows[0]

# @app.route('/hello')
# def hello():
#     print('hello')
#     return { 'message': 'hello' }

# @app.route('/hello/<name>')
# def hello_name(name):
#     print(f'hello {name}')
#     return { 'message': 'hello', 'name': name }

# @app.route('/user/<userID>')
# def userIDSet(userID):
#     print(f'user name is {userID}')
#     return { 'message': 'user', 'uswerId': userID }



