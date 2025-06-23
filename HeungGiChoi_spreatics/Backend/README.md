# REST API Spec
 - version 0.1(2025/06/11)
# 사용자 계정
## 사용자 생성
1. Endpoint
   - POST /create_user
2. Request body 
   - nickname (string): 사용자 nickname, 필수
   - password (string): 비밀번호, 필수
   - name (string): 사용자 이름, 필수
   - age (int, optional): 사용자 나이
   - email (string, optional): 사용자 email 주소
~~~
{
  "nickname": "Choi",
  "name": "최흥기",
  "password": "1234",
  "age": 32,
  "email": "pamo23@naver.com"
}
~~~
3. Description
   - 사용자 계정을 생성한다. nickname과 name, password는 필수 입력값이다.
   - nickname은 고유한 값이며 기존 사용자와 중복되면 생성이 실패한다.
4. Response body
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
  "reason": f'{str(e)}, The nickname is duplicated'
}
~~~
## 사용자 인증 (로그인)
1. Endpoint
   - POST / Auth_users
2. Request body 
   - nickname (string): 사용자 nickname, 필수
   - password (string): 비밀번호, 필수
~~~
{
  "nickname": "Choi",
  "password": "1234",
}
~~~
4. Description
   - 사용자가 로그인한다. nickname과 password를 필수로 입력해야한다.
   - nickname과 password가 일치하면 로그인에 성공한다.
   - nickname과 password가 일치하지 않으면 로그인에 실패한다.
6. Response body
   - status (string): log_in Success, log_in failed
   - user_id (int): 로그인 성공시, user_id 반환
   - reason (string): 실패 시, 실패 원인
~~~
{
  "status": "log_in Success",
  "user_id": "105"
}

{
  "status": "log_in failed",
  "reason": "nickname, password unmatched"
}
~~~
## 사용자 정보 조회
1. Endpoint
   - GET / users / <user_id>
    - user_id: 조회할 사용사 user_id
2. Request body
   - 없음
4. Description
   - user_id에 해당하는 사용자 계정 정보를 조회한다.
5. Response body
   - status (string): selected
   - user_id (int): 조회 성공시, user_id 반환
   - nickname (string): 조회 성공시, nickname 반환
   - name (string): 조회 성공시, name 반환
   - email (string): 조회 성공시, email 반환
   - age (int): 조회 성공시, age 반환
   - reason (string): 실패시, 실패 원인
~~~
{
  "status": "selected",
  "user_id": 105
  "nickname": "Choi",
  "name": "최흥기",
  "email": "pamo23@naver.com"
  "age": 32,
}
~~~
## 사용자 정보 수정
1. Endpoint
   - PUT / user_update / <user_id>
    - user_id : 정보를 수정할 사용자 id
2. Request body
   - auth_nickname (string): 초기 인증 사용자 nickname, 필수
   - auth_password (string): 초기 인증 사용자 password, 필수
   - chg_nickname (string): 변경 사용자 nickname
   - chg_password (string): 비밀번호
   - chg_email (string, optional): 사용자 email 주소
4. Description
   - 로그인 시 입력한 nickname/password와 auth_nickname/auth_password가 일치하는지 비교한다.
   - 일치하면 해당하는 사용자 계정정보를 수정한다.
   - 일치하지 않으면 계정정보를 수정할 수 없다.
6. Response body
   - status (string): updated, failed
   - chg_nickname (string): 수정 성공시, chg_nickname 반환
   - chg_password (string): 수정 성공 시, chg_password 반환
   - chg_email (string): 수정 성공시, chg_email 반환
   - reason (string): 실패시, 실패 원인
~~~
{
  "status": "updated",
  "chg_nickname": "Jun",
  "chg_password": "54321",
  "chg_email": "pamo12@nate.com",
}
{
  "status": "failed",
  "reason": "auth_nickname, auth_password was unmatched"
}
~~~
## 사용자 삭제
1. Endpoint
   - DELETE /users/<user_id>
     - user_id (int): 삭제할 사용자 id
2. Request body 
   - auth_nickname (string): 삭제 인증 nickname, 필수
   - auth_password (string): 삭제 인증 password, 필수
4. Description
   - user_id에 해당하는 사용자 계정을 삭제한다.
   - auth_nickname/auth_password를 통해 계정삭제 인증 과정을 한번 더 거친다.
   - auth_nickname/auth_password가 로그인 시 입력한 nickname/password 과 일치하지 않으면 삭제가 실패한다.
5. Response body
   - status (string): deleted, failed
   - result (string): 삭제 성공시, 삭제 성공을 알린다.
   - reason (string): 실패시, 실패 원인
~~~
{
  "status": "deleted",
  "result": "user account was deleted"
}
{
  "status": "failed",
  "reason": "auth_nickname, auth_password was unmatched"
}
~~~
# 포스팅
## 포스트 올리기
1. Endpoint
   - POST/users/<user_id>/posts
    - user_id (int): 포스팅할 사용자 user_id
2. Request body
   - title (string): 게시물 제목, 필수
   - text (string): 게시물 내용, 필수
~~~
{
  "title": "원테이크 5기 정기 공연",
  "text": "원테이크 5기 정기반!일하고 자는 시간 외에는 공연 연습에 매진하고 있습니다. 6.14-15 양일 공연 기대해주세요~~",
}
~~~
3. Description
   - user_id를 가진 계정으로 포스팅을 진행한다.
   - 제목과 내용이 없으면 포스팅을 할 수 없다. 
5. Response body
   - status (string): post_success, failed
   - user_id (int): 생성 성공 시, user_id 반환
   - title (string): 생성 성공 시, title 반환
   - text (string): 생성 성공 시, text 반환
   - reason (string): 실패 시, 실패 원인
~~~
{
  "status": "post_success",
  "user_id": 105
  "title": "원테이크 5기 정기 공연",
  "text": "원테이크 5기 정기반!일하고 자는 시간 외에는 공연 연습에 매진하고 있습니다. 6.14-15 양일 공연 기대해주세요~~",
}

{
  "status": "failed",
  "reason": "Please enter the title and content."
}
~~~
## 올라온 포스트 조회하기
1. Endpoint
   - GET/users/<user_id>/posts/<post_id>
    - user_id : 로그인한 user_id
    - post_id : 포스팅된 컨텐츠 id
2. Request body 
   - 없음
3. Description
   - 업로드된 post를 조회한다.
4. Response body
   - status (string): post_selected
   - user_id (int): 조회한 post를 작성한 user_id
   - post_id (int): 조회한 post의 id
   - title (string): 조회한 post의 제목
   - text (string): 조회한 post의 내용
   - create_at (string): 조회한 post의 생성 일자
~~~
{
  "status": "post_selected",
  "user_id": "3",
  "post_id": "1",
  "title": "원테이크 5기 정기 공연",
  "text": "원테이크 5기 정기반!일하고 자는 시간 외에는 공연 연습에 매진하고 있습니다. 6.14-15 양일 공연 기대해주세요~~"
}
~~~
## 포스트의 커맨트 조회하기
1. Endpoint
   - GET/users/<user_id>/posts/<post_id>/comments/<comment_id>
      - user_id : 로그인한 사용자 id
      - post_id : 포스팅된 컨텐츠 id
      - comment_id : 컨텐츠에 달린 커맨트 id
2. Request body
   - 없음
3. Description
   - 포스팅한 컨텐츠의 커맨트를 조회한다.
4. Response body
   - status (string): selected
   - user_id (int): 조회 시, 커맨트를 단 사용자 id
   - post_id (int): 조회 시, 커맨트 달린 post id
   - comment_id (int): 조회 시, 커맨트 id
   - text (string): 조회 시, 커맨트 내용
   - created_at (string): 조회 시, 커맨트 생성 일자
~~~
{
   "status": "selected",
   "user_id": 3,
   "post_id": 2,
   "title": "원테이크 5기 정기 공연",
   "comment_id": 1,
   "text": "꼭 보러 가겠습니다!",
   "create_at": "2025-06-19"
}
~~~
## 특정 포스트에 커맨트 달기
1. Endpoint
   - POST/users/<user_id>/posts/<post_id>/comments
      - user_id : 로그인한 사용자 id
      - post_id : 포스팅된 컨텐츠 id
2. Request body
   - text (string): 커맨트 내용, 필수
4. Description
   - 포스팅한 컨텐츠의 커맨트를 단다.
   - 커맨트 내용이 없으면 커맨트를 달 수 없다.
5. Response body
   - status (string): created, failed
   - user_id (int): 커맨트 달기 성공 시, 커맨트를 단 사용자 id
   - comment_id (int): 커맨트 달기 성공 시, 커맨트 id
   - text (string): 커맨트 달기 성공 시, 커맨트 내용
   - create_at (string): 커맨트 달기 성공 시, 커맨트 생성 일자
   - reason (string): 실패 시, 실패 원인
~~~
{
   "status": "created",
   "user_id": 3
}

{
   "status": "failed",
   "reason": "Please enter comment content."
}
~~~
# 소셜
## 다른 사용자 조회
## 팔로우 신청
## 팔로우한 목록을 조회
## 자신에게 팔로우 요청한 목록을 조회
## 팔로우를 수락/거절

# 메세지
## DM을 보내기
## DM 조회하기
## DM 삭제하기
