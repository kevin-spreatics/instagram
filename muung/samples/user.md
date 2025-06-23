# User
### POST /user &nbsp; ✅
- user creation
#### Request
- body
  - user_id - (string, required) user id, 대문자는 lowercase 하여 생성
  - nickname - (string, required)	사용자 닉네임
  - email - (string) email 주소	
  - nationality - (string) 국적	
  - birth_date - (string) 생년월일 (format: '%Y-%m-%d')
  - phone_number - (string) 전화번호	
```json
{
    "user_id": "grisomm",
    "nickname": "Grisomm",
    "birth_date": "1976-11-27"
}
```
- attache image file for photo (TBD)
  - 현재는 임시로 photo_url 생성
#### Response
- result - (string) “created” | “failed”
- user - (User [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#user))
- error (실패시)
  - code - (integer) 에러 코드
  - message - (string) 에러 메시지 
```json
{
    "result": "created",
    "user": {
        "user_id": "grisomm",
        "nickname": "Grisomm",
        "photo_url": "https://as1.ftcdn.net/v2/jpg/00/81/25/34/1000_F_81253416_eFuFp5UKC9thPguL0NytG3YiHVTdDk2i.jpg",
        "reward_point": 0,
        "email": null,
        "nationality": null,
        "birth_date": "1976-11-27",
        "phone_number": null,
        "create_time": "2022-04-29"
    }
}
```

### GET /user/{user_id} &nbsp; ✅
- get user by user_id
#### Request
- path parameter
  - user_id - (string) user id
#### Response
- User [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#user) 

### DELETE /user/{user_id} &nbsp; ✅
- delete user by user_id
#### Request
- path parameter
  - user_id - (string) user id
#### Response
- result - (string) “deleted” | “failed”
- error (실패시)
  - code - (integer) 에러 코드
  - message - (string) 에러 메시지

### POST /user/{user_id}/checkin &nbsp; ✅
- Submit user's checking with location
#### Request
- path parameter
  - user_id - (string) user id
- body
  - location - (geo_point) checkin할 GPS 좌표, [경도, 위도]
```json
{
    "location": [126.4080501, 33.2524795]
}
```
#### Response
- result - (string) “checkedin” | “failed”
- checkin - (Checkin [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#checkin))
- error (실패시)
  - code - (integer) 에러 코드
  - message - (string) 에러 메시지 
```json
{
    "result": "checkedin",
    "checkin": {
        "user_id": "beyondex",
        "location": [
            126.4080501,
            33.2524795
        ],
        "district_name": [
            "제주특별자치도",
            "서귀포시",
            "색달동"
        ],
        "checkin_time": "2022-04-13T08:14:21"
    }
}
```

### GET /user/{user_id}/checkin &nbsp; ✅
- Read user's checkin history
#### Request
- path parameter
  - user_id - (string) user id
#### Response
- List<Checkin [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#checkin)>
```json
[
    {
        "user_id": "beyondex",
        "location": [
            127.3125,
            37.5132
        ],
        "district_name": [
            "광주시",
            "남종면",
            "귀여리"
        ],
        "checkin_time": "2022-04-13T08:38:13"
    },
    {
        "user_id": "beyondex",
        "location": [
            127.1125,
            37.5132
        ],
        "district_name": [
            "서울특별시",
            "송파구",
            "방이동"
        ],
        "checkin_time": "2022-04-13T08:38:08"
    }
]
```

### DELETE /user/{user_id}/checkin &nbsp; ✅
- checkin 기록 삭제
#### Request
- path parameter
  - user_id - (string) user id
#### Response
- result - (string) “deleted” | “failed”
- error (실패시)
  - code - (integer) 에러 코드
  - message - (string) 에러 메시지


### POST /user/{user_id}/save  &nbsp; ✅
- 좋아요 추가
#### Request
- path parameter
  - user_id - (string) user id
- body
  - type - (string) "place" | "video"
  - target_id - (string) place_id | video_id
#### Response
- result - (string) “saved” | “failed”
- error (실패시)
  - code - (integer) 에러 코드
  - message - (string) 에러 메시지 



### DELETE /user/{user_id}/save &nbsp; ✅
- 좋아요 취소
#### Request
- path parameter
  - user_id - (string) user id
- body
  - type - (string) "place" | "video"
  - target_id - (string) place_id | video_id
#### Response
- result - (string) “canceled” | “failed”
- error (실패시)
  - code - (integer) 에러 코드
  - message - (string) 에러 메시지


### GET /user/{user_id}/save &nbsp; ✅
- 좋아요 목록 읽기
#### Request
- path parameter
  - user_id - (string) user id
- body
  - type - (List<string>) "place" | "video", default both
```javascript
{
  "type": ["video"]
}
```
```javascript
GET /user/beyondEx/save?type=video
```  
#### Response
- List<Save [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#save)>
  - video - (Video Object)
```javascript
[
  {
    "type": "video",
    "target_id": "A",
    "user_id": "beyondEx",
    "save_time": "2019-01-12T06:22:58",
    "video": { ... }
  },
  ...
]
