//
// Created by root on 2021/3/30.
//
#include <iostream>
#include "common/common_json.h"

using namespace common_json;

int main() {
    string json_str;
    Feedback feedback;
    feedback.returnback = 0;

    SetInfo_Feedback(json_str, feedback);

    GetInfo_Feedback(feedback, json_str);

    PassIn_Back passInBack;

    OpenReason item1;
    OpenReason item2;
    item1.reason_type = 1;
    item1.reason_id = "item1";
    item1.reason_name = "item1_name";
    item2.reason_type = 2;
    item2.reason_id = "item2";
    item2.reason_name = "item2_name";

    passInBack.reason.push_back(item1);
    passInBack.reason.push_back(item2);
    SetInfo_PassIn_Back(json_str, passInBack);
    cout << json_str << endl;


    //login_success_back
    LogInSuccess_Back logInSuccessBack;
    logInSuccessBack.result = "ok";
    for (int i = 0; i < 2; i++) {
        OpenReason item;
        item.reason_id = to_string(i);
        item.reason_name = "name"+to_string(i);
        item.reason_type = i;
        logInSuccessBack.reason.push_back(item);
    }
    SetInfo_LogInSuccess_Back(json_str, logInSuccessBack);
    cout<<json_str<<endl;
    LogInSuccess_Back logInSuccessBack1;
    GetInfo_LogInSuccess_Back(logInSuccessBack1, json_str);




    PayOnline_Back payOnlineBack;
    payOnlineBack.request_time = 1619227233;
    SetInfo_PayOnline_Back(json_str, payOnlineBack);
    cout << json_str << endl;

    PayOnline_Req payOnlineReq;
    GetInfo_PayOnline_Req(payOnlineReq, json_str);


    return 0;
}