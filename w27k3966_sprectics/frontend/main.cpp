#include "httplib.h"
#include "json.hpp"
#include <iostream>
using namespace std;
using namespace httplib;
using json = nlohmann::json;

Client cli("http://43.200.10.134:5001");  // EC2 서버 주소
int user_id = -1;

// 로그인 함수
void m_1_login() {
    string nickname, password;

    cout << "로그인" << endl;
    cout << "nickname : ";
    cin >> nickname;

    cout << "password : ";
    cin >> password;

    json body;
    body["nickname"] = nickname;
    body["password"] = password;

    auto res = cli.Post("/users/login", body.dump(), "application/json");

    if (res) {


        try {
            json response = json::parse(res->body);
            if (response["status"] == "success") {
                cout << "✅ 로그인 성공 " << endl;
                user_id = response["user_id"];
                cout << "user_id: " << user_id << endl;
            } else {
                cout << "❌ 로그인 실패 " << endl;
                if (response.contains("reason"))
                    cout << "이유: " << response["reason"] << endl;
            }
        } catch (json::parse_error& e) {
            cout << "== 응답 파싱 실패 ==" << endl;
            cout << "에러 내용: " << e.what() << endl;
        }
    } else {
        cout << "요청 실패. 오류 코드: " << res.error() << endl;
    }
}

// 회원가입 함수
void m_1_signup() {
    string nickname, password, name, age, email;
    cin.ignore(); 

    cout << "[회원가입]" << endl;

    cout << "nickname: ";
    getline(cin, nickname);

    cout << "password: ";
    getline(cin, password);

    cout << "name: ";
    getline(cin, name);

    cout << "age (없으면 엔터): ";
    getline(cin, age);

    cout << "email (없으면 엔터): ";
    getline(cin, email);

    json body;
    body["nickname"] = nickname;
    body["password"] = password;
    body["name"] = name;
    if (!age.empty()) body["age"] = age;
    if (!email.empty()) body["email"] = email;

    auto res = cli.Post("/users", body.dump(), "application/json");

    if (res) {

        try {
            json response = json::parse(res->body);
            if (response["status"] == "created") {
                cout << "✅ 회원가입 성공 " << endl;
                user_id = response["user_id"];
                cout << "user_id: " << user_id << endl;
            } else {
                cout << "❌ 회원가입 실패 " << endl;
                if (response.contains("reason"))
                    cout << "이유: " << response["reason"] << endl;
            }
        } catch (json::parse_error& e) {
            cout << "== 응답 파싱 실패 ==" << endl;
            cout << "에러 내용: " << e.what() << endl;
        }
    } else {
        cout << "요청 실패. 오류 코드: " << res.error() << endl;
    }
}

// 로그인&회원가입 메뉴
void m_1_userLogin() {
    while (true) {
        cout << "[ 1. 로그인 & 회원가입 ]" << endl;
        cout << "1. 로그인" << endl;
        cout << "2. 회원가입" << endl;
        cout << "3. 뒤로가기" << endl;
        cout << "메뉴 선택: ";

        int input;
        cin >> input;
        switch (input) {
            case 1:
                m_1_login();
                break;
            case 2:
                m_1_signup();
                break;
            case 3:
                return;
                break;
            default:
                break;
        }
    }
}


// 1-1 내 정보 보기

void view_my_info() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요!" << endl;
        return;
    }

    string path = "/users/" + to_string(user_id);
    auto res = cli.Get(path.c_str());

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        json user = response["user"];

        cout << "\n[내 정보]" << endl;
        cout << "user_id : " << user["user_id"] << endl;
        cout << "닉네임   : " << user["nickname"] << endl;
        cout << "이름     : " << user["name"] << endl;
        cout << "이메일   : " << user["email"] << endl;
        cout << "나이     : " << user["age"] << endl;
    } else {
        cout << "❌ 내 정보를 불러오지 못했습니다." << endl;
        if (res) {
            cout << "상태 코드: " << res->status << endl;
            try {
                json response = json::parse(res->body);
                if (response.contains("reason"))
                    cout << "이유: " << response["reason"] << endl;
            } catch (...) {
                cout << "응답 파싱 실패" << endl;
            }
        } else {
            cout << "서버 응답 없음" << endl;
        }
    }
}


// 1-2 내 정보 수정
void update_my_info() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요" << endl;
        return;
    }

    string name, nickname, age, password, email;
    cin.ignore();
    cout << "\n[정보 수정]" << endl;
    cout << "닉네임 (변경할 닉네임 입력): ";
    getline(cin, nickname);
    cout << "이름 (변경할 이름 입력): ";
    getline(cin, name);
    cout << "비밀번호 (변경할 비밀번호입력): ";
    getline(cin, password);
    cout << "이메일 (변경할 이메일 입력): ";
    getline(cin, email);
    cout << "나이 (변경할 나이 입력): ";
    getline(cin, age);

    json body;
    body["name"] = name;
    body["email"] = email;
    body["nickname"] = nickname;
    body["age"] = age;
    body["password"] = password;

    string path = "/users/" + to_string(user_id);
    auto res = cli.Put(path.c_str(), body.dump(), "application/json");

    if (res && res->status == 200) {
        cout << "✅ 정보 수정 완료!" << endl;
    } else {
        cout << "❌ 정보 수정 실패." << endl;
    }
}

// 1-3 사용자 삭제
void delete_my_account() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요" << endl;
        return;
    }

    string path = "/users/" + to_string(user_id);
    auto res = cli.Delete(path.c_str());

    if (res && res->status == 200) {
        cout << "✅ 회원 탈퇴 완료." << endl;
        user_id = -1; // 초기화
    } else {
        cout << "❌ 회원 탈퇴 실패." << endl;
    }
}
// 다른 사용자 조회
void view_other_user() {
    string nickname;
    cin.ignore();
    cout << "\n[다른 사용자 조회]" << endl;
    cout << "조회할 사용자 닉네임 입력: ";
    getline(cin, nickname);

    string path = "/users/nickname/" + nickname;
    auto res = cli.Get(path.c_str());

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        json user = response["user"];

        cout << "\n[사용자 정보]" << endl;
        cout << "user_id : " << user["user_id"] << endl;
        cout << "닉네임   : " << user["nickname"] << endl;
        cout << "이름     : " << user["name"] << endl;
        cout << "이메일   : " << user["email"] << endl;
        cout << "나이     : " << user["age"] << endl;
    } else {
        cout << "❌ 사용자 정보를 불러올 수 없습니다." << endl;
        if (res) {
            try {
                json response = json::parse(res->body);
                if (response.contains("reason"))
                    cout << "상태 코드: " << res->status << " / 이유: " << response["reason"] << endl;
            } catch (...) {
                cout << "상태 코드: " << res->status << " / 응답 파싱 실패" << endl;
            }
        } else {
            cout << "서버 응답 없음" << endl;
        }
    }
}

void m_2_1_self() {
    while (true) {
        cout << "\n[2-1. 본인]" << endl;
        cout << "1. 내 정보 보기" << endl;
        cout << "2. 정보 수정하기" << endl;
        cout << "3. 사용자 삭제하기" << endl;
        cout << "4. 뒤로가기" << endl;

        int input;
        cin >> input;
        switch (input) {
            case 1:
                view_my_info();
                break;
            case 2:
                update_my_info();
                break;
            case 3:
                delete_my_account();
                break;
            case 4:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void m_2_2_others() {
    while (true) {
        cout << "\n[2-2. 다른 사용자]" << endl;
        cout << "1. 사용자 조회" << endl;
        cout << "2. 뒤로가기" << endl;

        int input;
        cin >> input;
        switch (input) {
            case 1:
                view_other_user();
                break;
            case 2:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void m_2_user() {
    while (true) {
        cout << "\n[2. 사용자]" << endl;
        cout << "1. 본인" << endl;
        cout << "2. 다른 사용자" << endl;
        cout << "3. 뒤로가기" << endl;

        int input;
        cin >> input;

        switch (input) {
            case 1:
                m_2_1_self();
                break;
            case 2:
                m_2_2_others();
                break;
            case 3:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

// [게시글 생성]
void m_0_mainMeun(); 

void create_post() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요" << endl;
        m_0_mainMeun(); 
        return;
    }

    string title, text;
    cin.ignore();

    cout << "[게시글 생성]" << endl;
    cout << "제목: ";
    getline(cin, title);
    cout << "내용: ";
    getline(cin, text);

    json body;
    body["title"] = title;
    body["text"] = text;
    body["user_id"] = user_id;

    auto res = cli.Post("/posts", body.dump(), "application/json");

    if (res && res->status == 200) {
        auto response = json::parse(res->body);
        if (response["status"] == "created") {
            cout << "✅ 게시글 등록 성공! post_id: " << response["post_id"] << endl;
        } else {
            cout << "❌ 실패: " << response["reason"] << endl;
        }
    } else {
        cout << "요청 실패. 서버 응답 없음" << endl;
    }
}

// [게시글 전체 조회]
void get_posts() {
    auto res = cli.Get("/posts");

    if (res && res->status == 200) {
        auto response = json::parse(res->body);
        cout << "\n[전체 게시글 목록]\n";
        for (auto& post : response["posts"]) {
            cout << "post_id : " << post["post_id"] << endl;
            cout << "제목     : " << post["title"] << endl;
            cout << "내용     : " << post["text"] << endl;
            cout << "작성자   : " << post["nickname"] << endl;
            cout << "작성일   : " << post["created_at"] << endl;
            cout << "------------------------" << endl;
        }
    } else {
        cout << " 게시글 조회 실패" << endl;
    }
}

// [댓글 작성]
void add_comment() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요" << endl;
        return;
    }

    string post_id, text;
    cin.ignore();

    cout << "[댓글 작성]" << endl;
    cout << "댓글을 달 post_id: ";
    getline(cin, post_id);
    cout << "댓글 내용: ";
    getline(cin, text);

    json body;
    body["user_id"] = user_id;
    body["text"] = text;

    string path = "/posts/" + post_id + "/comments";
    auto res = cli.Post(path.c_str(), body.dump(), "application/json");

    if (res && res->status == 200) {
        auto response = json::parse(res->body);
        if (response["status"] == "success") {
            cout << "✅ 댓글 등록 완료! comment_id: " << response["comment"]["comment_id"] << endl;
        } else {
            cout << "❌ 실패: " << response["reason"] << endl;
        }
    } else {
        cout << "요청 실패. 서버 응답 없음" << endl;
    }
}

// [댓글 조회]
void get_comments() {
    string post_id;
    cout << "댓글을 조회할 post_id: ";
    cin >> post_id;

    string path = "/posts/" + post_id + "/comments";
    auto res = cli.Get(path.c_str());

    if (res && res->status == 200) {
        auto response = json::parse(res->body);
        cout << "\n[댓글 목록]" << endl;
        for (auto& comment : response["comments"]) {
            cout << "comment_id : " << comment["comment_id"] << endl;
            cout << "user_id    : " << comment["user_id"] << endl;
            cout << "내용       : " << comment["text"] << endl;
            cout << "------------------------" << endl;
        }
    } else {
        cout << " 댓글 조회 실패" << endl;
    }
}

// 게시물 삭제
void delete_post() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요" << endl;
        return;
    }

    string post_id;
    cout << "[게시글 삭제]" << endl;
    cout << "삭제할 post_id 입력: ";
    cin >> post_id;

    string path = "/posts/" + post_id;

    json body;
    body["user_id"] = user_id;  

    auto res = cli.Delete(path.c_str(), body.dump(), "application/json");

    if (res && res->status == 200) {
        auto response = json::parse(res->body);
        if (response["status"] == "success") {
            cout << "✅ 게시글 삭제 완료!" << endl;
        } else {
            cout << "❌ 실패: " << response["reason"] << endl;
        }
    } else {
        cout << "요청 실패. 서버 응답 없음" << endl;
    }
}
// 댓글 삭제
void delete_comment() {
    if (user_id == -1) {
        cout << "\n‼️먼저 로그인해주세요" << endl;
        return;
    }

    string comment_id;
    cout << "[댓글 삭제]" << endl;
    cout << "삭제할 comment_id 입력: ";
    cin >> comment_id;

    string path = "/comments/" + comment_id;

    json body;
    body["user_id"] = user_id;

    auto res = cli.Delete(path.c_str(), body.dump(), "application/json");

    if (res && res->status == 200) {
        auto response = json::parse(res->body);
        if (response["status"] == "success") {
            cout << "✅ 댓글 삭제 완료!" << endl;
        } else {
            cout << "❌ 실패: " << response["reason"] << endl;
        }
    } else {
        cout << "요청 실패. 서버 응답 없음" << endl;
    }
}

void m_3_posting() {
    while (true) {
        cout << "\n[3. 포스팅 메뉴]" << endl;
        cout << "1. 게시글 생성" << endl;
        cout << "2. 게시글 전체 조회" << endl;
        cout << "3. 댓글 달기" << endl;
        cout << "4. 댓글 조회" << endl;
        cout << "5. 게시글 삭제" << endl;     
        cout << "6. 댓글 삭제" << endl;        
        cout << "7. 뒤로가기" << endl;

        int input;
        cin >> input;
        switch (input) {
            case 1: create_post(); break;
            case 2: get_posts(); break;
            case 3: add_comment(); break;
            case 4: get_comments(); break;
            case 5: delete_post(); break;
            case 6: delete_comment(); break;
            case 7: return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}



// 메인 메뉴
void m_0_mainMeun() {
    while (1) {
        cout << "[홈메뉴]" << endl;
        cout << "1. 로그인" << endl;
        cout << "2. 사용자" << endl;
        cout << "3. 포스팅" << endl;
        cout << "4. DM" << endl;
        cout << "5. 종료" << endl;
        cout << "메뉴 선택: ";

        int input;
        cin >> input;
        cin.ignore();

        switch(input) {
            case 1:
                m_1_userLogin();
                break;
            case 2:
                m_2_user();
                break;
            case 3:
                m_3_posting();
                break;
            case 4:
                break;
            case 5:
                cout << "프로그램을 종료합니다." << endl;
                exit(0);  
            default:
                break;
        }
    }
}

int main() {
    m_0_mainMeun();  
    return 0;
}