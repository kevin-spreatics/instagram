# Place
### GET /place &nbsp; ✅
- GET place information by location
#### Request
- body
  - location - (geo_point) GPS 좌표, [경도, 위도]
```json
{
    "location": [126.4080501, 33.2524795]
}
```
```
GET /place?location=126.4080501,33.2524795
```
#### Response
- district_name - (List\<string\>) 지역명 (3 depth) 목록
- places - (List\<Place Object\>) (TBD)
```json
{
    "district_name": [
        "제주특별자치도",
        "서귀포시",
        "색달동"
    ],
    "places": []
}
```

### GET /place/{place_id} &nbsp; ✅
- Read place by place_id
#### Request
- path parameter
  - place_id - (string) 플레이스 id
#### Response
- Place [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#place) 

### POST /place/{place_id}/review  &nbsp; ✅
- 장소에 리뷰 달기
#### Request
- path parameter
  - place_id - (string) place id
- body
  - user - (string) user_id
  - review_text - (string) 리뷰 내용
#### Response
- result - (string) “created” | “failed”
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지

### DELETE /place/{place_id}/review  &nbsp; ✅
- 장소 리뷰 지우기
#### Request
- path parameter
  - place_id - (string) place id
- body
  - review_id - (integer) review_id
#### Response
- result - (string) “deleted” | “failed”
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지

### GET /place/{place_id}/review  &nbsp; ✅
- 장소에 달린 리뷰 목록 읽기
#### Request
- path parameter
  - place_id - (string) place id
```javascript
GET /place/A/review
```
#### Response
- List<Review [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#Review)>
```javascript
[
  {
    "type": "place",
    "target_id": "A",
    "review_id": "1",
    "user_id": "B",
    "review_time": "2019-01-12T06:22:58",
    "review_text": "C"
  },
  {
    "type": "place",
    "target_id": "A",
    "review_id": "D,
    "user_id": "E",
    "review_time": "2019-01-12T06:23:03",
    "review_text": "F"
  }
]
