from flask import Flask, request, jsonify
from werkzeug.security import generate_password_hash, check_password_hash

app = Flask(__name__)

users = {}
posts = []
comments = []
follows = []
follow_requests = []
dms = []

user_id_counter = 100
post_id_counter = 3000
comment_id_counter = 500
dm_id_counter = 9000
follow_request_id_counter = 2000

# 사용자 생성 및 검색
@app.route('/users', methods=['GET', 'POST'])
def search_users():
    global user_id_counter
    if request.method == 'POST':
        data = request.get_json()
        if 'nickname' not in data or 'name' not in data or 'password' not in data or 'email' not in data:
            return jsonify({"error": "Missing required fields"}), 400

        if data['nickname'] in users:
            return jsonify({"error": "Nickname already exists"}), 409

        user = {
            "user_id": user_id_counter,
            "nickname": data['nickname'],
            "name": data['name'],
            "password": generate_password_hash(data['password']),
            "email": data['email']
        }
        users[data['nickname']] = user
        user_id_counter += 1
        return jsonify({"status": "created", "user": user}), 201

    keyword = request.args.get('keyword', '')
    result = [
        {
            "user_id": user['user_id'],
            "nickname": user['nickname'],
            "profile_image": "https://cdn.example.com/profiles/default.jpg"
        }
        for user in users.values()
        if keyword.lower() in user['nickname'].lower() or keyword.lower() in user['name'].lower()
    ]
    return jsonify(result)

# 사용자 로그인
@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    nickname = data.get("nickname")
    password = data.get("password")
    user = users.get(nickname)
    if not user:
        return jsonify({"error": "User not found"}), 404
    if check_password_hash(user['password'], password):
        return jsonify({"status": "login success", "user_id": user['user_id'], "nickname": user['nickname']})
    return jsonify({"error": "Invalid credentials"}), 401

# 사용자 정보 조회
@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    for user in users.values():
        if user['user_id'] == user_id:
            return jsonify(user)
    return jsonify({"error": "User not found"}), 404

# 사용자 정보 수정
@app.route('/users/<int:user_id>', methods=['PUT'])
def update_user(user_id):
    data = request.json
    for user in users.values():
        if user['user_id'] == user_id:
            user.update({k: v for k, v in data.items() if k in ['name', 'password', 'age', 'email']})
            return jsonify({"status": "updated"})
    return jsonify({"error": "User not found"}), 404

# 사용자 삭제
@app.route('/users/<int:user_id>', methods=['DELETE'])
def delete_user(user_id):
    for nickname, user in list(users.items()):
        if user['user_id'] == user_id:
            del users[nickname]
            return jsonify({"status": "deleted"})
    return jsonify({"error": "User not found"}), 404

# 포스트 올리기
@app.route('/posts', methods=['POST'])
def create_post():
    global post_id_counter
    data = request.get_json()
    post = {
        "post_id": post_id_counter,
        "user_id": data["user_id"],
        "content": data["content"],
        "created_at": "2025-06-15T10:00:00"
    }
    posts.append(post)
    post_id_counter += 1
    return jsonify({"status": "post created", "post_id": post["post_id"]}), 201

# 포스트 목록 조회
@app.route('/posts', methods=['GET'])
def get_posts():
    user_id = int(request.args.get('user_id'))
    user_posts = [p for p in posts if p['user_id'] == user_id]
    return jsonify(user_posts)

# 포스트에 댓글 달기
@app.route('/posts/<int:post_id>/comment', methods=['POST'])
def add_comment(post_id):
    global comment_id_counter
    data = request.json
    comment = {
        "comment_id": comment_id_counter,
        "post_id": post_id,
        "user_id": data['user_id'],
        "nickname": next((u['nickname'] for u in users.values() if u['user_id'] == data['user_id']), ''),
        "comment": data['comment'],
        "created_at": "2025-06-13T16:00:00"
    }
    comments.append(comment)
    comment_id_counter += 1
    return jsonify({"status": "created", "comment_id": comment["comment_id"]})

# 포스트의 댓글 목록
@app.route('/posts/<int:post_id>/comments', methods=['GET'])
def get_comments(post_id):
    result = [c for c in comments if c['post_id'] == post_id]
    return jsonify(result)

# 팔로우 신청
@app.route('/follow/request', methods=['POST'])
def follow_request():
    global follow_request_id_counter
    data = request.json
    from_id, to_id = data['from_user_id'], data['to_user_id']
    if any(f["from_user_id"] == from_id and f["to_user_id"] == to_id for f in follows):
        return jsonify({"status": "already_following"})
    request_data = {
        "request_id": follow_request_id_counter,
        "from_user_id": from_id,
        "to_user_id": to_id
    }
    follow_requests.append(request_data)
    follow_request_id_counter += 1
    return jsonify({"status": "requested"})

# 팔로우 요청 목록 조회
@app.route('/follow/requests', methods=['GET'])
def get_follow_requests():
    user_id = int(request.args.get('user_id'))
    result = []
    for fr in follow_requests:
        if fr["to_user_id"] == user_id:
            from_user = next(u for u in users.values() if u["user_id"] == fr["from_user_id"])
            result.append({
                "request_id": fr["request_id"],
                "from_user_id": from_user["user_id"],
                "nickname": from_user["nickname"],
                "profile_image": "https://cdn.example.com/profiles/default.jpg"
            })
    return jsonify(result)

# 팔로우 수락/거절
@app.route('/follow/respond', methods=['POST'])
def follow_respond():
    data = request.json
    request_id = data['request_id']
    action = data['action']
    req = next((r for r in follow_requests if r['request_id'] == request_id), None)
    if not req:
        return jsonify({"status": "failed", "reason": "request not found"}), 404
    follow_requests.remove(req)
    if action == "accept":
        follows.append({
            "from_user_id": req['from_user_id'],
            "to_user_id": req['to_user_id']
        })
        return jsonify({"status": "accepted"})
    elif action == "reject":
        return jsonify({"status": "rejected"})
    return jsonify({"status": "failed", "reason": "invalid action"}), 400

# 팔로우한 목록 조회
@app.route('/follow/list', methods=['GET'])
def get_following():
    user_id = int(request.args.get('user_id'))
    following = [
        {
            "user_id": f["to_user_id"],
            "nickname": next(u["nickname"] for u in users.values() if u["user_id"] == f["to_user_id"]),
            "profile_image": "https://cdn.example.com/profiles/default.jpg"
        }
        for f in follows if f["from_user_id"] == user_id
    ]
    return jsonify(following)

# DM 보내기
@app.route('/dm', methods=['POST'])
def send_dm():
    global dm_id_counter
    data = request.json
    dm = {
        "dm_id": dm_id_counter,
        "from_user_id": data["from_user_id"],
        "to_user_id": data["to_user_id"],
        "message": data["message"],
        "sent_at": "2025-06-13T16:30:00"
    }
    dms.append(dm)
    dm_id_counter += 1
    return jsonify({"status": "sent", "dm_id": dm["dm_id"]})

# DM 대화 조회
@app.route('/dm/conversation', methods=['GET'])
def get_dm_conversation():
    uid1 = int(request.args.get("user1_id"))
    uid2 = int(request.args.get("user2_id"))
    conv = [dm for dm in dms if {dm["from_user_id"], dm["to_user_id"]} == {uid1, uid2}]
    return jsonify(conv)

# DM 삭제
@app.route('/dm/<int:dm_id>', methods=['DELETE'])
def delete_dm(dm_id):
    for dm in list(dms):
        if dm["dm_id"] == dm_id:
            dms.remove(dm)
            return jsonify({"status": "deleted"})
    return jsonify({"status": "failed", "reason": "DM not found"}), 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
