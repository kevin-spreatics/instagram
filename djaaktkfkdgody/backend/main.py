from flask import Flask

app = Flask(__name__)
@app.route('/hello')
def hello():
    print("Endpoint /hello was accessed")
   # return "hello"
    return { 'message' : 'hello'}

@app.route('/hello/<name>')
def hello_name(name):
    print(f'hello {name}')
    return {'message':'hello', 'name':name}

@app.route('/users/<user_id>/dms')
def dms(user_id):
    return {'message':'dms', 'user_id':user_id}

@app.route('/users/<follower_id>/follow/<followee_id>', methods=['POST'])
def follow(follower_id, followee_id):
    return {
            'message':'follow',
            'follower_id':follower_id,
            'followee_id':followee_id
            }

@app.route('/users/<user_id>')
def user_idd(user_id):
    print(f'user_idd {user_id}')
    return {'user_idd':user_id}

@app.route('/students/<student_id>')
def stu_id(student_id):
    return {'student_id' : student_id}

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
