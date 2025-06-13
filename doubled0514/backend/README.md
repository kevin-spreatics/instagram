# REST API Spec.
- version 0.1 (2025/6/13)
# 사용자 계정
## 사용자 생성
1. Endpoint
   - POST /users
2. Request body 
   - nickname (string): 사용자 nickname, 필수
   - password(string) : 비밀번호,필수
   - name (string): 사용자 이름, 필수
   - age (int, optional): 사용자 나이
   - email (string, optional): 사용자 email 주소
~~~
{
  "nickname": "double_d_eo",
  "name": "김현서",
  "email": "doubled0514@gmail.com"
}
~~~
4. Description
   - 사용자 계정을 생성한다. 
   - nickname은 고유한 값이며 기존 사용자와 중복되면 생성이 실패한다.
5. Response body
   - status (string): created, failed
   - nickname: 생성 성공 시, nickname 반환
   - reason (string): 실패 시, 실패 원인
~~~
{
  "status": "created",
  "nickname": "double_d_eo"
}

{
  "status": "failed",
  "reason": "nickname, double_d_eo is duplicated"
}
~~~
## 사용자 인증 (로그인)
1. Endpoint
  - POST/users/login
2. Requset body
  - nickname (string,필수) 
  - password(string,필수) : 사용자 비밀번호
~~~
{
   "nickname":"double_d_eo",
   "password" : "qwerty1234"
}
~~~
3. Description
  - 사용자 인증하여 로그인한다
  - 존재하지 않는 nickname이거나 password가 틀리면 로그인에 실패
4. Response Body
  - status(string) : success, failed
  - user_id(int): 로그인 성공시 user_id 반환
  - reason(string) : 실패시 실패 원인
~~~~
{
   "status" : "success",
   "user_id" : 105
}
{
   "status" : "failed",
   "reason" : "nickname or password is wrong"
}
~~~~
## 사용자 정보 조회
1.Endpoint
   - Get/users/<user_id>
2.Request body
   - 없음
3.Description
   - <users_id>에 대응하는 사용자 정보를 조회한다
4.Response body
   - nickname(string)
   - name(string)
   - age(int)
   - email(string)
~~~~
{
   "nickname" : "double_d_eo",
   "name" : "김현서",
   "age": "" ,
   "email" : "doubled0514@gamil.com"
}
~~~~
## 사용자 정보 수정
1. Endpoint
   - PUT/users/<user_id>
2. Request body
   - password(string,opt)
   - nickname(string,opt)
   - name(string,opt)
   - age(int,opt)
   - email(string,opt)
~~~
{
   "password":"qwerty123",
   "age" : 25
}
~~~  
3. description
   - 사용자 정보를 수정한다.
4. response body
   - status : success, failed
   - reason : 실패시, 실패 원인
~~~
{
   "status" : "success"
}
{
   "status" :"failed",
   "reason" : "age is wrong type"
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
## 포스트 올리기
1. Endpoint
   - POST/posts
2. Request body
   - user_id(int,필수) 
   - title(string,opt)
   - text(string,opt)
~~~
{
   "user_id" : 105
   "title" : "덥다",
   "text" : "에어컨이 안돼서 너무 더워요"
}
~~~
3. Description
   - user_id 의 사용자가 포스트 작성
4. Resopnse body
   - stauts : success, failed
   - post_id(int) : 성공시 post id 반환
   - reason(string) : 실패시 실패 이유
~~~
{
   "status" : "success",
   "post_id" : 1
}
{
   "staus" : "failed",
   "reason" : "you can't leave title empty"
}
~~~
## 올라온 포스트 조회하기
1. Endpoint
   - GET/posts
2. Request body
   - post_id(int,opt)
   - user_id(int,opt)
   - nickname(string,opt)
4. descriptionn
   - 전체 포스트를 조회한다
   - post_id, user_id, nickname에 기반해 조회할 수 있다
5. response body
   - [{"nickname":nickname, "title":title, "text":text},
      {"nickname":nickname, "title":title, "text":text}, ...]
   - status : success , failed
   - reason : 실패시 실패 원인 
~~~
[ { "nickname" : "double_d_eo",
   "title" : "덥다",
   "text" :"에어컨이 안돼서 너무 더워요"},
{"nickname":"sinsohee", "title":"별로 안더움","text":"여름 치고는 시원한 편"} ]

{
   "status" : "failed",
   "reason" : "cannot find posts"
}
~~~

## 포스트의 코멘트 조회하기
1. Endpoint
   - GET/posts/<post_id>/comment
2. Request body
3. Description
   - post_id에 대응하는 사용자의 포스트의 코멘트 조회
4. Respond body
   - status (string) : 실패,성공
   - reason (string) : 실패시 이유
   - [{"nickname":nickname(string), "comment":comment(string)},{"nickname":nickname(string), "comment":comment(string)}, ...]

## 특정 포스트에 커멘트 달기
1. Endpoint
   -POST/posts/<post_id>/comment
2. Request body
   -comment(string) : 커멘트
3. Description 
4. Response body
   - status(string) :성공, 실패
   - comment_id(int): 성공시 커멘트 ID 반환
   - reason(string): 실패시 이유


# 소셜
## 다른 사용자 조회
1. Endpoint
   - GET/users
2. Request body
   - user_id(int)
   - nickname(string)
3. Description
   - user_id 또는 nickname으로 조회한다
4. Response body
   - status(string) : 성공,실패
   - user_id
   - nickname
   - email
   - age(int)
## 팔로우 신청
## 팔로우한 목록 조회
## 자신에게 팔로우 요청한 목록 조회
## 팔로우를 수락/거절

# 메시지
## DM 보내기
## DM 조회하기
## DM 삭제하기
