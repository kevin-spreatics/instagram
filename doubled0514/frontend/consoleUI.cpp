
#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;
httplib::Client cli("http://localhost:5000");
bool isLoggedIn = false;
string currentUser;

void showLoginMenu();
void showUserMenu();
void showSocialMenu();
void showDMMenu();
void showProfileMenu();
void showMyProfileMenu(const std::string& userId);
void showOtherUserMenu(const std::string& targetUser);
void myinfo();
void mypost(const std::string& userId);
void myfollowing();
void sendingDM();
void sendingfollow();
void otherpost(const std::string& userId);
void showpost(const std::string& userId);

int main() {
    int choice;
    
    while (true) {
        cout << "\n[메인메뉴]" << endl;
        cout << "1. 로그인/회원가입" << endl;
        if (isLoggedIn) {
            cout << "2. 사용자 기능" << endl;
            cout << "3. 소셜" << endl;
            cout << "4. DM함" << endl;
        }
        cout << "0. 종료" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                showLoginMenu();
                break;
            case 2:
                if (isLoggedIn) showUserMenu();
                else cout << "로그인이 필요합니다.\n";
                break;
            case 3:
                if (isLoggedIn) showSocialMenu();
                else cout << "로그인이 필요합니다.\n";
                break;
            case 4:
                if (isLoggedIn) showDMMenu();
                else cout << "로그인이 필요합니다.\n";
                break;
            case 0:
                cout << "종료합니다." << endl;
                return 0;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void showLoginMenu() {
    int choice;
    cout << "\n[로그인/회원가입]" << endl;
    cout << "1. 로그인" << endl;
    cout << "2. 회원가입" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";
    cin >> choice;
    switch (choice) {
        case 1: {
        string nickname, password;
        cout << "닉네임 입력: ";
        cin >> nickname;
        cout << "비밀번호 입력: ";
        cin >> password;

        json loginData;
        loginData["nickname"] = nickname;
        loginData["password"] = password;

        auto res = cli.Post("/users/login", { {"Content-Type", "application/json"} }, loginData.dump(), "application/json");

        if (res && res->status == 200) {
            json response = json::parse(res->body);
            if (response["status"] == "success") {
                isLoggedIn = true;
                currentUser = to_string(response["user_id"].get<int>()); // user_id를 currentUser로 저장
                cout << nickname << "님 로그인 성공 (user_id: " << currentUser << ")" << endl;
            } else {
                cout << "로그인 실패: " << response["reason"] << endl;
            }
        } else {
            cout << "서버 오류 또는 연결 실패" << endl;
            if (res) cout << res->status << " " << res->body << endl;
            else cout << res.error() << endl;
    }
    break;}

        case 2: {
        string name, password, nickname, email, ageStr;
        int age = -1;

        cout << "[회원가입]" << endl;
        
        cout << "닉네임 (필수): ";
        cin >> nickname;

        cout << "비밀번호 (필수): ";
        cin >> password;

        cout << "이름 (필수): ";
        cin >> name;

        cin.ignore(); // 개행 문자 제거

        cout << "이메일 (선택, 입력하지 않으면 엔터): ";
        getline(cin, email);

        cout << "나이 (선택, 입력하지 않으면 엔터): ";
        getline(cin, ageStr);


        json data;
        data["name"] = name;
        data["nickname"] = nickname;
        data["password"] = password;

        if (!email.empty()) {
            data["email"] = email;
        } // 비어있지 않으면 email 항목 추가

        if (!ageStr.empty()) {
            try {
                age = stoi(ageStr);
                data["age"] = age;
            } catch (...) {
                cout << "나이 입력이 잘못되었습니다. 숫자만 입력해주세요." << endl;
            }
        } // 비어있지 않으면 나이 항목 추가 , stoi : 문자열으 정수로 

        auto res = cli.Post("/users", { {"Content-Type", "application/json"} }, data.dump(), "application/json");

        if (res && res->status == 200) {
            cout << "회원가입 성공: " << res->body << endl;
        } else {
            cout << "회원가입 실패: ";
            if (res) cout << res->status << " " << res->body << endl;
            else cout << res.error() << endl;
        }
        break;
    }

        case 0:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
    }
}

void showUserMenu() {
    string targetUser;
    cout << "\n조회할 사용자 ID 입력: ";
    cin >> targetUser;

    if (targetUser == currentUser) {
        showMyProfileMenu(currentUser);
    } else {
        showOtherUserMenu(targetUser);
    }
}
// 사용자 정보 조회용 함수 
bool fetchAndDisplayMyInfo(const string& userid) {
    json payload;
    payload["user_id"] = userid;

    auto res = cli.Post("/users/search", { {"Content-Type", "application/json"} }, payload.dump(), "application/json");

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        if (response["status"] == "success") {
            json user = response["user"];
            cout << "\n[프로필]" << endl;
            cout << "닉네임: " << user["nickname"] << endl;
            cout << "이름: " << user["name"] << endl;
            cout << "이메일: " << user["email"] << endl;
            cout << "나이: " << user["age"] << endl;
            return true;
        } else {
            cout << "사용자 정보 조회 실패: " << response["reason"] << endl;
        }
    } else {
        cout << "사용자 정보 조회 실패!" << endl;
        if (res) cout << res->body << endl;
    }
    return false;
}




void showMyProfileMenu(const string& userid) {
    fetchAndDisplayMyInfo(userid); 
    int choice;
    while (true) {

        cout << "1. 내 정보 수정/삭제" << endl;
        cout << "2. 내 포스트 조회/작성" << endl;
        cout << "3. 팔로워/팔로잉, 팔로우 신청 관리" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                myinfo();
                break;
            case 2:
                mypost(userid);
                break;
            case 3:
                myfollowing();
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void showOtherUserMenu(const string & targetUser) {
    int choice;
    if (!fetchAndDisplayMyInfo(targetUser)) {
    return;
    }
    while (true) {
        cout << "\n[" << targetUser << "의 프로필]" << endl;
        cout << "1. DM 보내기" << endl;
        cout << "2. 팔로우 신청하기" << endl;
        cout << "3. 포스트 보기 및 댓글 작성" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                sendingDM();
                break;
            case 2:
                sendingfollow();
                break;
            case 3:
                otherpost(targetUser);
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}


void myinfo(){
    int choice;
    while (true) {
        cout << "1. 사용자 정보 수정" << endl;
        cout << "2. 사용자 정보 삭제" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string nickname, email;
                int age;

                cout << "새 닉네임: ";
                cin >> nickname;
                cout << "새 이메일: ";
                cin >> email;
                cout << "새 나이: ";
                cin >> age;

                json payload;
                payload["nickname"] = nickname;
                payload["email"] = email;
                payload["age"] = age;

                auto res = cli.Put(("/users/" + currentUser), payload.dump(), "application/json");

                if (res && res->status == 200) {
                    cout << "사용자 정보가 성공적으로 수정되었습니다." << endl;
                } else {
                    cout << "수정 실패: ";
                    if (res) cout << res->status << endl;
                    else cout << res.error() << endl;
                }
                break;
            }
            case 2: {
                auto res = cli.Delete(("/users/" + currentUser).c_str());

                if (res && res->status == 200) {
                    cout << "사용자 정보 삭제 완료." << endl;
                    isLoggedIn = false;
                    currentUser = "";
                } else {
                    cout << "삭제 실패: ";
                    if (res) cout << res->status << endl;
                    else cout << res.error() << endl;
                }
                break;
            }
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void fetchPostsByUser(const string& user_id) {
    json payload;
    payload["user_id"] = user_id;

    auto res = cli.Post("/post/search", { {"Content-Type", "application/json"} }, payload.dump(), "application/json");

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        if (response["status"] == "success") {
            for (const auto& post : response["posts"]) {
                cout << "- post_id: " << post["post_id"] << "\n  제목: " << post["title"] 
                     << "\n  내용: " << post["text"] << "\n  작성일: " << post["created_at"] 
                     << "\n  닉네임: " << post["nickname"] << endl << endl;
            }
        } else {
            cout << "📛 " << response["reason"] << endl;
        }
    } else {
        cout << "⚠️ 서버 오류!" << endl;
        if (res) cout << res->body << endl;
    }
}

void createPost(const string& user_id) {
    string title, text;
    cin.ignore(); // 이전 입력 버퍼 비우기

    cout << "포스트 제목: ";
    getline(cin, title);

    cout << "포스트 내용: ";
    getline(cin, text);

    json payload;
    payload["user_id"] = user_id;
    payload["title"] = title;
    payload["text"] = text;

    auto res = cli.Post("/post", { {"Content-Type", "application/json"} }, payload.dump(), "application/json");

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        if (response["status"] == "success") {
            cout << " 포스트 작성 완료!" << endl;
        } else {
            cout << " 작성 실패: " << response["reason"] << endl;
        }
    } else {
        cout << " 서버 연결 실패 또는 예외 발생" << endl;
        if (res) cout << res->body << endl;
    }
}


void mypost(const string & userid){
    int choice ;
    while (true){
        fetchPostsByUser(userid);
        cout << "1.포스트 작성"<<endl;
        cout << "2.특정 포스트 조회" << endl;
        cout << "0.뒤로가기"<<endl;
        cout << "선택:";
        cin >> choice;
        switch (choice) {
            case 1:{
                createPost(userid);
                break;}
            case 2:{
                string post_id;
                cout << "조회할 post_id를 입력하세요: ";
                cin >> post_id;
                showpost(post_id);
                break;}
            case 0:{
                cout << "뒤로가기" << endl;
                return;}
            default:
                {cout << "잘못된 입력입니다." << endl;
                break;}
            }


}}



void otherpost(const string & userid){
    int choice;
    while(true){
        fetchPostsByUser(userid);
        cout << "1.특정 포스트 조회하기"  << endl;
        cout << "0. 뒤로가기"<< endl;
        cout << "선택 : ";
        cin >> choice;
        switch(choice){
            case 1: {
                string post_id;
                cout << "조회할 post_id를 입력하세요: ";
                cin >> post_id;
                showpost(post_id);
                break;}
            case 0 :{
                cout <<"뒤로가기"<< endl;
                return;}
            default:
            cout << "잘못된 입력입니다." << endl;    
}}}
void createCommentForPost(const string& post_id, const string& user_id) {
    cin.ignore();
    string text;
    cout << "댓글을 입력하세요: ";
    getline(cin, text);

    json payload;
    payload["text"] = text;

    string endpoint = "/post/" + post_id + "/comment/" + user_id;

    auto res = cli.Post(endpoint.c_str(), { {"Content-Type", "application/json"} }, payload.dump(), "application/json");

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        if (response["status"] == "success") {
            cout << " 댓글 작성 완료! (comment_id: " << response["comment_id"] << ")" << endl;
        } else {
            cout << " 실패: " << response["reason"] << endl;
        }
    } else {
        cout << " 서버 오류 발생!" << endl;
        if (res) cout << res->body << endl;
    }
}
void fetchCommentsForPost(const string& post_id) {
    json payload;
    payload["post_id"] = post_id;
    string endpoint = "/post/" + post_id + "/comment";

    auto res = cli.Post(endpoint.c_str(), { {"Content-Type", "application/json"} }, payload.dump(), "application/json");

    if (res && res->status == 200) {
        json comments = json::parse(res->body);

        if (!comments.is_array()) {
            cout << comments.dump(2) << endl;
            return;
        }

    } else {
        cout << " 댓글 조회 실패!" << endl;
        if (res) cout << res->body << endl;
    }
}

void showpost(const string& post_id) {
    json payload;
    payload["post_id"] = post_id;

    auto res = cli.Post("/post/search", { {"Content-Type", "application/json"} }, payload.dump(), "application/json");

    if (res && res->status == 200) {
        json response = json::parse(res->body);
        if (response["status"] == "success" && !response["posts"].empty()) {
            json post = response["posts"][0];
            cout << "\n[포스트 상세]" << endl;
            cout << "제목: " << post["title"] << endl;
            cout << "작성자: " << post["nickname"] << endl;
            cout << "내용: " << post["text"] << endl;
            cout << "작성일: " << post["created_at"] << endl;
        } else {
            cout << " 해당 post_id에 대한 포스트를 찾을 수 없습니다." << endl;
            return;
        }
    } else {
        cout << "서버 오류: ";
        if (res) cout << res->body << endl;
        return;
    }

    // 코멘트 작성 메뉴
    int choice;
    while (true) {
        fetchCommentsForPost(post_id); // 댓글 자동 조회
        cout << "\n1. 코멘트 작성하기" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createCommentForPost(post_id, currentUser); // currentUser는 로그인된 user_id
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void myfollowing(){
    int choice;
    while(true){
        cout << "1.팔로워/팔로잉 조회" <<endl;
        cout << "2.팔로우 신청 조회" << endl;
        cout << "0.뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice ;
        switch(choice){
            case 1: 
                cout << "팔로워/팔로잉 목록 조회" << endl;
                break;
            case 2:
                cout << "팔로우 신청 조회" <<endl;
                break;
            case 0 :
                cout <<"뒤로가기"<< endl;
                break;
            default:
            cout << "잘못된 입력입니다." << endl;
        }
    }
}
void sendingDM(){
    int choice;
    while(true){     
        cout << "DM 보내기 " << endl;
        break;
}}
void sendingfollow(){
    int choice;
    while(true){
        cout << "1.팔로잉/팔로우 조회하기" << endl;
        cout << "2.팔로우 신청하기" << endl;
        cout << "0. 뒤로가기"<< endl;
        cout << "선택 : ";
        cin >> choice;
        switch(choice){
            case 1: 
                cout << "팔로워/팔로잉 목록 조회" << endl;
                break;
            case 2:
                cout << "팔로우 신청 하기" <<endl;
                break;
            case 0 :
                cout <<"뒤로가기"<< endl;
                return;
            default:
            cout << "잘못된 입력입니다." << endl;
    }
}}

void showSocialMenu() {
    cout << "[소셜 기능은 아직 구현되지 않았습니다.]" << endl;
}

void showDMMenu() {
    cout << "[DM 기능은 아직 구현되지 않았습니다.]" << endl;
}