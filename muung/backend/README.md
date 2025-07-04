# Muung 인스타그램 클론 REST API Spec
version 0.1 (2025/7/4)

## 서버 정보
- 호스트: 0.0.0.0
- 포트: 5000
- DB: AWS RDS MySQL (insta_doubled)

## 공통 사항
- 모든 응답은 JSON 형식
- 오류 발생 시 { "error": "오류 메시지" } 형태로 반환

---

## 1. 헬로월드

### 서버 동작 확인
**Endpoint**  
GET /hello

**Request body**  
없음

**Response body**  
"Hello, World!"

---

## 2. 회원가입/로그인

### 회원가입  
**Endpoint**  
POST /signup

**Request body**
- nickname (string, 필수)
- name (string, 필수)
- email (string, 필수)
- age (int, 필수)
- password (string, 필수)
- confirm_password (string, 필수)

```json
{
  "nickname": "muung",
  "name": "유현민",
  "email": "tgbt010@naver.com",
  "age": 25,
  "password": "12345678",
  "confirm_password": "12345678"
}
```

**Response body**
- message (string): 회원가입 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "회원가입이 완료되었습니다."
}
```
```json
{
  "error": "비밀번호가 일치하지 않습니다."
}
```

---

### 로그인  
**Endpoint**  
POST /login

**Request body**
- email (string, 필수)
- password (string, 필수)

```json
{
  "email": "tgbt010@naver.com",
  "password": "12345678"
}
```

**Response body**
- message (string): 로그인 성공 메시지
- user (object): 로그인 성공 시 사용자 정보
- error (string): 실패 시 오류 메시지

```json
{
  "message": "로그인 성공",
  "user": {
    "id": 1,
    "nickname": "muung",
    "name": "유현민",
    "email": "tgbt010@naver.com",
    "age": 25,
    "password": "12345678"
  }
}
```
```json
{
  "error": "이메일 또는 비밀번호가 일치하지 않습니다."
}
```

---

## 3. 사용자 관리

### 전체 사용자 목록 조회  
**Endpoint**  
GET /users

**Request body**  
없음

**Response body**
- users (array): 사용자 목록
- error (string): 실패 시 오류 메시지

```json
{
  "users": [
    {
      "id": 1,
      "nickname": "muung",
      "name": "유현민",
      "email": "tgbt010@naver.com",
      "age": 25,
      "password": "12345678"
    }
  ]
}
```

---

### 사용자 정보 수정  
**Endpoint**  
PUT /users/<user_id>

**Request body**
- nickname (string, 필수)
- name (string, 필수)
- email (string, 필수)
- age (int, 필수)
- password (string, 필수)
- confirm_password (string, 필수)

```json
{
  "nickname": "muung2",
  "name": "유현민",
  "email": "tgbt010@naver.com",
  "age": 26,
  "password": "12345678",
  "confirm_password": "12345678"
}
```

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "사용자 정보가 수정되었습니다."
}
```
```json
{
  "error": "비밀번호가 일치하지 않습니다."
}
```

---

### 사용자 삭제  
**Endpoint**  
DELETE /users/<user_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "사용자 정보가 삭제되었습니다."
}
```
```json
{
  "error": "존재하지 않는 사용자입니다."
}
```

---

## 4. 포스트 관리

### 포스트 업로드  
**Endpoint**  
POST /posts

**Request body**
- user_id (int, 필수)
- title (string, 필수)
- content (string, 필수)

```json
{
  "user_id": 1,
  "title": "첫 포스트",
  "content": "안녕하세요!"
}
```

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "포스트가 업로드되었습니다."
}
```

---

### 전체 포스트 목록 조회  
**Endpoint**  
GET /posts

**Request body**  
없음

**Response body**
- posts (array): 포스트 목록
- error (string): 실패 시 오류 메시지

```json
{
  "posts": [
    {
      "id": 1,
      "title": "첫 포스트",
      "content": "안녕하세요!",
      "user_id": 1
    }
  ]
}
```

---

### 포스트 수정  
**Endpoint**  
PUT /posts/<post_id>

**Request body**
- title (string, 필수)
- content (string, 필수)
- user_id (int, 필수)

```json
{
  "title": "수정된 포스트",
  "content": "내용 수정",
  "user_id": 1
}
```

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "포스트가 수정되었습니다."
}
```

---

### 포스트 삭제  
**Endpoint**  
DELETE /posts/<post_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "포스트가 삭제되었습니다."
}
```

---

## 5. 댓글 관리

### 댓글 작성  
**Endpoint**  
POST /posts/<post_id>/comments

**Request body**
- content (string, 필수)
- user_id (int, 필수)

```json
{
  "content": "좋은 글이네요!",
  "user_id": 2
}
```

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "코멘트가 작성되었습니다."
}
```

---

### 댓글 목록 조회  
**Endpoint**  
GET /posts/<post_id>/comments

**Request body**  
없음

**Response body**
- comments (array): 댓글 목록
- error (string): 실패 시 오류 메시지

```json
{
  "comments": [
    {
      "id": 1,
      "content": "좋은 글이네요!",
      "user_id": 2,
      "post_id": 1
    }
  ]
}
```

---

## 6. 팔로우/언팔로우

### 팔로우 하기  
**Endpoint**  
POST /follows/<follow_id>/<follower_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "팔로우가 완료되었습니다."
}
```

---

### 팔로우 취소  
**Endpoint**  
DELETE /follows/<follow_id>/<follower_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "팔로우가 취소되었습니다."
}
```

---

## 7. 팔로우 요청

### 팔로우 요청 보내기  
**Endpoint**  
POST /follow/requests/<follower_id>/<followee_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "팔로우 요청이 보내졌습니다."
}
```

---

### 팔로우 요청 수락  
**Endpoint**  
PUT /follow/requests/<follower_id>/<followee_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "팔로우 요청이 수락되었습니다."
}
```

---

### 팔로우 요청 거절  
**Endpoint**  
DELETE /follow/requests/<follower_id>/<followee_id>

**Request body**  
없음

**Response body**
- message (string): 성공 메시지
- error (string): 실패 시 오류 메시지

```json
{
  "message": "팔로우 요청이 거절되었습니다."
}
```

---

### 팔로우 요청 목록 조회  
**Endpoint**  
GET /follow/requests/<followee_id>

**Request body**  
없음

**Response body**
- follows (array): 팔로우 요청 목록
- error (string): 실패 시 오류 메시지

```json
{
  "follows": [
    {
      "id": 1,
      "follower_id": 2,
      "followee_id": 1,
      "status": "pending"
    }
  ]
}
```

---

## 8. 팔로잉/팔로워 목록

### 내가 팔로우하는 사람 목록  
**Endpoint**  
GET /follow/following/<follower_id>

**Request body**  
없음

**Response body**
- follows (array): 팔로잉 목록
- error (string): 실패 시 오류 메시지

```json
{
  "follows": [
    {
      "id": 1,
      "follower_id": 1,
      "follow_id": 2
    }
  ]
}
```

---

### 나를 팔로우하는 사람 목록  
**Endpoint**  
GET /follow/followers/<followee_id>

**Request body**  
없음

**Response body**
- follows (array): 팔로워 목록
- error (string): 실패 시 오류 메시지

```json
{
  "follows": [
    {
      "id": 1,
      "follower_id": 2,
      "followee_id": 1
    }
  ]
}
```

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

