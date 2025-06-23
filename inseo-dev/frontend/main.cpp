//#include "httplib.h"
//#include "json.hpp"
#include <iostream>

using namespace std;
//using namespace httplib;
//using namespace nlohmann;

void login();
void sign_up();
void login_menu();
void home_menu();     
void me_menu();        
void logout();
void check_me();
void update_me();
void delete_me();
void check_my_posts();
void check_my_comments();
void upload_post();
void post_list();
void choose_post();
void write_comment();
void comment_list();
void search_user();
void search_post();
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
        cout << "| 로그인 | 회원가입 |" << endl << endl;;
        cout << "이용할 서비스를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "로그인"){
            login();
            home_menu();
            if(user_input == "종료"){
                break;
            }
        }
        else if(user_input == "회원가입"){
            sign_up();
        }
    }   
}
void me_menu(){
    user_input = "";
    while(1){
        cout << "[내 정보 메뉴]" << endl;
        cout << "| 사용자 정보 조회 | 사용자 정보 변경 | 사용자 삭제 | 내가 쓴 포스트 보기 | 내가 쓴 댓글 보기 | 뒤로가기 |" << endl;
        cout << "이용할 서비스를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "사용자 정보 조회"){
            check_me();
        }
        else if(user_input == "사용자 정보 변경"){
            update_me();
        }
        else if(user_input == "사용자 삭제"){
            delete_me();
            break;
        }
        else if(user_input == "내가 쓴 포스트 보기"){
            check_my_posts();
        }
        else if(user_input == "내가 쓴 댓글 보기"){
            check_my_comments();
        }
        else if(user_input == "뒤로가기"){
            return;
        }
    }
}
void choosing_post_menu(){
    user_input = "";
        while(1){
            post_list();
            cout << "| 포스트 내용보기 | 뒤로가기 |" << endl;
            cout << "이용할 서비스를 입력하세요. : ";
            getline(cin, user_input);
            cout << endl;

            if(user_input == "포스트 내용보기"){
                choose_post();
                while(1){
                    cout << "| 댓글입력 | 뒤로가기|" << endl;
                    cout << "이용할 서비스를 입력하세요. : ";
                    getline(cin, user_input);
                    cout << endl;
                    if(user_input == "댓글입력"){
                        write_comment();
                    }
                    else if(user_input == "뒤로가기"){
                        return;
                    }
                }
            }
            if(user_input == "뒤로가기")
                return;
        }

}
void posting_menu(){
    user_input="";
    while(1){
        cout << "[포스팅 메뉴]" << endl;
        cout << "| 포스트 작성 | 포스트 목록 조회 | 뒤로가기 |" << endl;
        cout << "이용할 서비스를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "포스트 작성"){
            upload_post();
        }
        else if(user_input == "포스트 목록 조회"){
            choosing_post_menu();
        }
        else if(user_input == "뒤로가기"){
            return;
        }
    }
}
void search_menu(){
    user_input="";
    while(1){
        cout << "[검색 메뉴]" << endl;
        cout << "| 다른 사용자 검색 | 포스트 검색 | 뒤로가기 |" << endl;
        cout << "이용할 서비스를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "다른 사용자 검색"){
            search_user();
        }
        else if(user_input == "포스트 검색"){
            search_post();
        }
        else if(user_input == "뒤로가기"){
            return;
        }
    }
}
void home_menu(){
    user_input="";
    while(1){
        cout << "[메인메뉴]" << endl;
        cout << "| 내 정보 | 포스팅 | 탐색 | 로그아웃 | 종료 |" << endl;
        cout << "이용할 서비스를 입력하세요. : ";
        getline(cin, user_input);
        cout << endl;

        if(user_input == "내 정보"){
            me_menu();
            if(user_input == "사용자 삭제"){
                break;
            }
        }
        else if(user_input == "포스팅"){
            posting_menu();
        }
        else if(user_input == "탐색"){
            search_menu();
        }
        else if(user_input == "로그아웃"){
            logout();
            break;
        }
        else if(user_input == "종료"){
            cout << "프로그램을 종료합니다." << endl;
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