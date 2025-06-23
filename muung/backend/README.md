### 사용자 생성 API

- **Method**: POST
- **Endpoint**: /users
- **Request Body**:
  - nickname(string): 사용자 닉네임(필수)
  - name(string): 사용자 이름(필수) 
  - password(string): 비밀번호(필수)
  - email(string, optional): 사용자 이메일 주소(선택)
  - age(int, optional): 사용자 나이(선택)
 
- 사용자 계정 생성. nickname은 고유해야 하며, 중복 시 생성 실패

- **요청 예시**:
~~~
{
  "nickname": "kevin",
  "name": "이승학",
  "password": "1234",
  "email": "kevin.spreatics@gmail.com"
}
~~~

- 성공 응답
~~~
{
  "status": "created",
  "user_id": 105
}
~~~

- 실패 응답
~~~
{
  "status": "failed",
  "reason": "nickname, kevin is duplicated"
}
~~~

### 사용자 삭제 API
- **Endpoint**: DELETE /users/<user_id>
- **Request body**: 없음
- **Desctiption**: 해당 user_id의 사용자 계정을 삭제함, 존재하지 않는 user_id일 경우 삭제 실패


### 포스팅 API
▶ 포스트 올리기
- Endpoint: POST /posts
- Request Body:
  - user_id(int): 작성자 id
  - content(string): 텍스트 내용
  - image_url(string): 이미지 주소(선택)
~~~
{
  "user_id": 1,
  "content": "오늘 점심은 라면!",
  "image_url": "http://example.com/image.jpg"
}
~~~
- 설명: 사용자가 텍스트와 이미지를 포함한 포스트를 업로드
- Response:
~~~
{
  "status": "created",
  "post_id": 101
}
~~~

▶ 올라온 포스트 전체 조회하기
- Endpoint: GET /posts
- Query Parameters (optional):
  - user_id: 특정 사용자 포스트만 조회
- 설명: 전체 포스트(또는 특정 사용자)의 리스트를 반환

▶ 포스트의 커맨트 조회
- Endpoint: GET /posts/<post_id>/comments
- 설명: 해당 포스트에 달린 댓글 목록을 반환

▶ 특정 포스트에 커맨트 달기
- Endpoint: POST /posts/<post_id>/comments
- Request Body:
~~~
{
  "user_id": 3,
  "comment": "맛있어 보여요!"
}
~~~
Response:
~~~
{
  "status": "created",
  "comment_id": 88
}
~~~


### 소셜
▶ 다른 사용자 조회
- Endpoint: GET /users/<user_id>
- 설명: 특정 사용자의 프로필을 조회

▶ 팔로우 신청
- Endpoint: POST /users/<follower_id>/follow/<followee_id>
- 설명: 팔로워가 상대방을 팔로우 신청
- Response:
~~~
{
  "status": "requested"
}
~~~

▶ 팔로우한 목록 조회
- Endpoint: GET /users/<user_id>/following
- 설명: 해당 사용자가 팔로우한 사용자 목록을 반환

▶ 나에게 팔로우 요청한 목록 조회
- Endpoint: GET /users/<user_id>/follow_requests
- 설명: 나에게 들어온 팔로우 요청 목록을 반환

▶ 팔로우 수락 / 거절
- Endpoint:
- 수락: POST /follow_requests/<request_id>/accept
- 거절: DELETE /follow_requests/<request_id>/reject
- 설명: 특정 팔로우 요청을 수락하거나 거절



### 메시지 (DM)
▶ DM 보내기
- Endpoint: POST /users/<user_id>/dms
- Request Body:
~~~
{
  "to": 2,
  "message": "안녕하세요!"
}
~~~
Response:
~~~
{
  "status": "sent",
  "dm_id": 999
}
~~~

▶ DM 조회하기
- Endpoint: GET /users/<user_id>/dms
- 설명: 해당 사용자의 DM 대화 목록을 반환

▶ DM 삭제하기
- Endpoint: DELETE /users/<user_id>/dms/<dm_id>
- 설명: 특정 DM 메시지를 삭제 
