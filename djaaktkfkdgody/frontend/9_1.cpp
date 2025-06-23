// #include <iostream>
// #include <thread>

// void say_hello() {
//     std::cout << "Hello from thread!\n";
// }

// int main() {
//     std::thread t(say_hello);
//     t.join();  // 쓰레드 종료까지 대기
//     return 0;
// }












// #include "httplib.h"
// #include <iostream>

// using namespace std;
// using namespace httplib;

// int main() {
//     Client cli("127.0.0.1:5000");

//     string body = R"({
//     "nickname" : "kevin",
//     "password" : "1234",
//     "name" : "이승학"
//     })";

//     auto res = cli.Post("/users", body, "application/json");

//     if (res)
//         cout << res->body << endl;
//     else
//         cout << res.error() << endl;

//     return 0;
    
// }

// #include "httplib.h"
// #include <iostream>

// using namespace std;
// using namespace httplib;

// int main() {
//     Client cli("127.0.0.1:5000");

//     // 1. POST: 사용자 생성
//     string body = R"({
//         "nickname" : "kevin",
//         "password" : "1234",
//         "name" : "이승학"
//     })";

//     auto res_post = cli.Post("/users", body, "application/json");
//     if (res_post)
//         cout << "[POST] 응답: " << res_post->body << endl;
//     else
//         cout << "[POST] 오류: " << static_cast<int>(res_post.error()) << endl;

//     // 2. GET: 사용자 조회
//     int user_id = 1;
//     string get_endpoint = "/users/" + to_string(user_id);
//     auto res_get = cli.Get(get_endpoint);

//     if (res_get && res_get->status == 200)
//         cout << "[GET] 사용자 정보: " << res_get->body << endl;
//     else
//         cout << "[GET] 오류 또는 사용자 없음" << endl;

//     // 3. DELETE: 사용자 삭제
//     string del_endpoint = "/users/" + to_string(user_id);
//     auto res_del = cli.Delete(del_endpoint);

//     if (res_del && res_del->status == 200)
//         cout << "[DELETE] 사용자 삭제됨: " << res_del->body << endl;
//     else
//         cout << "[DELETE] 삭제 실패" << endl;

//     return 0;
// }


// #include "httplib.h"
// #include <iostream>
// #ifdef _WIN32
// #include <windows.h>
// #endif

// using namespace httplib;
// using namespace std;

// int main() {
// #ifdef _WIN32
//     SetConsoleOutputCP(CP_UTF8); // Windows에서 한글 깨짐 방지
// #endif

//     // HTTP 클라이언트 생성 (127.0.0.1:5000 서버에 연결)
//     Client cli("127.0.0.1", 5000);

//     // 사용자 ID 지정
//     int user_id = 1;

//     // 사용자 정보 가져오기 (GET)
//     string get_endpoint = "/users/" + to_string(user_id);
//     auto res = cli.Get(get_endpoint.c_str());
//     if (res && res->status == 200) {
//         cout << "사용자 정보 조회 성공:\n" << res->body << endl;
//     } else {
//         cout << "사용자 정보 조회 실패" << endl;
//     }

//     // 사용자 삭제하기 (DELETE)
//     string delete_endpoint = "/users/" + to_string(user_id);
//     res = cli.Delete(delete_endpoint.c_str());
//     if (res && res->status == 200) {
//         cout << "사용자 삭제 성공" << endl;
//     } else {
//         cout << "사용자 삭제 실패" << endl;
//     }

//     return 0;
// }


