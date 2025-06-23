# API Reference v1
## Version History
### version 0.6.0 (5/01)
- user API 추가
### version 0.5.0 (4/13)
- place API update (지명, 장소명)
- user checkin API 추가
- search filter, sort list API 추가
### version 0.4.1 (4/06)
- video object update
### version 0.4.0 (3/31)
- coupon, campaign, coupon box, coupon templete, membership object 
- POST /video API update
### Version 0.3.3 (3/19)
- Review, Save objects, API 추가
### Version 0.3.2 (3/17)
- Place API 추가
### Version 0.3.1 (3/16)
- Video object 수정 - place, creator 필드 데이터 타입 변경
- GET /search request 수정 - page 관련 필드 수정
- GET /search response 수정 - place, creator, video 외부로 이동
- GET /video 수정 - 검색 결과 선택 시 동영상 목록 얻어 올 수 있도록 수정  
### Version 0.3.0 (3/15)
- API Objects (Video, Place, User) 작성
- Search API - /search, /autocomplete, /popular_keyword 작성
- Video API - /video 작성

## Endpoints
Endpoints start with http://3.36.33.69:8000/api/v1

API                    |Endpoint                          |Description   
:---                   |:---                              |:---          
[Video](video.md)      |POST /video                       |동영상 생성      
|                      |GET /video/{video_id}             |id로 동영상 정보 가져오기              
|                      |GET /video                        |조건으로 동영상 정보 목록 가져오기
|                      |PUT /video/{video_id}             |동영상 업데이트   
|                      |DELETE /video/{video_id}          |동영상 삭제
|                      |POST /video/{video_id}/review     |비디오에 리뷰 달기
|                      |DELETE /video/{video_id}/review   |비디오 리뷰 지우기
|                      |GET /video/{video_id}/review      |비디오에 달린 리뷰 읽기
[Search](search.md)    |GET /search                       |동영상 검색      
|                      |GET /autocomplete                 |키워드 자동완성      
|                      |GET /popular_keyword              |인기 검색어  
[Place](place.md)      |GET /place                        |지명, 장소명 가져오기
|                      |GET /place/{place_id}             |장소 정보 가져오기
|                      |POST /place/{place_id}/review     |장소 리뷰 달기
|                      |DELETE /place/{place_id}/review   |장소 리뷰 지우기
|                      |GET /place/{place_id}/review      |장소에 달린 리뷰 읽기
[User](user.md)        |PUT /user/{user_id}               |사용자 정보 업데이트
|                      |GET /user/{user_id}               |사용자 정보 읽기
|                      |POST /user/{user_id}/checkin      |checkin
|                      |GET /user/{user_id}/checkin       |checkin 기록 읽기
|                      |DELETE /user/{user_id}/checkin    |checkin 기록 삭제
|                      |POST /user/{user_id}/save         |좋아요 추가
|                      |DELETE /user/{user_id}/save       |좋아요 취소
|                      |GET /user/{user_id}/save          |좋아요 목록 읽기

## API Objects
### Video
Field               |Type           |Description
:---                |:---           |:---
video_id            |string         |id
video_type          |string         |비디오 타입 ("shorts" \| "story" \| "guide" )
title               |string         |제목
description         |string         |설명
location            |geo_point      |촬영 위치 (GPS 좌표)
district_location   |geo_shape      |촬영 장소가 속한 지역 (GPS 좌표)
district_name       |List\<string\> |촬영 장소가 속한 지역명 ex) "중문"
thumbnail_url       |string         |thumbnail url
video_url           |string         |video url
categories          |List\<string\> |카테고리 목록
tags                |List\<string\> |크리에이터 입력 tag 목록
generated_tags      |List\<string\> |시스템 생성 tag 목록
is_portrait         |bool           |세로 모드
resolution          |string         |해상도
width               |integer        |가로 픽셀
height              |integer        |세로 픽셀
upload_time         |string         |업로드 날짜
_upload_time        |date           |업로드 날짜, 시간 (ISO datetime format)
start_time          |float          |재생시작시간
duration            |float          |재생시간
saves               |long           |좋아요수
reviews             |long           |리뷰수
views               |long           |조회수
place               |string         |place_id
creator             |string         |user_id
  
### Place
Field               |Type           |Description
:---                |:---           |:---
place_id            |string         |id
name                |string         |이름
description         |string         |설명
categories          |List\<string\> |카테고리 목록 ex) ["숙소", "팬션"]
location            |geo_point      |장소의 위치 (GPS 좌표)
district_location   |geo_shape      |장소가 속한 지역 (GPS 좌표)
district_name       |string         |장소가 속한 지역명 ex) "중문"
featured_video      |List\<Video\>  |대표 동영상
facility            |string         |편의 시설 설명
tags                |List\<string\> |tag 목록
price               |float_range    |가격 범위
currency            |string         |통화
level               |integer        |등급 (for hotel)
review_rate         |float          |평점
saves               |long           |좋아요수
reviews             |long           |리뷰수
views               |long           |조회수
nearby              |List\<string\> |주변 장소              
place_url           |string         |장소 url

### User
Field               |Type           |Description            |Required
:---                |:---           |:---                   |:---
user_id             |string         |id                     |O
nickname            |string         |사용자 닉네임              |O
photo_url           |string         |사진 url                |
reward_point        |integer        |포인트                   |
email               |string         |email 주소              |
nationality         |string         |국적                    |
birth_date          |string         |생년월일('%Y-%m-%d')     |
phone_number        |string         |전화번호                  |
create_data         |string         |생성일('%Y-%m-%d')       |
oauth               |TBD            |인증정보                  |
passwd              |TBD            |암호화된 사용자 passwd     |
payment_info        |TBD            |결제정보                  |
trip_preference     |TBD            |여행취향                  |
checkins            |TBD            |방문지역                  |
upload_videos       |TBD            |업로드 동영상              |
saves               |TBD            |좋아요 동영상, 좋아요 장소    |
planes              |TBD            |일정                     |
bookings            |TBD            |예약                     |

### Membership
Field               |Type           |Description
:---                |:---           |:---
user_id             |string         |id
point               |integer        |포인트
point_time          |data           |포인트 변경 시점
type                |string         |"earn" \| "span"
activity_type       |string         |"review" \| "booking" \| "upload" \| "saved"
activity_id         |string         |activity id (ex. review_id, video_id, ...)

### Coupon
Field               |Type           |Description
:---                |:---           |:---
coupon_id           |string         |coupon id
issuer_id           |string         |발행인 id
templete_id         |string         |templete id
issue_time          |data           |발행일
title               |string         |쿠폰 제목
description         |string         |내용
discount            |integer        |할인
discount_type       |string         |"amount" \| "percent"
currency            |string         |통화
QR_code_url         |string         |QR code url
  
### CouponBox
Field               |Type           |Description
:---                |:---           |:---
user_id             |string         |id
coupon_id           |string         |coupon id
status              |string         |"valid" \| "done" \| "expired"

### CouponTemplete
Field               |Type           |Description
:---                |:---           |:---
templete_id         |string         |id
templete_code       |string         |templete html code

### Campaign
Field               |Type           |Description
:---                |:---           |:---
planner_id          |string         |id
duration            |date_range     |campaign range
status              |string         |"running" \| "stoped"

### Badge
Field               |Type           |Description
:---                |:---           |:---
name                |string         |badge 이름
image_url           |string         |badge 이미지 url
  
### Review
Field               |Type           |Description
:---                |:---           |:---
type                |string         |"place" \| "video"
target_id           |string         |place_id \| video_id
review_id           |integer        |리뷰 id
user_id             |string         |사용자
review_time         |strin          |리뷰 작성 날짜
_review_time        |date           |리뷰 날짜, 시간 (ISO datetime format)
review_text         |string         |리뷰 내용
image_urls          |List\<string\> |리뷰 이미지 목록 (TBD)
video_urls          |List\<string\> |리뷰 동영상 목록 (TBD)

### Save
Field               |Type           |Description
:---                |:---           |:---
type                |string         |"place" \| "video"
target_id           |string         |place_id \| video_id
user_id             |string         |사용자
save_time           |string         |좋아요한 날짜
_save_time          |date           |좋아요한 날짜, 시간 (ISO datetime format)

### Checkin
Field               |Type           |Description
:---                |:---           |:---
user_id             |string         |사용자
location            |geo_point      |checkin한 좌표
district_name       |List\<string\> |checkin한 위치의 지역명 ex) ["제주특별자치도", "서귀포시", "색달동"]
checkin_time        |string         |checkin한 날짜
_checkin_time       |date           |checkin한 날짜, 시간 (ISO datetime format)
  
### Filter
```json
{
  "hotel": {
    "price": ["$~$$", "$$~$$$", "$$$~$$$$"],
    "cancelation": ["free"],
    "level": [1, 2, 3, 4, 5],
    "type": ["hotel", "pansion", "motel", "guest house"],
    "facility": ["swimming pool", "bath hub", "spa", "fitness"],
    "bed": ["single", "double", "twin"],
    "meal": ["breakfast", "dinner"]
  },
  "restaurant": {
    "price": ["$~$$", "$$~$$$", "$$$~$$$$"],
    "type": ["restaurant", "dessert", "coffee & tea"],
    "Cuisine": ["italian", "asian" ]
  },
  "tour": {
    "tour": [],
    "ativity": []
  }
}
```
  
### Sort
```json
["review_rate", "reviews", "saves", "views", "nearby"]
```

### Plan
Field               |Type           |Description
:---                |:---           |:---

  
### Primitive data types
Type         |Description
:---         |:---
integer      |32-bit integer
long         |64-bit integer
float        |32-bit floating point number
double       |64-bit floating point number
string       |string
boolean      |boolean
date         |https://www.elastic.co/guide/en/elasticsearch/reference/current/date.html
geo_point    |https://www.elastic.co/guide/en/elasticsearch/reference/current/geo-point.html
geo_shape    |https://www.elastic.co/guide/en/elasticsearch/reference/current/geo-shape.html
float_range  |https://www.elastic.co/guide/en/elasticsearch/reference/current/range.html
date_range   |https://www.elastic.co/guide/en/elasticsearch/reference/current/range.html 
