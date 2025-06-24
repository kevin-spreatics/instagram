# 인스타그램처럼 작동하는 백엔드 서버 프로그램


# 1. 필요한 기능 가져오기(모듈 import)
'''
- Flask : 웹 서버 만드는 핵심 도구. 서버 : 요청을 받고, 응답을 보내주는 역할을 함
- request : 누가 요청하면 거기에 담긴 정보를 읽어오는 도구
- jsonify : 우리가 보내줄 내용을 JSON이라는 컴퓨터가 좋아하는 형태로 바꿔주는 도구
- generate_password_hash : 비밀번호를 저장하기 전에 암호처럼 바꿔주는 도구
- check_password_hash : 비밀번호가 맞는지 비교하는 도구
- pymysql : 데이터베이스랑 연결할 수 있게 도와주는 도구
'''
from flask import Flask, request, jsonify
from werkzeug.security import generate_password_hash, check_password_hash
import pymysql


# 2. Flask 앱 만들기
'''
=> '나 지금 웹서버 하나 만들 거야. 이름은 app이라고 부를게'
- app : 앞으로 계속 우리가 만든 서버를 조종할 핸들
- Flask(. . .) => '나 서버 하나 만들게'
- __name__은 현재 파일이름(파이썬모듈) 의미. Flask가 어디서 실행되었는지 판단
'''
app = Flask(__name__)


# 3. 데이터베이스 연결 함수 만들기
'''
- 우리가 정보를 저장할 데이터 창고(MySQL)와 연결하는 코드

- host : 어디 있는 창고에 연결할지(창고 주소)
- user, password : 로그인 정보
- db : 어떤 창고인지(데이터베이스 이름)
- DictCursor: 나중에 꺼낼 때, '0번째 줄' 대신에 user['name'] 이런 식으로 사람 이름처럼 꺼내기 위함

=>"컴퓨터야, 내가 데이터 좀 저장하거나 불러올 건데, 이 주소에 있는 창고랑 연결 좀 해줘~"
'''


def get_connection():
    return pymysql.connect(
        host='database-1.cts2qeeg0ot5.ap-northeast-2.rds.amazonaws.com',
        user='pdohee',
        password='gamzagoguma',
        db='instagram_gamza',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )


# 4. 회원가입 & 사용자 검색
'''
- /users 라는 주소로 누군가 요청을 보내면 두 가지 일 중 하나를 함

- POST : 누군가 회원가입하러 왔을 때
- GET : 누군가 사용자 검색하려고 왔을 때
'''


@app.route('/users', methods=['GET', 'POST'])
def users():
    # 회원가입
    if request.method == 'POST':
        # 사용자가 입력한 내용(이름, 비번 등)을 가져옴
        data = request.get_json()
        # 꼭 필요한 정보 미리 정해놓음.
        required = ['nickname', 'name', 'password']

        # 꼭 필요한 정보 중 빠진 거 있으면 실패 메시지
        if not all(k in data for k in required):
            return jsonify({"status": "failed", "reason": "Missing required fields"})

        conn = get_connection()
        try:
            with conn.cursor() as cursor:
                cursor.execute(
                    "SELECT * FROM users WHERE nickname=%s", (data['nickname'],))
                if cursor.fetchone():
                    return jsonify({"status": "failed", "reason": f"nickname, {data['nickname']} is duplicated"})

                # 비밀번호를 암호처럼 바꿔 저장(보안)
                hashed_pw = generate_password_hash(data['password'])

                # 사용자가 입력한 정보들을 실제 데이터베이스에 입력
                cursor.execute(
                    "INSERT INTO users (nickname, name, password, age, email) VALUES (%s, %s, %s, %s, %s)",
                    (data['nickname'], data['name'], hashed_pw,
                     data.get('age'), data.get('email'))
                )
                conn.commit()
                return jsonify({"status": "created", "user_id": cursor.lastrowid})
        finally:
            conn.close()

    # 사용자 검색
    # /users?keyword=수 라고 보내면 이름이나 닉네임에 '수'가 들어간 사람 찾음
    keyword = request.args.get('keyword', '')
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            like = f"%{keyword}%"
            cursor.execute(
                "SELECT user_id, nickname FROM users WHERE nickname LIKE %s OR name LIKE %s", (like, like))
            result = cursor.fetchall()

            return jsonify(result)
    finally:
        conn.close()


# 5. 로그인
'''
- 닉네임과 비밀번호 입력받음
- 닉네임에 맞는 사람을 db에서 찾음
- 그 사람이 입력한 비밀번호가 저장된 해시랑 맞는지 확인
- 맞으면 로그인 성공, 틀리면 실패 응답
'''


@app.route('/login', methods=['POST'])
def login():
    # 사용자가 입력한 닉네임과 비밀번호 받기
    data = request.get_json()
    nickname, password = data.get("nickname"), data.get("password")

    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            # 그 닉네임 가진 사람 있는지 확인
            cursor.execute(
                "SELECT * FROM users WHERE nickname=%s", (nickname,))
            user = cursor.fetchone()
            if not user:
                return jsonify({"status": "failed", "reason": "User not found"})

            # 저장된 해시값과 입력한 비밀번호가 일치하는지 확인
            if check_password_hash(user['password'], password):
                return jsonify({"status": "authenticated"})
            return jsonify({"status": "failed", "reason": "Invalid credentials"})
    finally:
        conn.close()


# 6. 유저 정보 보기, 수정, 삭제하기
'''
- /users/2 => user_id가 2번인 사람을 조회하거나 바꾸거나 삭제함
- GET : 2번 유저의 정보를 보여줌
- PUT : 정보 수정
- DELETE : 유저 삭제
'''


@app.route('/users/<int:user_id>', methods=['GET', 'PUT', 'DELETE'])
def user_detail(user_id):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            if request.method == 'GET':
                cursor.execute(
                    "SELECT user_id, nickname, name, email, age FROM users WHERE user_id=%s", (user_id,))
                user = cursor.fetchone()
                return jsonify(user) if user else jsonify({"status": "failed", "reason": "User not found"})

            elif request.method == 'PUT':
                data = request.json
                fields, values = [], []
                for k in ['name', 'password', 'age', 'email']:
                    if k in data:
                        v = generate_password_hash(
                            data[k]) if k == 'password' else data[k]
                        fields.append(f"{k}=%s")
                        values.append(v)
                values.append(user_id)
                if fields:
                    cursor.execute(
                        f"UPDATE users SET {', '.join(fields)} WHERE user_id=%s", values)
                    conn.commit()
                return jsonify({"status": "updated"})

            elif request.method == 'DELETE':
                cursor.execute(
                    "DELETE FROM users WHERE user_id=%s", (user_id,))
                conn.commit()
                return jsonify({"status": "deleted"})
    finally:
        conn.close()


# 7. 게시글 올리기 & 보기
'''
- POST : 게시글 등록 (title, text 저장)
- GET : 전체 게시글 조회 or 특정 유저의 글만 보기
'''


@app.route('/posts', methods=['POST', 'GET'])
def posts():
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            if request.method == 'POST':
                data = request.get_json()
                cursor.execute(
                    "INSERT INTO posts (user_id, title, text) VALUES (%s, %s, %s)",
                    (data['user_id'], data['title'], data['text'])
                )
                conn.commit()
                return jsonify({"status": "created", "post_id": cursor.lastrowid})

            user_id = request.args.get('user_id')
            if user_id:
                cursor.execute(
                    "SELECT * FROM posts WHERE user_id=%s ORDER BY created_at DESC", (user_id,))
            else:
                cursor.execute("SELECT * FROM posts ORDER BY created_at DESC")
            return jsonify(cursor.fetchall())
    finally:
        conn.close()


# 8. 댓글 달기
# 댓글 달기 (user_id, comment 내용 필요)
@app.route('/posts/<int:post_id>/comment', methods=['POST'])
def add_comment(post_id):
    data = request.get_json()
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute(
                "INSERT INTO comments (post_id, user_id, text) VALUES (%s, %s, %s)",
                (post_id, data['user_id'], data['comment'])
            )
            conn.commit()
            return jsonify({"status": "created", "comment_id": cursor.lastrowid})
    finally:
        conn.close()


# 9. 댓글 목록 보기
# 댓글 목록 보기 (작성 시간 순으로)


@app.route('/posts/<int:post_id>/comments', methods=['GET'])
def get_comments(post_id):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute(
                "SELECT * FROM comments WHERE post_id=%s ORDER BY created_at", (post_id,))
            return jsonify(cursor.fetchall())
    finally:
        conn.close()


# 10. 팔로우 요청
'''
- 누군가 다른 사람을 팔로우하려고 하면 요청 보냄
- 이미 팔로우 중이면 막음
'''


@app.route('/follow/request', methods=['POST'])
def follow_request():
    data = request.get_json()
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("SELECT * FROM follows WHERE follower_id=%s AND followee_id=%s",
                           (data['from_user_id'], data['to_user_id']))
            if cursor.fetchone():
                return jsonify({"status": "already_following"})

            cursor.execute("INSERT INTO follows (follower_id, followee_id) VALUES (%s, %s)",
                           (data['from_user_id'], data['to_user_id']))
            conn.commit()
            return jsonify({"status": "requested"})
    finally:
        conn.close()


# 11. 팔로우 응답(수락/거절)
'''
- accept ➝ 수락해서 친구가 됨
- reject ➝ 거절해서 친구 안 됨
'''


@app.route('/follow/respond', methods=['POST'])
def follow_respond():
    data = request.get_json()
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            if data['action'] == 'accept':
                cursor.execute(
                    "UPDATE follows SET status='accepted' WHERE follow_id=%s", (data['request_id'],))
                conn.commit()
                return jsonify({"status": "accepted"})
            elif data['action'] == 'reject':
                cursor.execute(
                    "DELETE FROM follows WHERE follow_id=%s", (data['request_id'],))
                conn.commit()
                return jsonify({"status": "rejected"})
            return jsonify({"status": "failed", "reason": "invalid action"})
    finally:
        conn.close()


# 12. 받은 팔로우 요청 보기
# 나를 팔로우하려는 요청자들의 목록 보기
@app.route('/follow/requests', methods=['GET'])
def follow_requests():
    user_id = request.args.get('user_id')
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("""
                SELECT follow_id AS request_id, follower_id AS from_user_id, u.nickname,
                       'https://cdn.example.com/profiles/default.jpg' AS profile_image
                FROM follows f
                JOIN users u ON f.follower_id = u.user_id
                WHERE followee_id=%s AND status='pending'""", (user_id,))
            return jsonify(cursor.fetchall())
    finally:
        conn.close()


# 13. 내가 팔로우한 목록
# 내가 팔로우한 사람들 보여줌
@app.route('/follow/list', methods=['GET'])
def follow_list():
    user_id = request.args.get('user_id')
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("""
                SELECT u.user_id, u.nickname,
                       'https://cdn.example.com/profiles/default.jpg' AS profile_image
                FROM follows f
                JOIN users u ON f.followee_id = u.user_id
                WHERE f.follower_id=%s AND f.status='accepted'""", (user_id,))
            return jsonify(cursor.fetchall())
    finally:
        conn.close()


# 14. dm 전송
@app.route('/dm', methods=['POST'])
def send_dm():
    data = request.get_json()
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute(
                "INSERT INTO messages (sender_id, receiver_id, text) VALUES (%s, %s, %s)",
                (data['from_user_id'], data['to_user_id'], data['message'])
            )
            conn.commit()
            return jsonify({"status": "sent", "message_id": cursor.lastrowid})
    finally:
        conn.close()


# 15. 대화 내역 보기
@app.route('/dm/conversation', methods=['GET'])
def get_dm():
    id1, id2 = request.args.get('user1_id'), request.args.get('user2_id')
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("""
                SELECT * FROM messages
                WHERE (sender_id=%s AND receiver_id=%s)
                   OR (sender_id=%s AND receiver_id=%s)
                ORDER BY created_at""", (id1, id2, id2, id1))
            return jsonify(cursor.fetchall())
    finally:
        conn.close()


# 16. 특정 메시지 삭제
@app.route('/dm/<int:message_id>', methods=['DELETE'])
def delete_dm(message_id):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute(
                "DELETE FROM messages WHERE message_id=%s", (message_id,))
            conn.commit()
            return jsonify({"status": "deleted"})
    finally:
        conn.close()


# 17. 서버 시작
'''
=> '서버야, 이제 진짜 작동해줘'
- 0.0.0.0: 외부에서도 접속 가능
- debug=True: 코드를 바꾸면 서버가 자동으로 다시 실행됨
'''
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
