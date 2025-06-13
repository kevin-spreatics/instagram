from flask import Flask, request

app = Flask(__name__)
@app.route('/users', methods=['POST'])
def create_user():
    
    data = request.get_json()
    print(data)

    nickname = data['nickname']
    name = data['name']
    password = data['password']
    age = data.get('age')
    email = data.get('email')

    user_id = 105

    return {
        'status': 'created',
        'user_id': user_id,
        'email': email
    }

app.run(debug=True, host='0.0.0.0', port=5001)