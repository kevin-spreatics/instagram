#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace httplib;
using namespace nlohmann;

void login();   // 로그인
void sign_up(); // 회원가입 
void login_menu();  // 로그인 화면 메뉴
void choosing_post_menu();  //특정 포스트 선택시 메뉴
void posting_menu();    // 포스트 기능 메뉴
void search_menu(); // 검색 기능 메뉴
void home_menu();   // 메인 메뉴
void me_menu(); // 본인 기능 메뉴
void logout();  // 로그아웃 기능
void check_me();    // 내 정보 보기
void update_me();   // 내 정보 수정
void delete_me();   // 내 계정 삭제
void check_my_posts();  // 내가 작성한 포스트 보기
void check_my_comments();   // 내가 작성한 댓글 보기
void upload_post(); // 포스트 올리기
void post_list();   // 포스트 목록 조회
void choose_post(string); // 특정 포스트 선택
void write_comment();   // 댓글 쓰기
void comment_list(string);    // 해당 포스트 댓글 목록 조회
void search_user(); // 특정 유저 검색
void search_post(); // 특정 포스트 검색

Client cli("3.34.112.5:5000");
string user_input;
string session_cookie; // 세션에 저장되어있는 user_id
string current_post_id; 
bool login_success = false;
bool end_input = false;


void login(){
    string endpoint = "/users/login";
    string nickname = "";
    string password = "";
    string s;
    
    cout << "[로그인]" << endl;
    cout << "아이디를 입력해주세요 : ";
    getline(cin, nickname);
    cout << "비밀번호를 입력해주세요 : ";
    getline(cin, password);

    json body;
    body["nickname"] = nickname;
    body["password"] = password;

    auto res = cli.Post(endpoint,body.dump(),"application/json");

    if (res){
        try{
            json response = json::parse(res->body);
            if(response["status"] == "success"){
                cout << "로그인 성공!" << endl;
                login_success = true;

                auto it = res->headers.find("Set-Cookie");
                 if (it != res->headers.end()) 
                    session_cookie = it->second;
            }
            else if(response["status"] == "failed"){
                s = "nickname, " + nickname +" doesn't exist";
                if(response["reason"] == "nickname is None.")
                    cout << "로그인 실패 : 아이디를 입력해주세요." << endl;
                else if(response["reason"] == "password is None.")
                    cout << "로그인 실패 : 비밀번호를 입력해주세요." << endl;
                else if(response["reason"] == s)
                    cout << "로그인 실패 : 존재하지 않는 아이디입니다." << endl;
                else if(response["reason"] == "Already logged in")
                    cout << "로그인 실패 : 이미 로그인 되어있습니다." << endl;
                else if(response["reason"] == "password, password doesn't match")
                    cout << "로그인 실패 : 비밀번호가 일치하지 않습니다." << endl;
                else
                    cout << "로그인 실패" << endl; 
            }
        
        } catch (const json::parse_error& e) {
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else
        cout << res.error() << endl;
    cout << endl;
}
void sign_up(){
    string endpoint = "/users/signup";
    string nickname="";
    string name="";
    string password="";
    string age="";
    string email="";
    string s;

    cout << "[회원가입]" << endl;
    cout << "아이디를 입력해주세요(필수) : ";
    getline(cin, nickname);
    cout << "비밀번호를 입력해주세요(필수) : ";
    getline(cin, password);
    cout << "이름을 입력해주세요(필수) : ";
    getline(cin, name);
    cout << "나이를 입력해주세요 : ";
    getline(cin,age);
    cout << "이메일을 입력해주세요 : ";
    getline(cin, email);

    json body;
    body["nickname"] = nickname;
    body["name"] = name;
    body["password"] = password;
    body["age"] = age;
    body["email"] = email;

    auto res = cli.Post(endpoint,body.dump(),"application/json");
    
    if (res){
        try{
            json response = json::parse(res->body);
            if(response["status"] == "success"){
                cout << "회원가입 성공!" << endl;
                if(response.contains("user_id"))
                    cout << "user_id : " << response["user_id"] << endl;
            }
            else if(response["status"] == "failed"){
                s = "nickname, " + nickname + " is duplicated";
                if(response["reason"] == s)
                    cout << "회원가입 실패 : 아이디가 이미 존재합니다." << endl;
                else 
                    cout << "회원가입 실패 : 아이디, 이름, 비밀번호는 필수 입력입니다."  << endl;
            }
        
        } catch (const json::parse_error& e) {
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else
        cout << res.error() << endl;
    
    cout << endl;
}

void login_menu(){
    user_input = "";
    while(1){
        cout << "[로그인 메뉴]" << endl;
        cout << "| 1. 로그인 | 2. 회원가입 |" << endl << endl;;
        cout << "이용할 서비스 번호를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "1"){
            login();
            if(login_success == true)   // 로그인 성공해야 메인메뉴로 감
                home_menu();
            if(end_input == true){
                break;
            }
        }
        else if(user_input == "2"){
            sign_up();
        }
    }   
}
void me_menu(){
    user_input = "";
    while(1){
        cout << "[내 정보 메뉴]" << endl;
        cout << "| 1. 사용자 정보 조회 | 2. 사용자 정보 변경 | 3. 사용자 삭제 | 4. 내가 쓴 포스트 보기 | 5. 내가 쓴 댓글 보기 | 6. 뒤로가기 |" << endl;
        cout << "이용할 서비스 번호를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "1"){
            check_me();
        }
        else if(user_input == "2"){
            update_me();
        }
        else if(user_input == "3"){
            delete_me();
            break;
        }
        else if(user_input == "4"){
            check_my_posts();
        }
        else if(user_input == "5"){
            check_my_comments();
        }
        else if(user_input == "6"){
            return;
        }
    }
}
void choosing_post_menu(){
    current_post_id = "";
    user_input = "";
        while(1){
            post_list();
            cout << "| 1. 포스트 내용보기 | 2. 뒤로가기 |" << endl;
            cout << "이용할 서비스 번호를 입력하세요. : ";
            getline(cin, user_input);
            cout << endl;

            if(user_input == "1"){
                cout << "내용을 볼 포스트 번호를 입력하세요 : ";
                getline(cin, user_input);
                current_post_id = user_input;
                while(1){
                    choose_post(current_post_id);
                    cout << "| 1. 댓글입력 | 2. 뒤로가기|" << endl;
                    cout << "이용할 서비스 번호를 입력하세요. : ";
                    getline(cin, user_input);
                    cout << endl;
                    if(user_input == "1"){
                        user_input = "";
                        write_comment();
                    }
                    else if(user_input == "2"){
                        user_input = "";
                        break;
                    }
                }
            }
            if(user_input == "2")
                return;
        }
}
void posting_menu(){
    user_input="";
    while(1){
        cout << "[포스팅 메뉴]" << endl;
        cout << "| 1. 포스트 작성 | 2. 포스트 목록 조회 | 3. 뒤로가기 |" << endl;
        cout << "이용할 서비스 번호를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "1"){
            upload_post();
        }
        else if(user_input == "2"){
            choosing_post_menu();
        }
        else if(user_input == "3"){
            return;
        }
    }
}
void search_menu(){
    user_input="";
    while(1){
        cout << "[검색 메뉴]" << endl;
        cout << "| 1. 다른 사용자 검색 | 2. 포스트 검색 | 3. 뒤로가기 |" << endl;
        cout << "이용할 서비스 번호를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "1"){
            search_user();
        }
        else if(user_input == "2"){
            search_post();
        }
        else if(user_input == "3"){
            return;
        }
    }
}
void home_menu(){
    user_input="";
    while(1){
        cout << "[메인메뉴]" << endl;
        cout << "| 1. 내 정보 | 2. 포스팅 | 3. 탐색 | 4. 로그아웃 | 5. 종료 |" << endl;
        cout << "이용할 서비스 번호를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "1"){
            me_menu();
            if(user_input == "3"){  //사용자 삭제시
                break;
            }
        }
        else if(user_input == "2"){
            posting_menu();
        }
        else if(user_input == "3"){
            search_menu();
        }
        else if(user_input == "4"){
            logout();
            break;
        }
        else if(user_input == "5"){
            cout << "프로그램을 종료합니다." << endl;
            end_input = true;
            break;
        }
    }
}
void logout(){
    cout << "로그아웃이 완료되었습니다." << endl << endl;
    login_success = false;
}
void check_me(){
    string endpoint = "/users/me";

    Headers headers = {
        {"Cookie", session_cookie}
    };

    auto res = cli.Get(endpoint, headers);

    if (res){
        try {
            json response = json::parse(res->body);
            if(response["status"] == "success"){
                auto user = response["user"];
                cout << "[내 정보]" << endl;
                cout << "user_id: " << user["user_id"] << endl;
                cout << "nickname: " << user["nickname"] << endl;
                cout << "name: " << user["name"] << endl;
                cout << "email: " << user["email"] << endl;
                cout << "age: " << user["age"] << endl;
            }
            else{
                cout << "조회 실패: " << response["reason"] << endl;
            }
        } catch (const json::parse_error& e){
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else{
        cout << "서버 연결 오류 또는 응답 없음" << endl;
    }
    cout << endl;
}

void update_me(){
    cout << "[사용자 정보 변경]" << endl;
    cout << "정보 변경 완료" << endl << endl;
}
void delete_me(){
    cout << "[사용자 삭제]" << endl;
    cout << "사용자 삭제 완료" << endl << endl;
}
void check_my_posts(){
    cout << "[내가 쓴 포스트 보기]" << endl;
    cout << "내가 쓴 포스트 목록들" << endl << endl;
}
void check_my_comments(){
    cout << "[내가 쓴 댓글 보기]" << endl;
    cout << "내가 쓴 댓글 목록들" << endl << endl;
}
void upload_post(){
    string endpoint = "/posts/upload";
    string title;
    string text;

    Headers headers = {
        {"Cookie", session_cookie}
    };

    cout << "[포스트 작성]" << endl;
    cout << "포스트 제목을 적어주세요 : ";
    getline(cin,title);
    cout << "포스트 내용을 적어주세요 : ";
    getline(cin,text);
    
    json body;
    body["title"] = title;
    body["text"] = text;

    auto res = cli.Post(endpoint, headers, body.dump(), "application/json");

    if (res){
        try {
            json response = json::parse(res->body);
            if(response["status"] == "created"){
                cout << "포스트 작성 완료" << endl;
            }
            else{
                cout << "포스트 작성 실패: " << response["reason"] << endl;
            }
        } catch (const json::parse_error& e){
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else{
        cout << "서버 연결 오류 또는 응답 없음" << endl;
    }
    cout << endl;
}
void post_list(){

    string endpoint = "/posts";

    auto res = cli.Get(endpoint);

    if (res){
        try {
            json response = json::parse(res->body);
            if(response["status"] == "success"){
                auto posts = response["posts"];
                cout << "[포스트 목록 조회]" << endl;
                printf("%-10s | %-20s | %-10s | %-25s\n", "post_id", "title", "nickname", "created_at");
                cout << "--------------------------------------------------------------------------------" << endl;  
                for(int i=0; i<posts.size();i++){
                    printf("%-10d | %-20s | %-10s | %-25s\n",
                        posts[i]["post_id"].get<int>(),
                        posts[i]["title"].get<string>().c_str(),
                        posts[i]["nickname"].get<string>().c_str(),
                        posts[i]["created_at"].get<string>().c_str());
                }
            }
            else{
                cout << "조회 실패: " << response["reason"] << endl;
            }
        } catch (const json::parse_error& e){
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else{
        cout << "서버 연결 오류 또는 응답 없음" << endl;
    }
    cout << endl;
}
void choose_post(string post_id){
    user_input = post_id;

    string endpoint = "/posts";

    json body;
    body["post_id"] = user_input;

    auto res = cli.Post(endpoint,body.dump(),"application/json");

    if (res){
        try {
            json response = json::parse(res->body);
            if(response["status"] == "success"){
                auto posts = response["posts"];
                cout << "[포스트 목록 조회]" << endl;
                cout << "------------------------------------------" << endl;
                cout << "포스트 no : " << posts["post_id"] << endl;
                cout << "제목 : " << posts["title"] << endl;
                cout << "작성자 : " << posts["nickname"] << endl;
                cout << "작성일자 : " << posts["created_at"] << endl;
                cout << "내용 : " << posts["text"] << endl;
                cout << "------------------------------------------" << endl;
                comment_list(user_input);
            }
            else{
                cout << "조회 실패: " << response["reason"] << endl;
            }
        } catch (const json::parse_error& e){
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else{
        cout << "서버 연결 오류 또는 응답 없음" << endl;
    }
    cout << endl;
}
void comment_list(string post_id){
    string endpoint = "/posts/" + post_id + "/comments";
    current_post_id = post_id;
    auto res = cli.Get(endpoint);

    if (res){
        try {
            json response = json::parse(res->body);
            if(response["status"] == "success"){
                if (response["comments"].is_array()) {
                    auto comments = response["comments"];
                    cout << "[댓글 목록]" << endl; 
                    for(int i=0; i<comments.size();i++){
                        cout << "작성자: " << comments[i]["nickname"].get<string>() 
                            << " | "
                            << "작성일: " << comments[i]["created_at"].get<string>() << endl;
                        cout << comments[i]["text"].get<string>() << endl << endl;
                    }
                }
                else if (response["comments"].is_string()) {
                    cout << "[댓글 목록]\n"<<"댓글없음" << endl;
                } 
                else {
                    cout << "알 수 없는 댓글 형식입니다." << endl;
                }
            }
            else{
                cout << "조회 실패: " << response["reason"] << endl;
            }
        } catch (const json::parse_error& e){
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else{
        cout << "서버 연결 오류 또는 응답 없음" << endl;
    }
}
void write_comment(){
    string endpoint = "/posts/" + current_post_id + "/comments";
    string text;
    
    Headers headers = {
        {"Cookie", session_cookie}
    };

    cout << "댓글 내용을 입력해주세요 : ";
    getline(cin, text);

    json body;
    body["text"] = text;

    auto res = cli.Post(endpoint, headers, body.dump(),"application/json");

    if (res){
        try {
            json response = json::parse(res->body);
            if(response["status"] == "created"){
                cout  << "댓글이 입력되었습니다." << endl;
            }
            else{
                cout << "댓글 생성 실패: " << response["reason"] << endl;
            }
        } catch (const json::parse_error& e){
            cout << "[JSON 파싱 오류] " << e.what() << endl;
            cout << "[서버 응답 내용] " << res->body << endl;
        }
    }
    else{
        cout << "서버 연결 오류 또는 응답 없음" << endl;
    }
    cout << endl;
}

void search_user(){
    cout << "검색된 유저 목록 조회 완료" << endl << endl;
}
void search_post(){
    cout << "검색된 포스트 목록 조회 완료" << endl << endl;
}


int main(){
    login_menu();

    return 0;
}