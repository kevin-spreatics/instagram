//#include "httplib.h"
#include "json.hpp"
#include <iostream>


using namespace std;
//using namespace httplib;
//using namespace nlohmann;

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
void choose_post(); // 특정 포스트 선택
void write_comment();   // 댓글 쓰기
void comment_list();    // 해당 포스트 댓글 목록 조회
void search_user(); // 특정 유저 검색
void search_post(); // 특정 포스트 검색
/*
void search_user(string search=""){
    Client cli("127.0.0.1:5000");
    string endpoint = "/users/search";

    json body;
    body["search"] = search;

    auto res = cli.Post(endpoint,body.dump(),"application/json");

    if (res && res->status == 200){
        //cout << res->body << endl;
        json response = json::parse(res->body);
        cout << "status" << response["status"] << endl;
        cout << "user" << response["user"] << endl;
        //cout << res->body << endl;
    }
    else
        cout << res.error() << endl;
}

void delete_me(int user_id){
    Client cli("127.0.0.1:5000");

    string endpoint = "/users/" + to_string(user_id);

    auto res = cli.Delete(endpoint);

    if (res && res->status == 200){
        cout << res->body << endl;
    }
    else
        cout << res.error() << endl;
}

int main() {
    //delete_me(7);
    search_user();
    return 0;
}
    */
string user_input;
bool end_input = false;

void login(){
    cout << "로그인 완료" << endl << endl;
}
void sign_up(){
    cout <<"회원가입 완료" << endl << endl;
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
    user_input = "";
        while(1){
            post_list();
            cout << "| 1. 포스트 내용보기 | 2. 뒤로가기 |" << endl;
            cout << "이용할 서비스 번호를 입력하세요. : ";
            getline(cin, user_input);
            cout << endl;

            if(user_input == "1"){
                choose_post();
                while(1){
                    cout << "| 1. 댓글입력 | 2. 뒤로가기|" << endl;
                    cout << "이용할 서비스 번호를 입력하세요. : ";
                    getline(cin, user_input);
                    cout << endl;
                    if(user_input == "1"){
                        write_comment();
                    }
                    else if(user_input == "2"){
                        return;
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
}
void check_me(){
    cout << "[사용자 정보 조회]" << endl;
    cout << "이름 : 홍길동" << endl;
    cout << "나이 : 30세" << endl << endl;
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
    cout << "[포스트 작성]" << endl;
    cout << "포스트 작성 완료" << endl << endl;
}
void post_list(){
    cout << "[포스트 목록 조회]" << endl;
    cout << "포스트1\n포스트2\n포스트3" << endl << endl;
}
void choose_post(){
    char yn = 'n';
    cout << "내용을 볼 포스트를 입력하세요 : ";
    getline(cin, user_input);
    cout <<user_input << "의 내용" << endl;
    comment_list();
}
void comment_list(){
    cout << "댓글 목록들 조회 완료" << endl << endl;
}
void write_comment(){
    cout << "댓글 입력완료" << endl << endl;
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