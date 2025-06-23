# 📷 Instagram API 문서

Instagram 기능을 모방한 RESTful API. 사용자 계정 관리, 게시물 업로드, 댓글, 팔로우, DM 기능 등을 제공.

---

## 🍔 API 개요 - 음식점 비유

- API는 **사용자(당신)** 와 **서비스(주방)** 사이를 연결해주는 **웨이터** 역할.
- 사용자가 요청을 보내면, API는 그것을 서버로 전달하고 결과를 받아 다시 사용자에게 응답.

---

## 📌 API 목록

- 👤 사용자 (User)
- 🖼 포스팅 (Post)
- 🤝 팔로우 (Follow)
- 💬 DM (Direct Message)

---

## 👤 사용자 (User)

### ▶ 사용자 생성

**Endpoint**  
`POST /users`

**Request Body**
```json
{
  "nickname": "kevin",
  "name": "이승학",
  "password": "1234",
  "email": "kevin.spreatics@gmail.com"
}
```

| 필드 | 타입 | 필수 | 설명 |
|------|------|------|------|
| nickname | string | ✔️ | 사용자 닉네임 (고유해야 함) |
| name | string | ✔️ | 사용자 이름 |
| password | string | ✔️ | 사용자 비밀번호 |
| age | int | ✖️ | 사용자 나이 |
| email | string | ✖️ | 사용자 이메일 주소 |

**Response Body**
```json
// 성공
{
  "status": "created",
  "user_id": 105
}

// 실패
{
  "status": "failed",
  "reason": "nickname, kevin is duplicated"
}
```

---

### ▶ 사용자 로그인

**Endpoint**  
`POST /auth/login`

**Request Body**
```json
{
  "nickname": "kevin",
  "password": "1234"
}
```

**Response Body**
```json
// 성공
{
  "status": "authenticated",
  "token": "eyJhbGciOiJIUzI1NiIsInR..."
}

// 실패
{
  "status": "failed",
  "reason": "Invalid credentials"
}
```

---

### ▶ 사용자 정보 조회

**Endpoint**  
`GET /users/{user_id}`

**Response Body**
```json
{
  "user_id": 105,
  "nickname": "kevin",
  "name": "이승학",
  "email": "kevin.spreatics@gmail.com",
  "age": 25
}
```

---

### ▶ 사용자 정보 수정

**Endpoint**  
`PUT /users/{user_id}`

**Request Body**
```json
{
  "email": "newemail@example.com",
  "age": 26
}
```

**Response Body**
```json
{
  "status": "updated"
}
```

---

### ▶ 사용자 삭제

**Endpoint**  
`DELETE /users/{user_id}`

**Response Body**
```json
{
  "status": "deleted"
}
```

---

## 🖼 포스팅 (Post)

### ▶ 포스트 올리기

**Endpoint**  
`POST /posts`

**Request Body**
```json
{
  "user_id": 105,
  "image_url": "https://cdn.example.com/image1.jpg",
  "caption": "오늘 날씨 완전 최고!"
}
```

**Response Body**
```json
// 성공
{
  "status": "created",
  "post_id": 3001
}

// 실패
{
  "status": "failed",
  "reason": "Missing image_url"
}
```

---

### ▶ 포스트 조회

**Endpoint**  
`GET /posts?user_id={user_id}`

**Response Body**
```json
[
  {
    "post_id": 3001,
    "image_url": "https://cdn.example.com/image1.jpg",
    "caption": "오늘 날씨 완전 최고!",
    "created_at": "2025-06-13T15:20:00"
  },
  {
    "post_id": 3002,
    "image_url": "https://cdn.example.com/image2.jpg",
    "caption": "점심은 김치찌개",
    "created_at": "2025-06-13T18:10:00"
  }
]
```

---

### ▶ 댓글 조회

**Endpoint**  
`GET /posts/{post_id}/comments`

**Response Body**
```json
[
  {
    "comment_id": 501,
    "user_id": 108,
    "nickname": "jane",
    "comment": "와 진짜 예뻐요!",
    "created_at": "2025-06-13T16:00:00"
  }
]
```

---

### ▶ 댓글 작성

**Endpoint**  
`POST /posts/{post_id}/comment`

**Request Body**
```json
{
  "user_id": 110,
  "comment": "와 여기 분위기 좋아보여요!"
}
```

**Response Body**
```json
// 성공
{
  "status": "created",
  "comment_id": 503
}

// 실패
{
  "status": "failed",
  "reason": "Comment cannot be empty"
}
```

---

## 🤝 팔로우 (Follow)

### ▶ 사용자 검색

**Endpoint**  
`GET /users?keyword={keyword}`

**Response Body**
```json
[
  {
    "user_id": 110,
    "nickname": "minsu",
    "profile_image": "https://cdn.example.com/profiles/minsu.jpg"
  }
]
```

---

### ▶ 팔로우 요청

**Endpoint**  
`POST /follow/request`

**Request Body**
```json
{
  "from_user_id": 105,
  "to_user_id": 110
}
```

**Response Body**
```json
{
  "status": "requested"
}
```

---

### ▶ 팔로우 수락 / 거절

**Endpoint**  
`POST /follow/respond`

**Request Body**
```json
{
  "request_id": 2001,
  "action": "accept"
}
```

**Response Body**
```json
{
  "status": "accepted"
}
```

---

### ▶ 팔로잉 목록 조회

**Endpoint**  
`GET /followings?user_id={user_id}`

**Response Body**
```json
[
  {
    "user_id": 110,
    "nickname": "minsu",
    "profile_image": "https://cdn.example.com/profiles/minsu.jpg"
  }
]
```

---

### ▶ 팔로우 요청 목록 조회

**Endpoint**  
`GET /follow/requests?user_id={user_id}`

**Response Body**
```json
[
  {
    "request_id": 2001,
    "from_user_id": 108,
    "nickname": "jane",
    "profile_image": "https://cdn.example.com/profiles/jane.jpg"
  }
]
```

---

## 💬 DM (Direct Message)

### ▶ DM 보내기

**Endpoint**  
`POST /dm`

**Request Body**
```json
{
  "from_user_id": 105,
  "to_user_id": 110,
  "message": "안녕하세요! 반가워요"
}
```

**Response Body**
```json
{
  "status": "sent",
  "dm_id": 9001
}
```

---

### ▶ DM 조회

**Endpoint**  
`GET /dm/conversation?user1_id={id1}&user2_id={id2}`

**Response Body**
```json
[
  {
    "dm_id": 9001,
    "from_user_id": 105,
    "message": "안녕하세요! 반가워요",
    "sent_at": "2025-06-13T16:30:00"
  }
]
```

---

### ▶ DM 삭제

**Endpoint**  
`DELETE /dm/{dm_id}`

**Response Body**
```json
{
  "status": "deleted"
}
```

---
