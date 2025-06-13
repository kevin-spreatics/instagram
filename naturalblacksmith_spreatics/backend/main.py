from flask import Flask, request
import pymysql
from pymysql.err import IntegrityError

def get_connection():
    return pymysql.connect(
        host='database-1.cts2qeeg0ot5.ap-northeast-2.rds.amazonaws.com',
        user='kevin',
        password='spreatics*',
        db='insta_jiwon',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )

app = Flask(__name__)

@app.route('/users', methods = ['POST'])
def create_user():

    #user data 받아오기
    data = request.get_json()
    
    user_name = data['name']
    user_pw = data['pw']
    user_nickname = data['nickname']
    user_email = data['email']
    user_age = data['age']

    #sql insert data
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            #중복 nickname 아니면 바로 
            cursor.execute()

            sql_create = """
            Insert into user (name, age, nickname, password, email) Values
            (%s, %s, %s, %s, %s)
            """
            cursor.execute(sql_create, (user_name, user_age, user_nickname, user_pw, user_email))
    except IntegrityError as e:
        if 'nickname' in e:
            return {
                "status": "failed",
                "reason": "nickname is already taken. choose a different nickname"
                }
        else:
            return e


#포스팅
@app.route('/posting', methods = ['POST'])
def create_post():

    #user data 받아오기
    data = request.get_json()
    
    post_title = data['title']
    post_text= data['text']
    post_date = data['date']
    user_id = data['user_id']

    #sql insert data
    conn = get_connection()

    if post_text and post_date: 
        with conn.cursor() as cursor:

            sql_create = """
            Insert into post (title, post_text, posting_date, user_id) Values
            (%s, %s, %s, %s)
            """
            cursor.execute(sql_create, (post_title, post_text, post_date, user_id))
            new_post_id = cursor.lastrowid

        conn.commit()
        
        return {
            "status": "posting success",
            "post_id": new_post_id
        }
    else: 
        return {
                "status": "failed",
                "reason": "please write down anything in your post"
                }

    
@app.route('/delete_post', methods = ['POST'])
def delete_post():
    
    #user 받아오기
    user = request.get_json()
    user_id = user["user_id"]

    #sql 연결
    conn = get_connection()
    
    #sql(user_id)가 user(user_id)인 포스팅 가져오기 
    with conn.cursor() as cursor:
        sql = """
        Select * from post 
        where user_id = %s
        """
        
        cursor.execute(sql, (user_id,))
        rows = cursor.fetchall()
        for row in rows:
            print(row)

    conn.commit()

    #포스팅 중 지우고 싶은 post_id 받아오기
    post = request.get_json()
    post_id = post['post_id']

    #sql(post_id)가 post(post_id)인 포스팅 지우기
    with conn.cursor() as cursor:
        sql = """
        Delete from post 
        where post_id = %s
        """
        cursor.execute(sql, (post_id,))
        deleted_rows = cursor.rowcount
    conn.commit()

    if deleted_rows == 0:
        return {
            "status": "failed",
            "reason": "no post found with that post_id"
        }
    else:
        return {
            "status": "deleted",
            "post_id": post_id
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



