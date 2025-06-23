import pymysql
from flask import Flask, request

app = Flask(__name__)

def get_connection():
    return pymysql.connect(
        host='database-1.cts2qeeg0ot5.ap-northeast-2.rds.amazonaws.com',
        user='kevin',
        password='spreatics*',
        db='instagram_sohee',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )
# 사용자 생성
@app.route('/users', methods=['POST'])
def create_user():
    data = request.get_json()
    

    # 입력값 추출
    nickname = data['nickname']
    name = data['name']
    password = data['password']
    age = data.get('age')
    email = data.get('email')

    conn = get_connection()
    with conn.cursor() as cursor:
        try:
            sql = """
            INSERT INTO users (nickname, name, password, age, email) 
                values (%s, %s, %s, %s, %s)
            """
            cursor.execute(sql, (nickname, name, password, age, email))
            conn.commit()

            cursor.execute("SELECT user_id FROM users WHERE nickname = %s", (nickname,))
            result = cursor.fetchone()
            user_id = result['user_id']

        except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
    conn.close()

    return {
        "status": "created",
        "user_id": user_id
    }

# 사용자 로그인
@app.route('/users/login', methods=['POST'])
def user_login():
    data = request.get_json()
    nickname = data['nickname']
    password = data['password']

    conn = get_connection()
    with conn.cursor() as cursor:
        sql = "SELECT user_id FROM users WHERE nickname = %s AND password = %s"
        cursor.execute(sql, (nickname, password))
        result = cursor.fetchone()

    conn.close()

    if result:
        user_id = result['user_id']
        return {
            'status': 'success',
            'user_id': user_id
        }
    else:
        return {
            'status': 'failed',
            'reason': 'nickname or password ERROR'
        }
    
# 사용자 정보 조회
@app.route('/users/<int:user_id>', methods=['GET'])
def get_user_info(user_id):
    conn = get_connection()
    with conn.cursor() as cursor:
        sql = "SELECT user_id, nickname, name, email FROM users WHERE user_id = %s"
        cursor.execute(sql, (user_id,))
        result = cursor.fetchone()
    conn.close()

    if result:
        return {
            "status": "success",
            "user": result
        }
    else:
        return {
            "status": "failed",
            "reason": "User not found"
        }
    
# 사용자 정보 수정
@app.route('/users/<int:user_id>', methods=['PUT'])
def update_user(user_id):
    data = request.get_json()

    nickname = data.get('nickname')
    name = data.get('name')
    password = data.get('password')
    age = data.get('age')
    email = data.get('email')

    conn = get_connection()
    with conn.cursor(pymysql.cursors.DictCursor) as cursor:
        
        cursor.execute("SELECT * FROM users WHERE user_id = %s", (user_id,))
        user = cursor.fetchone()

        if not user:
            conn.close()
            return {'status': 'failed', 'reason': 'User not found'}

        updates = []
        values = []

        if nickname:
            updates.append("nickname = %s")
            values.append(nickname)
        if name:
            updates.append("name = %s")
            values.append(name)
        if password:
            updates.append("password = %s")
            values.append(password)
        if age is not None:
            updates.append("age = %s")
            values.append(age)
        if email:
            updates.append("email = %s")
            values.append(email)

        if updates:
            sql = "UPDATE users SET " + ", ".join(updates) + " WHERE user_id = %s"
            values.append(user_id)
            cursor.execute(sql, values)
            conn.commit()

    conn.close()
    return {'status': 'success'}

# 사용자 삭제
@app.route('/users/<int:user_id>', methods = ['DELETE'])
def del_user(user_id):
    conn = get_connection()
    with conn.cursor() as cursor:
        cursor.execute("SELECT * FROM users WHERE user_id = %s", (user_id,))
        result = cursor.fetchone()

        if not result:
            conn.close()
            return {
                "status" : "failed",
                "reason" :"User not found"
            }

        cursor.execute("DELETE FROM users WHERE user_id = %s", (user_id,))
        conn.commit()

    conn.close()
    return {
        "status" : "deleted"
    }

## 포스팅
# 게시글 생성
@app.route('/posts', methods = ['POST'])
def create_post():
    data = request.get_json()

    title = data.get('title')
    text = data.get('text')
    user_id = data.get('user_id')

    if not title:
        return {"status": "failed", "reason": "Missing required field: title"}
    if not text:
        return {"status": "failed", "reason": "Missing required field: text"}
    if not user_id:
        return {"status": "failed", "reason": "Missing required field: user_id"}
    
    conn = get_connection()
    with conn.cursor () as cursor:
        cursor.execute("SELECT * FROM users WHERE user_id = %s", (user_id,))
        user_result = cursor.fetchone()

        if not user_result:
            conn.close()
            return {"status": "failed", "reason": "User_id not found"}, 404

        
        sql = """
            INSERT INTO posts (title, text, created_at, user_id)
            VALUES (%s, %s, NOW(), %s)
        """
        cursor.execute(sql, (title, text, user_id))
        conn.commit()
        post_id = cursor.lastrowid
        

    conn.close()

    return {
        "status": "created",
        "post_id": post_id
    }

# 올라온 포스트 조회하기 
@app.route('/posts', methods= ['GET'])
def get_posts():
    conn = get_connection()
    with conn.cursor(pymysql.cursors.DictCursor) as cursor:
        sql = """
        SELECT 
            p.post_id, p.title, p.text AS text, p.user_id, u.nickname, p.created_at
        FROM 
            posts p
        JOIN 
            users u ON p.user_id = u.user_id
        ORDER BY 
            p.created_at DESC
        """
        cursor.execute(sql)
        result = cursor.fetchall()

    conn.close()

    return {
        "status": "success",
        "posts": result
    }

# 커맨트 달기
@app.route('/posts/<int:post_id>/comments', methods=['POST'])
def add_comment(post_id):
    data = request.get_json()

    user_id = data.get('user_id')
    text = data.get('text')

    
    if not user_id or not text:
        return {'status': 'failed', 'reason': 'Missing user_id or text'}

    conn = get_connection()
    with conn.cursor(pymysql.cursors.DictCursor) as cursor:
        
        cursor.execute("SELECT * FROM posts WHERE post_id = %s", (post_id,))
        post = cursor.fetchone()
        if not post:
            conn.close()
            return {'status': 'failed', 'reason': 'Post not found'}

        
        cursor.execute("SELECT * FROM users WHERE user_id = %s", (user_id,))
        user = cursor.fetchone()
        if not user:
            conn.close()
            return {'status': 'failed', 'reason': 'User not found'}

       
        sql = """
        INSERT INTO comments (user_id, post_id, text)
        VALUES (%s, %s, %s)
        """
        cursor.execute(sql, (user_id, post_id, text))
        conn.commit()
        comment_id = cursor.lastrowid

    conn.close()

    return {
        'status': 'success',
        'comment': {
            'post_id': post_id,
            'comment_id': comment_id,
            'user_id': user_id,
            'text': text
        }
    }

# 포스트의 커맨트 조회하기
@app.route('/posts/<int:post_id>/comments', methods=['GET'])
def get_comments(post_id):
    conn = get_connection()
    with conn.cursor(pymysql.cursors.DictCursor) as cursor:
        
        sql = """
        SELECT 
            c.comment_id, c.post_id, c.user_id, c.text AS text
        FROM 
            comments c
        WHERE 
            c.post_id = %s
        ORDER BY 
            c.comment_id ASC
        """
        cursor.execute(sql, (post_id,))
        result = cursor.fetchall()

    conn.close()

   
    return {
        "status": "success",
        "comments": result
    }

## 소셜
# 다른 사용자 조회하기








app.run(debug=True, host='0.0.0.0', port=5001)