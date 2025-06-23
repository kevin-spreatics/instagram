from flask import Flask, request
import pymysql

def get_connection():
    return pymysql.connect(
        host='localhost',
        user='root',
        db='create_account',
        password='gmdtm457^^',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )

app = Flask(__name__)
#포스트 올리기
@app.route('/users/<user_id>/posts', methods=['POST'])
def upload_post(user_id):
    data = request.get_json()

    title = data.get('title')
    text = data.get('text')

    if (title is None) or (text is None):
        return {"status": "failed", "reason": "Enter the post title and content."}

    conn = get_connection()

    with conn.cursor() as cursor:
        sql = """insert into posts(title, text, user_id) 
                 values(%s, %s, %s);"""
        cursor.execute(sql, (title, text, user_id))
        conn.commit()

        return {"status": "post_success", "user_id": user_id, "title": title, "text": text}

# 올라온 포스트 조회하기
@app.route('/users/<user_id>/posts/<post_id>')
def select_post(user_id, post_id):
    conn = get_connection()
    
    with conn.cursor() as cursor:
        sql = """select *
                 from posts
                 where user_id = %s
                    and post_id = %s;"""
        cursor.execute(sql, (user_id, post_id))

        rows = cursor.fetchall()
        print(rows[0])

        return {"status": "post_selected", "user_id": user_id, "post_id": post_id, "title": rows[0]['title'], "text": rows[0]['text'], "create_at": rows[0]['created_at']}

# 포스트에 커맨트 달기
@app.route('/users/<user_id>/posts/<post_id>/comments', methods=['POST'])
def comment(user_id, post_id):
    data = request.get_json()

    text = data.get('text')
    if text is None:
        return {"status": "failed", "reason": "Please, enter the comment text."}
    
    conn = get_connection()

    with conn.cursor() as cursor:
        sql = """insert into comments(user_id, post_id, text) 
                 values(%s, %s, %s);"""
        cursor.execute(sql, (user_id, post_id, text))
        conn.commit()

        return {"status": "created", "user_id": user_id}

# 포스트에 달린 커맨트 조회하기
@app.route('/users/<user_id>/posts/<post_id>/comments')
def select_comment(user_id, post_id):

    conn = get_connection()

    with conn.cursor() as cursor:
        sql = """select *
                 from comments
                 where post_id = %s
                    and user_id = %s;"""
        cursor.execute(sql, (post_id, user_id))
        rows = cursor.fetchall()

        return {"status": "selected", "user_id": user_id, "post_id": post_id, "comment_id": rows[0]['comment_id'], "text": rows[0]['text'], "created_at": rows[0]['created_at']}

app.run(debug=True, host='0.0.0.0', port=5000)