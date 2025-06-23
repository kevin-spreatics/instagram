# Video
### POST /video_request &nbsp; ✅ 
- Request to upload video
#### Request
- body
  - creator - (string) 비디오 생성자의 user_id
  - location - (geo_point) 촬영 위치 (GPS 좌표)
```json
{
	"creator":"grisomm",
	"location":[126.4080501,33.4524795]
}
```
#### Response
- result - (string) “submited” | “failed”
- upload_url - 비디오 업로드할 s3 url, 수신후 video파일을 해당 url로 put
- video_id - (string) 발급된 video_id
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지
```json
{
    "result": "submitted",
    "video_id": "000994931920524",
    "upload_url": "https://beyondex.s3.amazonaws.com/Travel/user_video/000994931920524.mp4?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAR4Y4FUTKZD5ZKK4Z%2F20221119%2Fap-northeast-2%2Fs3%2Faws4_request&X-Amz-Date=20221119T065541Z&X-Amz-Expires=1000&X-Amz-SignedHeaders=host&X-Amz-Signature=8fcff562241522ad758aeaf4af6404588173514366bd83c87d37217d5510a663"
}
```
### GET /video_request &nbsp; ✅ 
- Get list of requested videos
- 업로드 요청한 비디오의 처리 상태 확인
- 가능한 state
  - upload, validate, analyze, accept, reject
- analyze가 끝나면 accept 상태가 되고, generated_tags에 인식된 tag가 포함
- timeline_tags에는 시간별 tags 포함
#### Request
- body
  - creator - (string) 비디오 생성자의 user_id
#### Response
- List\<video_request\>
```json
[
    {
        "user_id": "grisomm",
        "video_id": "000106033706875",
        "state": "analyze",
	"reason_if_reject": null, # reject 시, reject reasion 메시지
        "submit_time": "2022-11-20 04:01:01",
        "presign_url": "https://beyondex.s3.amazonaws.com/Travel/user_video/000658717566498.mp4?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAR4Y4FUTKZD5ZKK4Z%2F20221120%2Fap-northeast-2%2Fs3%2Faws4_request&X-Amz-Date=20221120T040102Z&X-Amz-Expires=1000&X-Amz-SignedHeaders=host&X-Amz-Signature=24231286b60a6351d18f633513a1b518ba425a68e013f5bcbbe9ba39c9ea9d53",
        "instance": "Apple M1",	# (string) analyze 중인 instance
        "process_type": "GPU",	# (string) instance의 process type
        "generated_tags": null, # (List\<string\>) 분석된 tags
        "timeline_tags": null,	# 분석된 tags의 timeline
	"suggested_poi": null,  # (List\<string\>) 제안하는 장소명
        "complete_time": null,	# 분석 완료 시간
        "elapsed_time": null,	# 분석 소요 시간
        "video": {}
    },
]
```
### POST /video_request/{video_id} &nbsp; ✅ 
- accept 된 비디오 인덱스 요청
#### Request
- body
  - title	- (string) 제목
  - description	- (string, optional) 설명
  - categories - (List\<string\>)	카테고리 목록
  - tags - (List\<string\>, optional) tag 목록
  - video_type - (string, optional) 비디오 타입 ("shorts" \| "story" \| "guide" )
  - place (string) - 장소명
```json
{
	"title": "칼튼 호텔",
	"description":"방콕 스쿰빗 칼튼 호텔",
	"categories": [ "hotel" ],
	"tags":["호텔 수영장","호텔 객실"],
	"place":"Carlton Hotel Bangkok Sukhumvit"
}
```
#### Response
- result - (string) “created” | “failed”
- videos - List\<Video\> index 된 비디오 목록
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지
```json

```

### POST /video (depricated) &nbsp; ✅ 
- Video creation
#### Request
- body
  - creator - (string) 비디오 생성자의 user_id
  - title	- (string) 제목
  - description	- (string, optional) 설명
  - location - (geo_point) 촬영 위치 (GPS 좌표)
  - categories - (List\<string\>)	카테고리 목록
  - tags - (List\<string\>, optional) tag 목록
  - video_type - (string, optional) 비디오 타입 ("shorts" \| "story" \| "guide" )
  - trim (optional)
    - start_time - (string) 시작 시간, timecode format HH:MM:SS:FF (ex. "00:00:10:21")
    - end_time - (string) 종료 시간, timecode format HH:MM:SS:FF (ex. "00:00:25:02")
  - place (TBD)
```json
{
    "title": "deluxe room - land view",
    "description": "2149, deluxe room, land view",
    "location": [126.4080501, 33.2524795],
    "categories": [
        "hotel"
    ],
    "tags": [
        "2149",
        "deluxe room",
        "land view",
        "객실"
    ],
    "creator": "beyondex", 
    "trim": {
      "start_time": "00:00:10:21",
      "end_time": "00:00:25:02"
    }
}
```
- attache video file (using http multipart format)  

Header 
```
Content-Type: multipart/form-data; boundary=...
```
Body
```
--e48ea1555b656ee039c3c7fcd62f12a5
Content-Disposition: form-data; name="json"
Content-Type: application/json

...

--e48ea1555b656ee039c3c7fcd62f12a5
Content-Disposition: form-data; name="video"; filename="video_file"
Content-Type: video/mp4

...

```
- request example (python code)
```python
import requests
import json
import sys

video_file = sys.argv[1]
url = 'http://3.36.33.69:8000/api/v1/video'
body = {
    'creator': 'beyondEx',
    'title': '신라호텔 수영장',
    'description': '신라호텔 수영장에서 호캉스',
    'location': [13.731260201818605, 100.5790655552985],
    'categories': ['호텔', '수영장'],
    'tags': ['신라호텔', '중문', '호캉스', '수영장', '인피니티풀']
}
files = {
        'json': (None, json.dumps(body), 'application/json'),
        'video': ('video_file', open(video_file, 'rb'), 'video/mp4'),
    }
requests.post(url, files=files)
```
#### Response
- result - (string) “created” | “failed”
- video - (Video [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#video) )
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지
```json
{
    "result": "created",
    "video": {
        "title": "deluxe room - land view",
        "description": "2149, deluxe room, land view",
        "location": [
            126.4080501,
            33.2524795
        ],
        "categories": [
            "hotel"
        ],
        "tags": [
            "2149",
            "deluxe room",
            "land view",
            "객실"
        ],
        "creator": "beyondex",
        "place": "100127127244262",
        "video_id": "000323951331445",
        "video_url": "https://dbgnxnfg295ht.cloudfront.net/Travel/user_video/000323951331445.m3u8",
        "thumbnail_url": "https://dbgnxnfg295ht.cloudfront.net/Travel/user_video/000323951331445.jpg",
        "district_name": [
            "제주특별자치도",
            "서귀포시",
            "색달동"
        ],
        "height": 720,
        "width": 1280,
        "start_time": 0.0,
        "duration": 1.988567,
        "resolution": "720p",
        "is_portrait": true,
        "upload_time": "2022-04-11T12:17:03.842225",
        "saves": 0,
        "reviews": 0,
        "views": 0
    }
}
```

### GET /video/{video_id} &nbsp; ✅ 
- Read video by video_id
#### Request
- path parameter
  - video_id - (string) 비디오 id
#### Response
- Video [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#video) 
  
### GET /video &nbsp; ✅ 
- Get videos by creator, place, and referrer video
- GET body를 허용안하는 http library를 위해 query parameter 지원
#### Request
- body
  - creator - (string) 생성한 크리에이터의 user_id
  - place - (string) 동영상이 속한 장소의 place_id
  - referrer - (string) 참조하는 동영상의 video_id, 결과 목록에 미포함됨 &nbsp; :heavy_multiplication_x:
  - pagenate
    - page - (integer) page 번호
    - size - (integer) page size
- expample) 크리에이터 user_a의 모든 동영상 목록
```javascript
{
  "creator": "user_a",
}
```
```javascript
GET /video?creator=user_a
```
- expample) 장소 place_b의 모든 동영상 목록, pagenate 시킴
```javascript
{
  "place": "place_b",
  "pagenate": {
    "page": 0,
    "size": 30,
  }
}
```
```javascript
GET /video?place=place_b&pagenate.page=0&pagenae.size=30
```
- example) user_a 가 place_b에서 찍은 동영상 목록 
```javascript
{
  "creator": "user_a",
  "place": "place_b"
}
```
```javascript
GET /video?creator=user_a&place=place_b
```
#### Response
- total - (integer) 동영상 개수
- videos - (List\<Video\>) 동영상 목록
```javascript
{
  "total": 39,
  "videos": [
    {
      // Video object
      "video_id": 523
      "title": "신라호텔 스위트룸", 
      ...
    },
    ...
  ]
}
```
### PUT /video/{video_id} &nbsp; ✅ 
- Update video with video_id
#### Request
- path parameter
  - video_id - (string) 비디오 id
- body
  - title	- (string) 제목
  - description	- (string) 설명
  - location - (geo_point) 촬영 위치 (GPS 좌표)
  - categories - (List\<string\>)	카테고리 목록
  - tags - (List\<string\>) tag 목록
#### Response
- result - (string) “updated” | "noop" | “failed”
- video - (Video [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#video) )
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지
  
### DELETE /video/{video_id} &nbsp; ✅ 
Delete video with video_id
#### Request
- path parameter
  - video_id - (string) 비디오 id
#### Response
- result - (string) “deleted” | “failed”
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지

### POST /video/{video_id}/review &nbsp; ✅ 
- 비디오에 리뷰 달기
#### Request
- path parameter
  - video_id - (string) video id
- body
  - user - (string) user_id
  - review_text - (string) 리뷰 내용
#### Response
- result - (string) “created” | “failed”
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지

### DELETE /video/{video_id}/review &nbsp; ✅ 
- 비디오 리뷰 지우기
#### Request
- path parameter
  - video_id - (string) video id
- body
  - review_id - (integer) review_id
#### Response
- result - (string) “deleted” | “failed”
- error (실패시)
  - code - (string) 에러 코드
  - message - (string) 에러 메시지

### GET /video/{video_id}/review &nbsp; ✅ 
- 비디오에 달린 리뷰 목록 읽기
#### Request
- path parameter
  - video_id - (string) video id
```javascript
GET /video/A/review
```
#### Response
- List<Review [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#Review)>
```javascript
[
  {
    "type": "video",
    "target_id": "A",
    "review_id": 1,
    "user_id": "B",
    "review_time": "2019-01-12T06:22:58",
    "review_text": "C"
  },
  {
    "type": "video",
    "target_id": "A",
    "review_id": 2,
    "user_id": "D",
    "review_time": "2019-01-12T06:23:03",
    "review_text": "E"
  }
]
