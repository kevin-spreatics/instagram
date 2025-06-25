from flask import Flask, request, session
import pymysql
from datetime import datetime

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

# 세션 암호키
app.secret_key = 'abc1234'

# 404오류시 실행
@app.errorhandler(404)
def not_found(e):
    return {"status": "failed", "reason": "Not Found"}, 404

# 사용자 생성
@app.route('/users/signup', methods=['POST'])
def create_user():
    data = request.get_json(silent=True) or {}
    nickname = data.get('nickname')
    name = data.get('name')
    password = data.get('password')
    age = data.get('age')
    age = int(age) if age and age.isdigit() else None
    email = data.get('email')
    email = email if email else None
    
    if not nickname or not name or not password:
        return {"status": "failed", "reason": "nickname, name, password is None"}

    conn = get_connection()

    with conn.cursor() as cursor:
        try:
            search_sql = """    
            SELECT *
            FROM users
            WHERE nickname = %s
            """
            cursor.execute(search_sql, (nickname,))
            row = cursor.fetchone()
            if row is not None:
                return {"status": "failed", "reason": f"nickname, {nickname} is duplicated"}

            sql = """
            INSERT INTO users (nickname, name, password, age, email) 
                values (%s, %s, %s, %s, %s)
            """
            cursor.execute(sql, (nickname, name, password, age, email))
            conn.commit()

        except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
    
    with conn.cursor() as cursor:
        sql = """
        SELECT user_id
        FROM users
        WHERE nickname = %s
        """
        cursor.execute(sql,(nickname,))
        return {"status": "success", "user_id": cursor.fetchone()}
    
# 사용자 인증(로그인)
@app.route('/users/login', methods = ['POST'])
def login_user():
    # 로그인 중복 방지
    if 'user_id' in session:
        return {"status": "failed", "reason": "Already logged in."}
    
    data = request.get_json()
    nickname = data.get('nickname')
    password = data.get('password')

    if not nickname:
        return {"status":"failed", "reason":"nickname is None."}
    elif not password:
        return {"status":"failed", "reason":"password is None."}
    
    conn = get_connection()
    
    try:
        with conn.cursor() as cursor:
            sql_select = "SELECT * FROM users WHERE nickname = %s"
            cursor.execute(sql_select, (nickname,))
            row = cursor.fetchone()

            if not row:
                return {"status": "failed", "reason": f"nickname, {nickname} doesn't exist"}
            if row['password'] == password:
                #이미 로그인 되어있으면 거부
                if 'user_id' in session:
                    return {"status": "failed", "reason": "Already logged in"}
                #세션에 user id 저장
                session['user_id'] = row['user_id']
                return {"status": "success", "user_id": row['user_id'], "login":"login complete."}
            else:
                return {"status": "failed", "reason": "password, password doesn't match"}

    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
# 로그아웃
@app.route('/users/logout', methods=['POST'])
def logout():
    if 'user_id' not in session:
        return {"status": "failed", "reason": "Not currently logged in"}
    # 세션에서 정보 삭제
    session.pop('user_id', None)
    return {"status": "success", "logout":"logout complete."}

# 내 정보 조회
@app.route('/users/me')
def get_my_info():
    session_user_id = session.get('user_id')
    if not session_user_id:
        return {"status": "failed", "reason": "Login required."}

    conn = get_connection()
    with conn.cursor(pymysql.cursors.DictCursor) as cursor:
        sql = """
        SELECT user_id, nickname, name, age, email
        FROM users
        WHERE user_id = %s
        """
        cursor.execute(sql, (session_user_id,))
        row = cursor.fetchone()
        if not row:
            return {"status": "failed", "reason": "User not found."}
        return {"status": "success", "user": row}

# 다른 사용자 정보 조회
@app.route('/users/search', methods = ["GET", "POST"])
def search_user():
    data = request.get_json(silent=True) or {}
    search = data.get('search')

    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            # 아무정보 입력안하면 전체 유저 검색
            if not search:
                sql = """
                SELECT age, email, name, nickname, user_id
                FROM users
                """
                cursor.execute(sql)
                rows = cursor.fetchall()

                if not rows:
                    return {"status": "success", "users": "no search results."}
                
                return {"status": "success", "user": rows}
            # 검색어에 해당하는 nickname 이나 name 검색 / 본인 정보보기
            elif search:
                sql = """
                SELECT age, email, name, nickname, user_id
                FROM users
                WHERE nickname LIKE %s OR name LIKE %s or user_id = %s
                """
                cursor.execute(sql,('%'+search+'%','%'+search+'%',session["user_id"]))
                rows = cursor.fetchall()

                if not rows:
                    return {"status": "success", "users": "no search results."}
                
                return {"status": "success", "user": rows}

    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }

# 사용자 정보 수정
@app.route('/users/me/<user_id>', methods = ['PUT'])
def update_user(user_id):
    session_user_id = session.get('user_id')
    # 로그인 해야 접근가능
    if session_user_id is None:
        return {"status": "failed", "reason": "Login required."}
    # url user_id와 세션 user_id가 다르면 권한 없음
    if str(session_user_id) != user_id:
        return {"status": "failed", "reason": "Permission denied.", "session_user_id" : session_user_id, "user_id": user_id}

    data = request.get_json(silent=True) or {}
    conn = get_connection()

    with conn.cursor(pymysql.cursors.DictCursor) as cursor:
        try:
            sql_select = """
            SELECT *
            FROM users
            WHERE user_id = %s
            """
            cursor.execute(sql_select,(user_id,))
            row = cursor.fetchone()
            
            # 입력한 user_id 가 db에 없는 경우
            if row is None:
                return {"status": "failed", "reason": f"user_id, {user_id} doesn't exist."}

            # 사용자가 입력하지 않으면 기존 값 유지
            nickname = data.get('nickname') or row['nickname']
            name = data.get('name') or row['name']
            password = data.get('password') or row['password']
            age = data.get('age') or row['age']
            email = data.get('email') or row['email']

            sql_update = """
            UPDATE users 
            SET nickname = %s, name = %s, password = %s, age = %s, email = %s 
            WHERE user_id = %s
            """
            cursor.execute(sql_update, (nickname, name, password, age, email,user_id))
            conn.commit()
            
            updated_user = {
                "user_id": user_id,
                "nickname": nickname,
                "name": name,
                "age": age,
                "email": email
            }
            
            return {"status":"success", "user":updated_user}

        except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
        
# 사용자 삭제
@app.route('/users/me/<user_id>', methods = ['DELETE'])
def delete_user(user_id):
    session_user_id = session.get('user_id')
    # 로그인 해야 접근가능
    if session_user_id is None:
        return {"status": "failed", "reason": "Login required."}
    # url user_id와 세션 user_id가 다르면 권한 없음
    if str(session_user_id) != user_id:
        return {"status": "failed", "reason": "Permission denied."}
    
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            select = "SELECT * FROM users WHERE user_id = %s"
            cursor.execute(select, (user_id,))
            row = cursor.fetchone()

            if row is None:
                return {"status": "failed", "reason": f"user_id, {user_id} doesn't exist"}
            
            delete = "DELETE FROM users WHERE user_id = %s"
            cursor.execute(delete, (user_id,))
            conn.commit()

            return {"status": "deleted"}

    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }

# 포스트 올리기
@app.route('/posts/upload', methods = ['POST'])
def upload_post():
    user_id = session.get('user_id')
    # 로그인 해야 접근가능
    if not user_id:
        return {"status": "failed", "reason": "Login required."}
    
    data = request.get_json()
    title = data.get('title')
    text = data.get('text')

    if not title:
        return {"status":"failed", "reason":"title is None."}
    elif not text:
        return {"status":"failed", "reason":"text is None."}
    elif not user_id:
        return {"status":"failed", "reason":"user_id is None."}
    
    conn = get_connection()
    
    try:
        with conn.cursor() as cursor:
            select1 = "SELECT * FROM users WHERE user_id = %s"
            cursor.execute(select1, (user_id,))
            row = cursor.fetchone()

            if not row:
                return {"status": "failed", "reason": f"user_id, {user_id} doesn't exist"}
            
            insert = """
            INSERT INTO posts (title, text, user_id) 
                values (%s, %s, %s)
            """
            cursor.execute(insert, (title, text, user_id))
            conn.commit()

            select2 = "SELECT * FROM posts WHERE user_id = %s ORDER BY created_at DESC LIMIT 1"
            cursor.execute(select2, (user_id,))
            row = cursor.fetchone()
            return {"status": "created", "posts": row}

    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
    
# 올라온 포스트 조회하기
@app.route('/posts', methods=["GET","POST"])
def search_post():
    data = request.get_json(silent=True) or {}
    user_id = data.get('user_id')
    post_id = data.get('post_id')

    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            # 아무정보 안보내면 전체 포스트 조회하기
            if not user_id and not post_id: 
                sql = """
                SELECT posts.*, users.nickname 
                FROM posts
                JOIN users ON posts.user_id = users.user_id
                """
                cursor.execute(sql)
                rows = cursor.fetchall()
                
                return {"status": "success", "posts": rows}
            # 포스트 id로 조회하기
            elif not user_id:
                sql = """
                SELECT posts.*, users.nickname 
                FROM posts
                JOIN users ON posts.user_id = users.user_id
                WHERE post_id = %s
                """
                cursor.execute(sql,(post_id,))
                rows = cursor.fetchall()

                if not rows :
                    return {"status": "success", "posts":f"post_id, {post_id} doesn't exist"}

                return {"status": "success", "posts": rows[0]}
            # 유저 id로 조회하기
            elif not post_id:
                sql = """
                SELECT posts.*, users.nickname 
                FROM posts
                JOIN users ON posts.user_id = users.user_id
                WHERE user_id = %s
                """
                cursor.execute(sql,(user_id,))
                rows = cursor.fetchall()

                if not rows:
                    return {"status": "success", "posts":f"user_id, {user_id}'s posts don't exist"}

                return {"status": "success", "posts": rows}
                
    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
    
# 포스트의 커맨트 조회하기
@app.route('/posts/<post_id>/comments')
def search_comments(post_id):
    conn = get_connection()

    try:
        with conn.cursor() as cursor:
            # 조회할 post_id 가 존재하지 않는 경우
            select = """
            SELECT *
            FROM posts
            WHERE post_id = %s
            """
            cursor.execute(select, (post_id,))
            row = cursor.fetchone()

            if not row:
                return {"status":"failed", "reason":f"post_id, {post_id} doesn't exist"}
            # comments 조회
            select2 = """
            SELECT comments.*, users.nickname
            FROM comments
            JOIN users ON comments.user_id = users.user_id
            WHERE post_id = %s
            """
            cursor.execute(select2, (post_id,))
            rows = cursor.fetchall()

            if not rows:
                return {"status":"success", "comments":"comments is empty"}

            return {"status": "success", "comments": rows}
            
    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }
    
# 특정 포스트에 커맨트 달기
@app.route('/posts/<post_id>/comments', methods = ['POST'])
def make_comment(post_id):
    user_id = session.get('user_id')
    # 로그인 해야 접근가능
    if user_id is None:
        return {"status": "failed", "reason": "Login required."}
    
    data = request.get_json()
    text = data.get('text')

    if text is None:
        return {"status":"failed", "reason":"text is None."}
    
    conn = get_connection()
    
    try:
        with conn.cursor() as cursor:
            #user_id 있는지 체크
            select1 = "SELECT * FROM users WHERE user_id = %s"
            cursor.execute(select1, (user_id,))
            row = cursor.fetchone()

            if not row:
                return {"status": "failed", "reason": f"user_id, {user_id} doesn't exist"}
            #comments에 comment 추가
            insert = """
            INSERT INTO comments (text, user_id, post_id) 
                values (%s, %s, %s)
            """
            cursor.execute(insert, (text, user_id, post_id))
            conn.commit()
            # 게시한 comments 리턴
            select2 = select2 = """
            SELECT *
            FROM comments
            WHERE post_id = %s && user_id = %s
            ORDER BY created_at DESC LIMIT 1
            """
            cursor.execute(select2, (post_id, user_id))
            row = cursor.fetchone()
            return {"status": "created", "comments": row}

    except pymysql.err.IntegrityError as e:
            return { "status": "failed", "reason": str(e) }

app.run(debug=True, host='0.0.0.0', port=5000)
