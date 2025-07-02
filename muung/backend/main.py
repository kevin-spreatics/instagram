from flask import Flask, request, jsonify
import pymysql
from datetime import datetime 
app = Flask(__name__)
def get_connection():
    return pymysql.connect(
        # 데이터베이스 정보
        host='database-1.cts2qeeg0ot5.ap-northeast-2.rds.amazonaws.com',
        # 데이터베이스 사용자 정보
        user='kevin',
        # 데이터베이스 비밀번호
        password='spreatics*',
        # 데이터베이스 이름
        db='insta_doubled',
        # 데이터베이스 인코딩
        charset='utf8mb4',
        # 데이터베이스 커서 클래스
        cursorclass=pymysql.cursors.DictCursor
     )

# 헬로월드
@app.route('/hello')
def home():
    return "Hello, World!"

# 회원가입
@app.route('/signup', methods=['POST'])
# 회원가입 함수
def signup():
    data = request.json
    nickname = data.get('nickname')
    name = data.get('name')
    email = data.get('email')
    age = data.get('age')
    password = data.get('password')
    confirm_password = data.get('confirm_password')
    
    # 회원가입 데이터 확인
    if not nickname or not name or not email or not age or not password or not confirm_password:
        return jsonify({'error': '모든 필드를 입력해주세요.'}), 400

    # 비밀번호 확인
    if password != confirm_password:
        return jsonify({'error': '비밀번호가 일치하지 않습니다.'}), 400

    # 회원가입 데이터베이스 저장
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "INSERT INTO users (nickname, name, email, age, password) VALUES (%s, %s, %s, %s, %s)"
                cursor.execute(sql, (nickname, name, email, age, password))
                conn.commit()
                return jsonify({'message': '회원가입이 완료되었습니다.'}), 200 
    # 회원가입 데이터베이스 저장 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 로그인
@app.route('/login', methods=['POST'])
# 로그인 함수
def login():
    data = request.json
    email = data.get('email')
    password = data.get('password')
    
    # 로그인 데이터 확인
    if not email or not password:
        return jsonify({'error': '이메일과 비밀번호를 입력해주세요.'}), 400
    
    # 로그인 데이터베이스 조회
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                # 로그인 데이터베이스 조회
                sql = "SELECT * FROM users WHERE email = %s AND password = %s"
                cursor.execute(sql, (email, password))
                user = cursor.fetchone()
                # 로그인 데이터베이스 조회 결과 확인
                if user:
                    return jsonify({'message': '로그인 성공', 'user': user}), 200
                else:
                    return jsonify({'error': '이메일 또는 비밀번호가 일치하지 않습니다.'}), 400
    # 로그인 데이터베이스 조회 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 사용자 조회 : 전체 user 조회하게끔 수정
@app.route('/users', methods=['GET', 'POST'])
# 사용자 조회 함수
def get_users():
    # 사용자 조회 데이터베이스 조회
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                # 사용자 조회 데이터베이스 조회
                sql = "SELECT * FROM users"
                cursor.execute(sql)
                users = cursor.fetchall()
                return jsonify({'users': users}), 200
    # 사용자 조회 데이터베이스 조회 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
        
#사용자 수정
@app.route('/users/<int:user_id>', methods=['PUT'])
# 사용자 수정 함수
def update_user(user_id):
    # 사용자 수정 데이터 받기
    data = request.json
    nickname = data.get('nickname')
    name = data.get('name')
    email = data.get('email')
    age = data.get('age')
    password = data.get('password')
    confirm_password = data.get('confirm_password')
    
    # 사용자 수정 데이터 확인
    if not nickname or not name or not email or not age or not password or not confirm_password:
        return jsonify({'error': '모든 필드를 입력해주세요.'}), 400
    
    # 비밀번호 확인
    if password != confirm_password:
        return jsonify({'error': '비밀번호가 일치하지 않습니다.'}), 400
    
    # 사용자 수정 데이터베이스 수정
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "UPDATE users SET nickname = %s, name = %s, email = %s, age = %s, password = %s WHERE id = %s"
                cursor.execute(sql, (nickname, name, email, age, password, user_id))
                conn.commit()
                return jsonify({'message': '사용자 정보가 수정되었습니다.'}), 200
    # 사용자 수정 데이터베이스 수정 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500

#사용자 삭제
@app.route('/users/<int:user_id>', methods=['DELETE'])
# 사용자 삭제 함수
def delete_user(user_id):
    # 사용자 삭제 데이터베이스 삭제
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "DELETE FROM users WHERE id = %s"
                cursor.execute(sql, (user_id))
                conn.commit()
                return jsonify({'message': '사용자 정보가 삭제되었습니다.'}), 200
    # 사용자 삭제 데이터베이스 삭제 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 포스트 업로드
@app.route('/posts', methods=['POST'])
# 포스트 업로드 함수
def upload_post():
    # 포스트 업로드 데이터 받기
    data = request.json
    user_id = data.get('user_id')
    title = data.get('title')
    content = data.get('content')
    
    # 포스트 업로드 데이터 확인
    if not title or not content or not user_id:
        return jsonify({'error': '모든 필드를 입력해주세요.'}), 400
    
    # 포스트 업로드 데이터베이스 저장
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "INSERT INTO posts (title, content, user_id) VALUES (%s, %s, %s)"
                cursor.execute(sql, (title, content, user_id))
                conn.commit()
                return jsonify({'message': '포스트가 업로드되었습니다.'}), 200
    # 포스트 업로드 데이터베이스 저장 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 포스트 조회
@app.route('/posts', methods=['GET', 'POST'])
# 포스트 조회 함수
def get_posts():
    # 포스트 조회 데이터베이스 조회
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "SELECT * FROM posts"
                cursor.execute(sql)
                posts = cursor.fetchall()
                return jsonify({'posts': posts}), 200
    # 포스트 조회 데이터베이스 조회 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500

# 포스트 수정
@app.route('/posts/<int:post_id>', methods=['PUT'])
# 포스트 수정 함수
def update_post(post_id):
    # 포스트 수정 데이터 받기
    data = request.json
    title = data.get('title')
    content = data.get('content')
    user_id = data.get('user_id')
    
    # 포스트 수정 데이터 확인
    if not title or not content or not user_id:
        return jsonify({'error': '모든 필드를 입력해주세요.'}), 400
    
    # 포스트 수정 데이터베이스 수정
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "UPDATE posts SET title = %s, content = %s WHERE id = %s"
                cursor.execute(sql, (title, content, post_id))
                conn.commit()
                return jsonify({'message': '포스트가 수정되었습니다.'}), 200
    # 포스트 수정 데이터베이스 수정 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 포스트 삭제
@app.route('/posts/<int:post_id>', methods=['DELETE'])
# 포스트 삭제 함수
def delete_post(post_id):
    # 포스트 삭제 데이터베이스 삭제
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "DELETE FROM posts WHERE id = %s"
                cursor.execute(sql, (post_id))
                conn.commit()
                return jsonify({'message': '포스트가 삭제되었습니다.'}), 200
    # 포스트 삭제 데이터베이스 삭제 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 포스트의 코멘트 작성
@app.route('/posts/<int:post_id>/comments', methods=['POST'])
# 포스트의 코멘트 작성 함수
def create_comment(post_id):
    # 포스트의 코멘트 작성 데이터 받기
    data = request.json
    content = data.get('content')
    user_id = data.get('user_id')
    
    # 포스트의 코멘트 작성 데이터 확인
    if not content or not user_id:
        return jsonify({'error': '모든 필드를 입력해주세요.'}), 400
    
    # 포스트의 코멘트 작성 데이터베이스 저장
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "INSERT INTO comments (content, user_id, post_id) VALUES (%s, %s, %s)"
                cursor.execute(sql, (content, user_id, post_id))
                conn.commit()
                return jsonify({'message': '코멘트가 작성되었습니다.'}), 200
    # 포스트의 코멘트 작성 데이터베이스 저장 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 포스트의 코멘트 조회
@app.route('/posts/<int:post_id>/comments', methods=['GET'])
# 포스트의 코멘트 조회 함수
def get_comments(post_id):
    # 포스트의 코멘트 조회 데이터베이스 조회
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "SELECT * FROM comments WHERE post_id = %s"
                cursor.execute(sql, (post_id))
                comments = cursor.fetchall()
                return jsonify({'comments': comments}), 200
    # 포스트의 코멘트 조회 데이터베이스 조회 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
 
# 팔로우 하기
@app.route('/follows/<int:follow_id>/<int:follower_id>', methods=['POST'])
# 팔로우 하기 함수
def follow(follow_id, follower_id):
    # 팔로우 하기 데이터베이스 저장
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "INSERT INTO follows (follow_id, follower_id) VALUES (%s, %s)"
                cursor.execute(sql, (follow_id, follower_id))
                conn.commit()
                return jsonify({'message': '팔로우가 완료되었습니다.'}), 200
    # 팔로우 하기 데이터베이스 저장 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 팔로우 취소
@app.route('/follows/<int:follow_id>/<int:follower_id>', methods=['DELETE'])
# 팔로우 취소 함수
def unfollow(follow_id, follower_id):
    # 팔로우 취소 데이터베이스 삭제
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "DELETE FROM follows WHERE follow_id = %s AND follower_id = %s"
                cursor.execute(sql, (follow_id, follower_id))
                conn.commit()
                return jsonify({'message': '팔로우가 취소되었습니다.'}), 200
    # 팔로우 취소 데이터베이스 삭제 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500

# 팔로우 요청 보내기
@app.route('/follow_requests/<int:follower_id>/<int:followee_id>', methods=['POST'])
# 팔로우 요청 보내기 함수
def send_follow_request(follower_id, followee_id):
    # 팔로우 요청 보내기 데이터베이스 저장
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "INSERT INTO follow_requests (follower_id, followee_id) VALUES (%s, %s)"
                cursor.execute(sql, (follower_id, followee_id))
                conn.commit()
                return jsonify({'message': '팔로우 요청이 보내졌습니다.'}), 200
    # 팔로우 요청 보내기 데이터베이스 저장 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 팔로우 요청 수락
@app.route('/follow_requests/<int:follower_id>/<int:followee_id>', methods=['PUT'])
# 팔로우 요청 수락 함수
def accept_follow_request(follower_id, followee_id):
    # 팔로우 요청 수락 데이터베이스 수정
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "UPDATE follow_requests SET status = 'accepted' WHERE follower_id = %s AND following_id = %s"
                cursor.execute(sql, (follower_id, followee_id))
                conn.commit()
                return jsonify({'message': '팔로우 요청이 수락되었습니다.'}), 200
    # 팔로우 요청 수락 데이터베이스 수정 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 팔로우 요청 거절
@app.route('/follow_requests/<int:follower_id>/<int:followee_id>', methods=['PUT'])
# 팔로우 요청 거절 함수
def reject_follow_request(follower_id, followee_id):
    # 팔로우 요청 거절 데이터베이스 수정
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "UPDATE follow_requests SET status = 'rejected' WHERE follower_id = %s AND following_id = %s"
                cursor.execute(sql, (follower_id, followee_id))
                conn.commit()
                return jsonify({'message': '팔로우 요청이 거절되었습니다.'}), 200
    # 팔로우 요청 거절 데이터베이스 수정 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 팔로우 요청 목록 조회
@app.route('/follow_requests/<int:follower_id>', methods=['GET'])
# 팔로우 요청 목록 조회 함수
def get_follow_requests(follower_id):
    # 팔로우 요청 목록 조회 데이터베이스 조회
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "SELECT * FROM follow_requests WHERE follower_id = %s"
                cursor.execute(sql, (follower_id))
                follow_requests = cursor.fetchall()
                return jsonify({'follow_requests': follow_requests}), 200
    # 팔로우 요청 목록 조회 데이터베이스 조회 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500
    
# 팔로우 요청 목록 조회
@app.route('/follow_requests/<int:followee_id>', methods=['GET'])
# 팔로우 요청 목록 조회 함수
def get_follow_requests_by_followee(followee_id):
    # 팔로우 요청 목록 조회 데이터베이스 조회
    try:
        with get_connection() as conn:
            with conn.cursor() as cursor:
                sql = "SELECT * FROM follow_requests WHERE followee_id = %s"
                cursor.execute(sql, (followee_id))
                follow_requests = cursor.fetchall()
                return jsonify({'follow_requests': follow_requests}), 200
    # 팔로우 요청 목록 조회 데이터베이스 조회 오류
    except Exception as e:
        return jsonify({'error': str(e)}), 500






# 서버 실행
app.run(debug=True,host = '0.0.0.0', port = 5000)