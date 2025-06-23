# Search
키워드 검색, 위치 검색, 자동완성, 인기 검색어  
### GET /search &nbsp; 
- 키워드 검색, 위치 검색  
- filter, sort, pagenation
- GET body를 허용안하는 http library를 위해 POST도 동일하게 동작 
#### Request
- body
  - query - (string) 검색 키워드    ✅ 
  - nearby ✅ 
    - location - (geo_point) 위치 검색할 GPS 좌표, geojson format - [경도, 위도] 
    - range - (string) 검색 범위 지정 (단위 m | km, default m)
  - filter
    - filter - (Filter [Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#filter)) 필터 object format으로 선택한 필드 포함
    - categories - (List\<string\>) 카테고리 목록 (ex. "팬션", "음식점", "까페")  ✅ 
    - keywords - (List\<string\>) 필터 키워드 목록 (ex. "무료 취소", "수영장", "한식", "수상스키")
    - creator - (string) 동영상 크리에이터의 user_id ✅ 
    - place - (string) 동영상 장소의 place_id 
  - sort - (string) [Sort Object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#filter) 정렬 기준에서 선택
    - reviews, saves, views  ✅ 
    - review_rate, nearby
  - pagenate
    - page - (integer) page 번호
    - size - (integer) page size
  - distict
    - creator - (boolean) creator 중복 제거, default true
    - place - (boolean) place 중복 제거, default true
- example) 
```json
{
  "query": "방콕 스쿰빗",
  "filter": { 
    "categories": ["hotel", "restaurant"],
    "creator": "beyondEx"
  },
  "nearby": {
    "location": [100.56453771231412,13.734759888812238], 
    "range": "5km"  
  }
}
```

- example) 제주도 중문 근처 즐길거리 검색
  - 제주도 중문 근처 -> 위치 검색, 즐길거리 -> 카테고리 가중치 [Semantic] 
```json
{
  "query": "제주도 중문 근처 즐길거리"
}
```  
- example) 제주도 중문의 4,5 등급 호텔 필터, 장소에 대한 평점순 정렬
```json
{
  "query": "제주도 중문",
  "filter": {
    "filter": {
      "hotel": {
        "level":[4,5]
      }
  },
  "sort": "review_rate"
}
```
- example) 내주변 1.5km내의 음식점 거리순으로 정렬
```json
{
  "nearby": {
    "location": [-71.34, 41.12] 
    "range": "1500"   // 1500m
  },
  "filter": {
    "categories": ["음식점"]
  },
  "sort": "nearby"
}
```
- example) 크리에이터 A가 찍은 장소 B의 모든 동영상 
```json
{
  "filter": {
    "creator": "A",
    "place": "B"
  },
  "distinct": {
    "creator": false,
    "place": false
  }
}
```
#### Response
- 크리에이터&장소 별로 하나의 동영상만 결과에 포함하여 응답
- body
  - took - (integer) API 소요시간 (ms)
  - timed_out - (boolean) es engine timeout 여부
  - max_score - (float) 검색 결과 최대 스코어
  - total - (integer) 검색 결과 개수
  - results - (List\<Result\>) 검색 결과 목록
    - Result
      - score - (float) 검색 스코어
      - video - (Video) 검색된 video [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#video)
      - place - (Place) video가 속한 place [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#place)
      - creator - (User) video의 creator [object](https://github.com/myideom/beyondEx/blob/main/docs/%EC%9C%84%EC%B9%98%EA%B8%B0%EB%B0%98%20%EB%B9%84%EB%94%94%EC%98%A4%20%EA%B2%80%EC%83%89%20%EC%84%9C%EB%B9%84%EC%8A%A4/MVP%20%EA%B0%9C%EB%B0%9C/API/API%20Reference%20v1.md#user)
- example)
```javascript
{
  "took": 120,
  "timed_out": false,
  "max_score": 8.56,
  "total": 39,
  "results": [
    {
      "score": 8.56,
      "video": {            // Video object
        "video_id": 523
        "title": "신라호텔 스위트룸", 
          ...
      }
      "place": {            // Place object
        "place_id": 21,
        "name": "신라호텔",
        ...
      },
      "creator": {          // User object
        "user_id": 98,
        "nickname": "beyondEx",
        ...
      }
    },
    ...
  ]
}
```
### GET /filter &nbsp; 
- Request
  - body (TBD)
- Response
  - Filter Object
```json
{
  "hotel": {
    "price": ["$~$$", "$$~$$$", "$$$~$$$$"],
    "cancelation": ["free"]
    "level": [1, 2, 3, 4, 5],
    "type": ["hotel", "pansion", "motel", "guest house"],
    "facility": ["swimming pool", "bath hub", "spa", "fitness"],
    "bed": ["single", "double", "twin"],
    "meal": ["breakfast", "dinner"]
  }
  "restaurant": {
    "price": ["$~$$", "$$~$$$", "$$$~$$$$"],
    "type": ["restaurant", "dessert", "coffee & tea", ... ]
    "Cuisine": ["Italian", ... ]
  }
  "tour": {
    "tour": [...],
    "ativity": [...]
  }
}
```

### GET /sort &nbsp; 
- Request
  - body (TBD)
- Response
  - Sort Object
```json
["review rate", "like", "nearby", "save", "view"]
```

### GET /autocomplete &nbsp; 🔲
- Request
  - query parameter
    - partial_query - (string) 입력중인 부분 키워드
    - nearby - (geo_point) 사용자의 위치 (GPS 좌표)
```shell
GET /autocomplete?partial_query=중문호&nearby=41.12,-71.34
```
- Response
  - keywords - (List\<string\>) 검색어 목록
### GET /popular_keyword &nbsp; 🔲
- (여행전) 여행전 필요한 인기 검색어
- (여행중) 여행시 위치 기반 인기 검색어
- Request
  - query parameter
    - nearby - (geo_point) 내위치 GPS 좌표
```shell
GET /popular_keywords?nearby=41.12,-71.34
```
- Response
    - keywords - (List\<string\>) 인기 검색어 목록
