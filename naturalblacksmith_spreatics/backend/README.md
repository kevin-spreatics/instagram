# REST API Spec.
- version 0.1 (2025/6/11)
## user 
## 사용자 생성
1. Endpoint
   - POST /user/create
2. Request body 
   - nickname (string): 사용자 nickname (required)
   - name (string): 사용자 이름, (required)
   - pw (string): 비밀번호, (required)
   - age (int, optional): 사용자 나이
   - email (string, optional): 사용자 email 주소
~~~
{
  "nickname": "lemon",
  "pw": "1234",
  "name": "이승학",
  "email": "kevin.spreatics@gmail.com"
}
~~~
4. Description
   - 사용자 계정을 생성한다. nickname과 name, password는 필수 입력값이다.
   - nickname은 고유한 값이며 기존 사용자와 중복되면 생성이 실패한다.
5. Response body
   - status (string): user create success, user create failed
   - new user id(int): 생성 성공 시, user_id 반환
   - reason (string): 실패 시, 실패 원인
~~~
# 생성 성공 시
{
"status" : "user create success",
"new user id" : 7
}

# 생성 실패시 - 필수 입력값 부재
{
"status" : "failed",
"reason" : "nickname, password, name is mandatory."
}

# 생성 실패시 - 기타 오류
{
"status": "user create failed", 
"reason": str(e)
}
~~~
## 로그인 
1. Endpoint
   - POST /user/login
2. Request body 
   - nickname: 사용자 nickname (required)
   - pw: 사용자 비밀번호 (required) 
4. Description
   - nickname과 pw로 user_id가 존재하는지 조회한다 
   - user_id가 있다면 로그인 성공 
   - nickname, pw는 둘 다 입력 필요
5. Response body
   - status (string): login success, login failed
   - login user: 성공시, 로그인한 유저의 유저 id
   - reason (string): 실패시, 실패 원인
~~~
#  로그인 성공 시 
{
"status" : "login success",
"login user" : row['user_id']
}

# 로그인 실패 시 - nickname 또는 password 입력 부재
{
"status" : "login error",
"reason" : "nickname and password required"
}

# 로그인 실패 시 - nickname과 password와 매칭되는 user_id 부재 (회원 가입 안 한 채로 로그인한 상태)
{
 "status": "login failed",
"reason": "incorrect nickname or password"
}

# 로그인 실패 시 - 기타 오류
{
   "status": "login failed",
   "reason : str(e)
 }
~~~

## 사용자 비밀번호 변경
1. Endpoint
   - PATCH /user/<int:user_id>/password_change
   - user_id: 현재 로그인한 사용자의 id
2. Request body 
   - pw: 사용자 기존 비밀번호
   - new_pw: 새 비밀번호
4. Description
   - 이미 로그인한 경우에만 호출 가능 (global 변수 user_id 확인)
   - endpoint로 받은 user id의 비밀번호를 확인한다
   - json으로 받아온 pw와 user_id 일치 시 new_pw로 비밀번호를 변경한다 
5. Response body
   - status (string): password change success, password change failed
   - reason (string): 실패시, 실패 원인
~~~
# 비밀번호 변경 성공시
{
"status" : "password change success"
}

# 비밀번호 변경 실패시 - 기존 비밀번호 또는 새 비밀번호 입력 부재
{
"status": "password change failed",
"reason": "password and new password required"
}

# 비밀번호 변경 실패 시 - 기존 비밀번호 불일치
{
"status" : "password change failed",
"reason" : "incorrect password"
}

# 비밀번호 변경 실패 시 - 기타 오류 
{
   "status" : "password change failed",
   "reason" : str(e)
}

~~~
## 사용자 조회 
1. Endpoint
   - GET /user/check
2. Request body 
   - user_id : 조회할 사용자의 user_id
4. Description
   - user_id에 해당하는 사용자를 찾는다 
   - 해당 사용자의 nickname, password, name 값을 return 한다.
5. Response body
   - status (string): user get success, user get failed
   - nickname (string) : 성공시, 조회된 사용자의 nickname
   - password (string) : 성공시, 조회된 사용자의 비밀번호
   - name (string) : 성공시, 조회된 사용자의 이름 
   - reason (string): 실패시, 실패 원인
~~~
# 사용자 조회 성공시 
{
"status" : "delete success",
"nickname" : lemonwater,
"password" : lemonwater2233,
"name" : lemony
}

# 사용자 조회 실패시 - user_id가 일치하는 사용자 없음 
{
"status" : "user get failed",
"error" : "user not found"
}

# 사용자 조회 실패시 - 기타 오류
{
"status" : "user get failed",
"error" : str(e)
}
~~~
## 사용자 삭제 (id 사용)
1. Endpoint
   - DELETE /user/<int:user_id>/delete
   - user_id: 현재 로그인한 사용자의 id
2. Request body 
   없음
4. Description
   - user_id에 해당하는 사용자 계정을 삭제한다.
   - user_id가 없으면 삭제가 실패한다.
5. Response body
   - status (string): delete success, delete failed
   - deleted user (int) : 성공시, 삭제된 user_id
   - reason (string): 실패시, 실패 원인
~~~
# 사용자 삭제 성공시 
{
"status" : "delete success",
"deleted user" : user_id
}

# 사용자 삭제 실패 시 - 해당 user_id를 가진 user 없음
{
"status" : "delete fail",
"error" : "user not found"
}

# 사용자 삭제 실패 시 - 기타 오류
{
"status" : "delete fail",
"error" : str(e)
}

~~~
## 사용자 삭제 (nickname 사용)
1. Endpoint
   - DELETE /user/<string:nickname>/delete
   - nickname: 현재 로그인한 사용자의 nickname
2. Request body 
   없음
4. Description
   - nickname에 해당하는 사용자 계정을 삭제한다.
   - nickname이 없으면 삭제가 실패한다.
5. Response body
   - status (string): delete success, delete failed
   - deleted user (int) : 성공시, 삭제된 user_id
   - reason (string): 실패시, 실패 원인
~~~
# 사용자 삭제 성공시 
{
"status" : "delete success"
}

# 사용자 삭제 실패 시 - 해당 user_id를 가진 user 없음
{
"status" : "delete fail",
"error" : "user not found"
}

# 사용자 삭제 실패 시 - 기타 오류
{
"status" : "delete fail",
"error" : str(e)
}
~~~
# posting
## 포스팅 생성
1. Endpoint
    - POST /posting/<int:user_id>/create
    - user_id: 로그인한 user_id 

2. Request body
- post title: 포스팅 제목 (required)
- post_text: 포스팅 내용 (required)

3. Description
- posting title, text를 입력하여 로그인한 user_id로 post 하나를 생성

5. Response body
 - status: posting success, posting failed
 - new post id: 새롭게 생성된 user id
 - reason: 실패시, 실패 요인 

~~~
# 포스팅 생성 성공시 
{
"status" : "posting success",
"new post id" : 12
}

# 포스팅 생성 실패 시 - 기타 오류
{
"status" : "delete fail",
"error" : str(e)
}

~~~

## 포스팅 삭제 
1. Endpoint
    - DELETE /posting/delete

2. Request body
- post_id: 삭제할 포스팅 id (required)

3. Description
- posting id를 입력하여 해당 게시글을 삭제 

5. Response body
 - status: posting delete success, posting delete failed
 - reason: 실패시, 실패 요인 

~~~
# 포스팅 삭제 성공시 
{
"status" : "posting delete success"
}

# 포스팅 삭제 실패 시 - 입력한 post_id에 해당하는 포스팅이 없음 
{
"status" : "delete fail",
"error" : "no title was found"
}

# 포스팅 삭제 실패 시 - 기타 오류
{
"status" : "delete fail",
"error" : str(e)
}
~~~

## 포스팅 전체 조회 
1. Endpoint
    - GET /posting/<int:user_id>/check
    - user_id: 현재 로그인한 사용자의 user_id

2. Request body
- 없음

3. Description
- 현재 로그인한 사용자 계정으로 포스팅한 포스트를 조회

5. Response body
 - status: posting check success, posting check failed
 - post_id: 조회된 포스트의 post_id
 - title: 조회된 각 포스트의 제목
 - text: 조회된 각 포스트의 내용 
~~~
# 포스팅 조회 성공시 
{
"status" : "posting delete success"
"post_id" : 12,
"title" : lemonbutter,
"text" : lemon is the best!! 
}
{
"status" : "posting delete success"
"post_id" : 14,
"title" : lemonday,
"text" : today is the day of lemon!
}
{
"status" : "posting delete success"
"post_id" : 20,
"title" : lemon day 2,
"text" : tomorrow will be the day of lemon!
}

# 포스팅 조회 실패 시 - 현재 계정으로 생성한 포스팅이 없음 
{
"status" : "delete fail",
"error" : "post not found"
}

# 포스팅 조회 실패 시 - 기타 오류
{
"status" : "delete fail",
"error" : str(e)
}

~~~
