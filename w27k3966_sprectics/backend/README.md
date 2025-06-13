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
  "nickname": "sohee",
  "name": "shinsohee",
  "password": "1234",
  "email": "sohee.spreatics@gmail.com"
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
  "user_id": 105
}

{
  "status": "failed",
  "reason": "nickname, sohee is duplicated"
}
~~~
## 사용자 인증 (로그인)

1. Endpoint
   - POST /users/login

2. Request body
   - nickname (string): 사용자 nickname, 필수
   - password (string): 비밀번호, 필수
~~~
{
  "nickname": "sohee",
  "password": "1234"
}
~~~

3. Description
   - 사용자 nickname과 password 입력
   - 틀릴 경우 Error
   - 
4. Response body
   - status (string): success, failed
   - user_id (int) : 로그인 성공한 사용자 id
   - reason (string): 실패 시, 실패 원인
   
~~~
{
 "status" : "success",
 "user_id" : 105
}
{
  "status" : "failed",
  "reason" : "nickname ERROR"
}
{
  "status" : "failed",
  "reason" : "password ERROR"
}
{
  "status": "failed",
  "reason": "Missing required field: password"
}
~~~

## 사용자 정보 조회
1. Endpoint
   - GET /users/<user_id>

2. Request Body
   -user_id (int) : 조회하려는 사용자 id

3. Description
   - 특정 user_id에 해당하는 사용자의 정보를 조회
   - 
4. Response body
   - status (string): success, failed
   - user (object) : 사용자 정보 객체 
   - reason (string): 실패 시, 실패 원인

~~~
{
 "status" : "success",
 "user" : {
    "user_id": 105,
    "nickname": "sohee",
    "name": "shinsohee",
    "email": "sohee.spreatics@gmail.com"
  }
}
{
  "status": "failed",
  "reason": "User not found"
}

~~~

## 사용자 정보 수정
1. Endpoint
   - PUT /users/<user_id>
   
2. Request params
   -user_id (int) : 수정하려는 사용자 id
   
3. Request body  
   - nickname (string, optional): 새로운 nickname  
   - name (string, optional): 새로운 이름  
   - password (string, optional): 새로운 비밀번호  
   - email (string, optional): 이메일 변경

4. Description  
   - 특정 user_id에 해당하는 사용자의 정보를 수정 
   - body에 포함된 항목만 수정되며, 입력하지 않은 필드는 유지
~~~
{
 "nickname" : "newsohee",
  "password" : "13579"
}
~~~
5. Response body
   - status (string): success, failed
   - reason (string): 실패 시, 실패 원인

~~~
{
  "status": "success"
}
{
  "status": "failed",
  "reason": "User not found"
}
~~~
## 사용자 삭제
1. Endpoint
   - DELETE /users/<user_id>
     - user_id (int): 삭제할 사용자 id
2. Request body 
   - 없음
4. Description
   - user_id에 해당하는 사용자 계정을 삭제
   - user_id가 없으면 삭제가 실패
5. Response body
   - status (string): deleted, failed
   - reason (string): 실패시, 실패 원인
~~~
{
  "status": "failed",
  "reason": "user_id, User not found"
}
{
  "status": "failed",
  "reason": "Invalid password format"
}
~~~

# 포스팅
## 게시글 생성

1. Endpoint  
   - POST /posts

2. Request body  
   - title (string): 게시글 제목, 필수  
   - text (string): 게시글 본문 내용, 필수  
   - user_id (int): 작성자 ID, 필수

3. Description  
   - 사용자가 새 게시글을 작성
   - 요청한 `user_id`의 사용자가 작성자로 기록

~~~
{
  "title": "오늘의 일기",
  "text": "우래옥 웨이팅 내앞에 40팀",
  "user_id": 105
}
~~~

4. response body
   - status(string) : updated, failed
   - post_id(int) : 생성된 게시글
   - reason(string) : 실패 원인
   
~~~  
{
  "status": "created",
  "post_id": 101
}
{
  "status": "failed",
  "reason": "Missing required field: text"
}
{
  "status": "failed",
  "reason": "User_id not found"
}
~~~
## 올라온 포스트 조회하기

1. Endpoint  
   - GET /posts
2. Request body
   - user_id (int, optional): 특정 사용자의 포스트만 조회
3. Description
   - 전체 또는 특정 사용자의 포스트 목록을 조회
  
4. Response body

   - status (string): "success" 또는 "failed"
   - posts (array of objects): 
      {post_id (int)
       user_id (int)
       text (string)
       comment_count(int)
       like_count (int)}
   - reason (string): 실패 시 원인

~~~
{
  "status": "success",
  "posts": [ { /* ... */ } ]
}
~~~

## 포스트의 커맨드 조회하기
1. Endpoint
   - GET /posts/{post_id}/comments
2. Request body
   -없음
3. Description
   - 지정된 post_id의 커맨트 목록을 조회한다.
4. Response body
   - status (string): "success" 또는 "failed"
   - comments(arry):
     {
      post_id (int)
      comment_id (int)
      user_id (int)
      text (string)
     }
   - reason (string): 실패 시 원인
~~~
{
  "status": "success",
  "comments": [ { /* ... */ } ]
}
~~~

## 특정 포스트의 커맨드 달기
1. Endpoint
   - POST /posts/{post_id}/comments
2. Request body
   - user_id (string) : post 작성한 사용자의 id, 필수
3. Description
   - 지정된 post_id의 커맨트 작성한다.
4. Response body
   - status (string): "success" 또는 "failed"
   - comments(array):
     {
      post_id (int)
      comment_id (int)
      user_id (int)
      text (string)
     }
   - reason (string): 실패 시 원인
~~~
{
  "status": "success",
  "comments": [ { /* ... */ } ]
}
~~~

# 소셜
## 다른 사용자 조회

1. Endpoint
   - GET /users
2. Query parameters
   - search (string, optional): 닉네임 또는 이름 검색어
3. Description
   - 사용자 목록을 조회한다.
4. Response body
   - status (string): "success" 또는 "failed"
   - users (array): [{ user_id, nickname, name }]
  
~~~
{
  "status": "success",
  "users": [ { /* ... */ } ]
}
~~~

## 팔로우 신청

1. Endpoint
   -POST /follow_requests

2. Request body
   - follower_no( int, required): 요청자 user_id
   - followee_no( int, required): 대상 user_id
~~~
{
  "follower_no": 105,
  "followee_no": 110
}
~~~
3. Description
   - 팔로우 요청을 생성한다.
4. Response body
   - status (string): "success" 또는 "failed"
   - request_no (int): 생성된 요청 번호
   - reason (string): 실패 시 원인
  
~~~
{
  "status": "created",
  "request_no": 55
}
~~~


## 팔로우한 목록 조회

1. Endpoint
   - GET /users/{user_no}/following
2. Request body
   - 없음
3. Description
   - user_id가 팔로우한 사용자 목록을 반환
4. Response body
   - status (string): "success" 또는 "failed"
   - following (array of objects): [{ user_no, nickname, name }]
  
~~~
{
  "status": "success",
  "following": [ { /* ... */ } ]
}
~~~

## 자신에게 팔로우 요청한 목록 조회

1. Endpoint
   - GET /users/{user_no}/follow_requests
2. Request body
   - 없음
3. Description
   - user_id에게 온 팔로우 요청 목록을 반환한다.
4. Response body
   - status (string): "success" 또는 "failed"
   - requests (array of objects): [{ request_no, follower_no, created_at }]
  
~~~
{
  "status": "success",
  "following": [ { /* ... */ } ]
}
~~~

## 팔로우 수락/거절

1. Endpoint
   - PATCH /follow_requests/{request_no}
2. Request body
   - action (string, required): "accept" 또는 "reject"
~~~
{
  "action": "accept"
}
~~~
3. Description
   - 해당 팔로우 요청을 수락하거나 거절한다.
4. Response body
   - status (string): "updated" 또는 "failed"
   - reason (string): 실패 시 원인
  
~~~
{
  "status": "updated"
}
~~~

# 메세지
## DM 보내기

1. Endpoint
   - POST /messages
2. Request body
   - sender_no (int, required): 발신자 user_id
   - receiver_id (int, required): 수신자 user_id
   - content (string, required): 메시지 내용 
~~~
{
  "sender_no": 105,
  "receiver_no": 110,
  "content": "안녕"
}
~~~
3. Description
   - 새로운 DM을 생성하여 전송한다.

4. Response body
   - status (string): "success" 또는 "failed"
   - message_no (int): 생성된 메시지 번호
   - reason (string): 실패 시 원인
  
~~~
{
  "status": "success",
  "message_no": 200
}
~~~

## DM 조회하기
1. Endpoint
   - GET /messages
2. Query parameters
   - user_no (int, required): 조회할 사용자 user_id
   - with (int, optional): 특정 사용자와의 대화 상대 user_id
   - page (int, optional): 페이지 (default: 1)
   - limit (int, optional): 페이지당 개수 (default: 30)
3. Description
- 해당 사용자와 전체 또는 특정 상대방 간 DM 목록을 조회한다.
4. Response body
   - status (string): "success" 또는 "failed"
   - messages (array): [{ message_no, sender_no, receiver_no, content, created_at }]
~~~
{
  "status": "success",
  "messages": [ { /* ... */ } ]
}
~~~

#DM 삭제하기
1. Endpoint
   - DELETE /messages/{message_no}
2. Request body
   - 없음
3. Description
   - 지정된 message_no의 DM을 삭제한다.
4. Response body
   - status (string): "deleted" 또는 "failed"
   - reason (string): 실패 시 원인
~~~
{ //성공시
  "status": "deleted"
}
{ // 실패시
  "status": "failed"
}
~~~

