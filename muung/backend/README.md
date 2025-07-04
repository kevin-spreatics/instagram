# Muung 인스타그램 클론 REST API 명세

## 서버 정보
- 호스트: 0.0.0.0
- 포트: 5000
- DB: AWS RDS MySQL (insta_doubled)

## 공통 사항
- 모든 응답은 JSON 형식
- 오류 발생 시 { "error": "오류 메시지" } 형태로 반환

---

## 1. 헬로월드
- **GET /hello**
  - 서버 동작 확인용
  - 응답: "Hello, World!"

---

## 2. 회원가입/로그인

### 회원가입
- **POST /signup**
  - 요청: JSON
    - nickname (string, 필수)
    - name (string, 필수)
    - email (string, 필수)
    - age (int, 필수)
    - password (string, 필수)
    - confirm_password (string, 필수)
  - 응답: 회원가입 성공/실패 메시지

### 로그인
- **POST /login**
  - 요청: JSON
    - email (string, 필수)
    - password (string, 필수)
  - 응답: 로그인 성공 시 사용자 정보 반환

---

## 3. 사용자 관리

- **GET /users**
  - 전체 사용자 목록 조회

- **PUT /users/<user_id>**
  - 사용자 정보 수정
  - 요청: nickname, name, email, age, password, confirm_password

- **DELETE /users/<user_id>**
  - 사용자 삭제

---

## 4. 포스트 관리

- **POST /posts**
  - 포스트 업로드 (user_id, title, content 필요)

- **GET /posts**
  - 전체 포스트 목록 조회

- **PUT /posts/<post_id>**
  - 포스트 수정 (title, content, user_id 필요)

- **DELETE /posts/<post_id>**
  - 포스트 삭제

---

## 5. 댓글 관리

- **POST /posts/<post_id>/comments**
  - 댓글 작성 (content, user_id 필요)

- **GET /posts/<post_id>/comments**
  - 해당 포스트의 댓글 목록 조회

---

## 6. 팔로우/언팔로우

- **POST /follows/<follow_id>/<follower_id>**
  - 팔로우 하기

- **DELETE /follows/<follow_id>/<follower_id>**
  - 팔로우 취소

---

## 7. 팔로우 요청

- **POST /follow/requests/<follower_id>/<followee_id>**
  - 팔로우 요청 보내기

- **PUT /follow/requests/<follower_id>/<followee_id>**
  - 팔로우 요청 수락

- **DELETE /follow/requests/<follower_id>/<followee_id>**
  - 팔로우 요청 거절

- **GET /follow/requests/<followee_id>**
  - 팔로우 요청 목록 조회

---

## 8. 팔로잉/팔로워 목록

- **GET /follow/following/<follower_id>**
  - 내가 팔로우하는 사람 목록

- **GET /follow/followers/<followee_id>**
  - 나를 팔로우하는 사람 목록

---

## 예시 요청/응답

### 회원가입 예시
```
POST /signup
{
  "nickname": "muung",
  "name": "유현민",
  "email": "tgbt010@naver.com",
  "age": 25,
  "password": "12345678",
  "confirm_password": "12345678"
}
```

### 로그인 예시
```
POST /login
{
  "email": "tgbt010@naver.com",
  "password": "12345678"
}
```

### 응답 예시
```
{
  "message": "로그인 성공",
  "user": {
    "id": 1,
    "nickname": "muung",
    ...
  }
}
```

---

### 기타 응답 예시

- 성공: `{ "message": "성공 메시지" }`
- 실패: `{ "error": "오류 메시지" }`

