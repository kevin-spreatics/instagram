# [1] 필요한 모듈 임포트
# Flask는 웹 서버 프레임워크, jsonify는 JSON 응답 생성
# werkzeug.security는 비밀번호를 안전하게 저장하고 검증하는 데 사용됨
# pymysql은 MySQL 데이터베이스와 Python 간 연동을 도와주는 드라이버
from flask import Flask, request, jsonify
from werkzeug.security import generate_password_hash, check_password_hash
import pymysql

# [2] Flask 애플리케이션 객체 생성
# Flask 앱 객체를 만들고 이 객체를 통해 라우팅 등을 설정함
app = Flask(__name__)

# [3] MySQL 데이터베이스 연결 함수 정의
# 매 요청 시 새로운 DB 연결을 생성하며, DictCursor를 통해 결과를 딕셔너리 형태로 받음
# 반환된 커넥션은 반드시 사용 후 .close() 해줘야 자원이 해제됨

def get_connection():
    return pymysql.connect(
        host='database-1.cts2qeeg0ot5.ap-northeast-2.rds.amazonaws.com',  # RDS 호스트 주소
        user='pdohee',           # 데이터베이스 사용자 ID
        password='gamzagoguma',  # 사용자 비밀번호
        db='instagram_gamza',    # 연결할 데이터베이스 이름
        charset='utf8mb4',       # 한글 등 유니코드 문자 지원
        cursorclass=pymysql.cursors.DictCursor  # SELECT 결과를 dict 형태로 반환함
    )

# [4] 사용자 등록 (회원가입) 및 검색 (키워드 기반)
@app.route('/users', methods=['GET', 'POST'])
def users():
    # [POST] 사용자 회원가입 처리
    if request.method == 'POST':
        data = request.get_json()  # JSON 본문 파싱
        required = ['nickname', 'name', 'password']  # 필수 필드 목록

        # 필수 필드가 빠져 있는지 확인
        if not all(k in data for k in required):
            return jsonify({"status": "failed", "reason": "Missing required fields"})

        conn = get_connection()
        try:
            with conn.cursor() as cursor:
                # 닉네임 중복 체크
                cursor.execute("SELECT * FROM users WHERE nickname=%s", (data['nickname'],))
                if cursor.fetchone():  # 이미 존재하면 실패 응답
                    return jsonify({"status": "failed", "reason": f"nickname, {data['nickname']} is duplicated"})

                # 비밀번호는 평문 저장이 위험하므로 해싱 처리
                hashed_pw = generate_password_hash(data['password'])

                # 사용자 INSERT SQL 실행
                cursor.execute(
                    "INSERT INTO users (nickname, name, password, age, email) VALUES (%s, %s, %s, %s, %s)",
                    (data['nickname'], data['name'], hashed_pw, data.get('age'), data.get('email'))
                )
                conn.commit()  # 변경사항 커밋
                return jsonify({"status": "created", "user_id": cursor.lastrowid})  # 성공 시 user_id 반환
        finally:
            conn.close()

    # [GET] 닉네임 또는 이름을 키워드로 사용자 검색
    keyword = request.args.get('keyword', '')
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            like = f"%{keyword}%"  # LIKE 쿼리용 와일드카드
            cursor.execute("SELECT user_id, nickname FROM users WHERE nickname LIKE %s OR name LIKE %s", (like, like))
            result = cursor.fetchall()
            
            return jsonify(result)
    finally:
        conn.close()

# [5] 사용자 로그인
# 입력된 닉네임과 비밀번호를 이용해 사용자 인증
@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    nickname, password = data.get("nickname"), data.get("password")

    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("SELECT * FROM users WHERE nickname=%s", (nickname,))
            user = cursor.fetchone()
            if not user:
                return jsonify({"status": "failed", "reason": "User not found"})

            # 저장된 해시 비밀번호와 사용자가 입력한 비밀번호를 비교
            if check_password_hash(user['password'], password):
                return jsonify({"status": "authenticated"})
            return jsonify({"status": "failed", "reason": "Invalid credentials"})
    finally:
        conn.close()

# [6] 사용자 상세 정보 조회 / 수정 / 삭제
# RESTful 방식으로 GET, PUT, DELETE 처리
@app.route('/users/<int:user_id>', methods=['GET', 'PUT', 'DELETE'])
def user_detail(user_id):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            if request.method == 'GET':
                # 특정 사용자 조회
                cursor.execute("SELECT user_id, nickname, name, email, age FROM users WHERE user_id=%s", (user_id,))
                user = cursor.fetchone()
                return jsonify(user) if user else jsonify({"status": "failed", "reason": "User not found"})

            elif request.method == 'PUT':
                # 특정 사용자 정보 수정 (비어있지 않은 필드만 업데이트)
                data = request.json
                fields, values = [], []
                for k in ['name', 'password', 'age', 'email']:
                    if k in data:
                        v = generate_password_hash(data[k]) if k == 'password' else data[k]
                        fields.append(f"{k}=%s")
                        values.append(v)
                values.append(user_id)
                if fields:
                    cursor.execute(f"UPDATE users SET {', '.join(fields)} WHERE user_id=%s", values)
                    conn.commit()
                return jsonify({"status": "updated"})

            elif request.method == 'DELETE':
                # 사용자 삭제 (외래키에 의해 연결된 포스트 및 댓글도 삭제됨)
                cursor.execute("DELETE FROM users WHERE user_id=%s", (user_id,))
                conn.commit()
                return jsonify({"status": "deleted"})
    finally:
        conn.close()

# [7] 게시글 작성 및 조회
# caption과 text만 저장, 이미지 업로드는 없음
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
                cursor.execute("SELECT * FROM posts WHERE user_id=%s ORDER BY created_at DESC", (user_id,))
            else:
                cursor.execute("SELECT * FROM posts ORDER BY created_at DESC")
            return jsonify(cursor.fetchall())
    finally:
        conn.close()

# [8] 댓글 등록
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

# [9] 댓글 조회
@app.route('/posts/<int:post_id>/comments', methods=['GET'])
def get_comments(post_id):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("SELECT * FROM comments WHERE post_id=%s ORDER BY created_at", (post_id,))
            return jsonify(cursor.fetchall())
    finally:
        conn.close()

# [10] 팔로우 요청 보내기
@app.route('/follow/request', methods=['POST'])
def follow_request():
    data = request.get_json()
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("SELECT * FROM follows WHERE follower_id=%s AND followee_id=%s", (data['from_user_id'], data['to_user_id']))
            if cursor.fetchone():
                return jsonify({"status": "already_following"})

            cursor.execute("INSERT INTO follows (follower_id, followee_id) VALUES (%s, %s)", (data['from_user_id'], data['to_user_id']))
            conn.commit()
            return jsonify({"status": "requested"})
    finally:
        conn.close()

# [11] 팔로우 요청 수락 / 거절
@app.route('/follow/respond', methods=['POST'])
def follow_respond():
    data = request.get_json()
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            if data['action'] == 'accept':
                cursor.execute("UPDATE follows SET status='accepted' WHERE follow_id=%s", (data['request_id'],))
                conn.commit()
                return jsonify({"status": "accepted"})
            elif data['action'] == 'reject':
                cursor.execute("DELETE FROM follows WHERE follow_id=%s", (data['request_id'],))
                conn.commit()
                return jsonify({"status": "rejected"})
            return jsonify({"status": "failed", "reason": "invalid action"})
    finally:
        conn.close()

# [12] 받은 팔로우 요청 목록
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

# [13] 내가 팔로우한 사용자 목록
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

# [14] DM 보내기
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

# [15] DM 대화 전체 조회
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

# [16] DM 삭제
@app.route('/dm/<int:message_id>', methods=['DELETE'])
def delete_dm(message_id):
    conn = get_connection()
    try:
        with conn.cursor() as cursor:
            cursor.execute("DELETE FROM messages WHERE message_id=%s", (message_id,))
            conn.commit()
            return jsonify({"status": "deleted"})
    finally:
        conn.close()

# [17] Flask 개발 서버 실행
# 개발 환경에서는 debug=True로 설정해 변경 사항 자동 반영 가능
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
