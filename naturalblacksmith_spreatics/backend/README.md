# REST API Spec.
- version 0.1 (2025/6/11)
# 사용자 계정
## 사용자 생성
1. Endpoint
   - POST /users
2. Request body 
   - nickname (string): 사용자 nickname, 필수
   - name (string): 사용자 이름, 필수
   - password (string): 비밀번호, 필수
   - age (int, optional): 사용자 나이
   - email (string, optional): 사용자 email 주소
~~~
{
  "nickname": "kevin",
  "name": "이승학",
  "password": "1234",
  "email": "kevin.spreatics@gmail.com"
}
~~~
4. Description
   - 사용자 계정을 생성한다. nickname과 name, password는 필수 입력값이다.
   - nickname은 고유한 값이며 기존 사용자와 중복되면 생성이 실패한다.
5. Response body
   - status (string): created, failed
   - user_id (int): 생성 성공 시, user_id 반환
   - reason (string): 실패 시, 실패 원인
~~~
{
  "status": "created",
  "user_id": 1
}

{
  "status": "failed",
  "reason": "nickname, kevin is already taken. choose a different nickname"
}
~~~
## 사용자 삭제
1. Endpoint
   - DELETE /users/<user_id>
     - user_id (int): 삭제할 사용자 id
2. Request body 
   - 없음
4. Description
   - user_id에 해당하는 사용자 계정을 삭제한다.
   - user_id가 없으면 삭제가 실패한다.
5. Response body
   - status (string): deleted, failed
   - reason (string): 실패시, 실패 원인
~~~
{
  "status": "failed",
  "reason": "user_id, 101 doesn't exist"
}
~~~
# 포스팅
1. Endpoint
    - POST /post/
    - user_id (int): 포스팅 올리는 user_id

2. Request body
- post title: 포스팅 제목 (required)
- post_text: 포스팅 내용 (required)
- posting date: 포스팅 한 날짜 (required)

3. Description 
- posting date, post_text은 필수 입력 
- post title는 선택 

5. Response body
 - status: post 

#코멘트
1. Endpoint
    - POST /comment/<user_id>/<post_id>/<text>
    - user_id (int): 포스팅 올리는 user_id

2. Request body
- post title: 포스팅 제목 (required)
- post_text: 포스팅 내용 (required)
- posting date: 포스팅 한 날짜 (required)

3. Description 
- posting date, post_text은 필수 입력 
- post title는 선택 

5. Response body


# 소셜
1. Endpoint
    - POST /follow/
    - user_id (int): 포스팅 올리는 user_id

2. Request body
- post title: 포스팅 제목 (required)
- post_text: 포스팅 내용 (required)
- posting date: 포스팅 한 날짜 (required)

3. Description 
- posting date, post_text은 필수 입력 
- post title는 선택 

5. Response body


# 메시지
1. Endpoint
    - POST /DM/
    - user_id (int): 포스팅 올리는 user_id

2. Request body
- post title: 포스팅 제목 (required)
- post_text: 포스팅 내용 (required)
- posting date: 포스팅 한 날짜 (required)

3. Description 
- posting date, post_text은 필수 입력 
- post title는 선택 

5. Response body

