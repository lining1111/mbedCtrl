//
// Created by root on 2021/3/30.
//

#ifndef MY_RPI_DOOR_COMMON_JSON_H
#define MY_RPI_DOOR_COMMON_JSON_H

#include <string>
#include <map>

#ifdef arm
#include <json/json.h>
#endif

#ifdef arm64

#include <json/json.h>

#endif

#ifdef x86

#include <jsoncpp/json/json.h>

#endif

using namespace std;


namespace common_json {
#define Cmd_Code(code) Cmd_Code_##code
    enum Cmd_Code {
        Cmd_Code_FEEDBACK = 99,
        Cmd_Code_HEARTBEAT = 0,

        Cmd_Code_GATE_RICH = 1,
        Cmd_Code_GATE = 2,
        Cmd_Code_SCREEN_AD = 3,
        Cmd_Code_SCREEN_COLOR = 4,
        Cmd_Code_CAMERA = 5,
        Cmd_Code_SCREEN_TIME = 95,

        Cmd_Code_LOGIN_REQ = 10,
        Cmd_Code_GATE_INFO_REQ = 11,
        Cmd_Code_PASS_IN_REQ = 12,
        Cmd_Code_PASS_OUT_REQ = 13,
        Cmd_Code_MATCH_REQ = 14,
        Cmd_Code_CHARGE_AMOUNT_REQ = 15,
        Cmd_Code_CHARGE_RECORD_REQ = 16,
        Cmd_Code_CHANGE_CONTENT_REQ = 17,
        Cmd_Code_CHANGE_PASSWORD_REQ = 18,

        Cmd_Code_LOGIN_BCK = 19,
        Cmd_Code_LOGIN_SUCCESS_BCK = 20,
        Cmd_Code_GATE_INFO_BCK = 21,
        Cmd_Code_PASS_IN_BCK = 22,
        Cmd_Code_PASS_OUT_BCK = 23,
        Cmd_Code_MATCH_BCK = 24,
        Cmd_Code_CHARGE_RECORD_BCK = 25,
        Cmd_Code_CHANGE_CONTENT_BCK = 26,
        Cmd_Code_CHARGE_AMOUNT_BCK = 27,
        Cmd_Code_CHANGE_PASSWORD_BCK = 28,
        Cmd_Code_CHANGE_PASSWORD_SUCCESS_BCK = 29,
        Cmd_Code_LOGIN_COVER_BCK = 30,
        Cmd_Code_PASS_OUT_CLEAR = 31,

        Cmd_Code_PAY_ONLINE_CHECK_REQ = 100,
        Cmd_Code_PAY_ONLINE_RESULT_BAK = 101,
        Cmd_Code_PAY_ONLINE_END_REQ = 102,
        Cmd_Code_PAY_ONLINE_RECORD_REQ = 103,
        Cmd_Code_PAY_ONLINE_NOPLATE_REQ = 104,
        Cmd_Code_PAY_ONLINE_COMMUNITY_REQ = 106,
        Cmd_Code_PAY_ONLINE_COMMUNITY_BCK = 107,

        Cmd_Code_PREPAY_ONLINE_CHECK_REQ = 110,
        Cmd_Code_PREPAY_ONLINE_RESULT_BAK = 111,
        Cmd_Code_PREPAY_ONLINE_END_REQ = 112,

        Cmd_Code_PAY_ONLINE_PASSIN_BCK = 120,
        Cmd_Code_PAY_ONLINE_PASSOUT_BCK = 121,
        Cmd_Code_PAY_ONLINE_AUTO_END_REQ = 122,

        Cmd_Code_REMOTE_FIND_CAR_BCK = 1001,
        Cmd_Code_REMOTE_ALL_CAMERA_BCK = 1002,
        Cmd_Code_REMOTE_LOGIN_SUCCESS_BCK = 1003,
        Cmd_Code_REMOTE_LOGIN_FAILED_BCK = 1004,

        Cmd_Code_REMOTE_OPEN_GATE = 1103,
        Cmd_Code_REMOTE_GET_ALL_CAMERA_REQ = 1104,
        Cmd_Code_REMOTE_LOGIN_REQ = 1105,
    };

#define common(str) common_##str
    const string common_code = "code";
    const string common_msgid = "msgid";
    const string common_data = "data";

    const string common_returnback = "returnback";
    const string common_ip = "ip";
    const string default_ip = "127.0.0.1";
    const string common_user_name = "user_name";
    const string common_user_passwd = "passwd";
    const string common_gate_name = "gate_name";
    const string common_gate_id = "gate_id";
    const string common_gate_ids_str = "gate_ids_str";
    const string common_garage_id = "garage_id";
    const string common_way = "way";
    const string common_license = "license";
    const string common_time = "time";
    const string common_sn = "sn";
    const string common_devip = "devip";
    const string common_province = "province";
    const string common_city = "city";
    const string common_number = "number";
    const string common_picture = "picture";

    const string common_gate_number = "gate_number";
    const string common_exception_price = "exception_price";
    const string common_gate_array = "gate_array";
    const string common_camera_addr = "camera_addr";
    const string common_in_out_type = "in_out_type";

    const string common_car_num = "car_num";
    const string common_in_time = "in_time";
    const string common_car_picture = "car_picture";
    const string common_space_id = "space_id";
    const string common_car_array = "car_array";
    const string common_result = "result";
    const string common_reason_type = "reason_type";
    const string common_reason_id = "reason_id";
    const string common_reason_name = "reason_name";
    const string common_reason = "reason";
    const string common_error = "error";
    const string common_camera_ip = "camera_ip";
    const string common_open_cause = "open_cause";
    const string common_visiting_address = "visiting_address";
    const string common_visitor_name = "visitor_name";
    const string common_visitor_phone = "visitor_phone";
    const string common_bussiness_id = "bussiness_id";
    const string common_res_garage_id = "res_garage_id";
    const string common_is_empty_open = "is_empty_open";
    const string common_visitor_time = "visitor_time";
    const string common_car_type_id = "car_type_id";
    const string common_led_car_type = "led_car_type";
    const string common_use_res_num = "use_res_num";
    const string common_car_owner_name = "car_owner_name";
    const string common_car_checkin_str = "checkin_str";
    const string common_car_statu = "car_statu";
    const string common_car_type = "car_type";
    const string common_car_owner_phone = "car_owner_phone";
    const string common_checkin_str = "checkin_str";
    const string common_preferential_reason = "preferential_reason";
    const string common_username = "username";
    const string common_amount_due = "amount_due";
    const string common_amount_paid = "amount_paid";
    const string common_amount_to_paid = "amount_to_paid";
    const string common_preferential_amount = "preferential_amount";
    const string common_parking_id = "parking_id";
    const string common_show = "show";
    const string common_voice = "voice";
    const string common_target_garage_id = "target_garage_id";
    const string common_note = "note";
    const string common_stay_time = "stay_time";
    const string common_abnormal_amount = "abnormal_amount";
    const string common_start_number = "start_number";
    const string common_lines = "lines";
    const string common_total_amount = "total_amount";
    const string common_total_discount = "total_discount";
    const string common_total_gate_charge = "total_gate_charge";
    const string common_total_number = "total_number";
    const string common_total_selfpay = "total_selfpay";
    const string common_record_array = "record_array";
    const string common_selfpay = "selfpay";
    const string common_start_line = "start_line";
    const string common_garage_name = "garage_name";
    const string common_out_time = "out_time";
    const string common_parking_fee = "parking_fee";
    const string common_old_passwd = "old_passwd";
    const string common_new_passwd = "new_passwd";
    const string common_body = "body";
    const string common_request_time = "request_time";
    const string common_plat_nu = "plat_nu";
    const string common_charge_nu = "charge_nu";
    const string common_open_gate = "open_gate";
    const string common_order_id = "order_id";
    const string common_order_no = "order_no";
    const string common_pay_time = "pay_time";
    const string common_in_park_time = "in_park_time";
    const string common_amount_to_pay = "amount_to_pay";
    const string common_free_reason = "free_reason";
    const string common_lock = "lock_queue_recv";
    const string common_gate = "gate";
    const string common_voice_id = "voice_id";
    const string common_voice_wparam = "voice_wparam";
    const string common_voice_lparam = "voice_lparam";
    const string common_ad = "ad";
    const string common_line = "line";
    const string common_comm_id = "comm_id";
    const string common_plate_nu = "plate_nu";
    const string common_free_secs = "free_secs";
    const string common_charge_secs = "charge_secs";
    const string common_camera_array_len = "camera_array_len";
    const string common_camera_array = "camera_array";
    const string common_is_on_watch = "is_on_watch";
    const string common_camera_sn = "camera_sn";
    const string common_display_name = "display_name";
    const string common_password = "password";

    struct Feedback {
        int code = Cmd_Code_FEEDBACK;
        int msgid = 0;
        int returnback = 0;
    };

    struct HeartBeat {
        int code = Cmd_Code_HEARTBEAT;
        int msgid = 0;
        string ip;
    };

    struct CameraInfo {
        int code = Cmd_Code_CAMERA;
        int msgid = 0;
        string license;
        long captime = 0;
        string sn;
        string devip;
        string province;
        string city;
        string number;
        string picture;
    };

    //gate_rich
    struct GateRich {
        int code = Cmd_Code_GATE_RICH;
        int msgid = 0;
        string sn;
        string car_num;
        string show;
        string voice;
        bool lock = false;//on off
        bool gate = false;//on off

        int voice_id = 0;
        double voice_wparam = 0.0;
        double voice_lparam = 0.0;
    };

    typedef enum {
        VoiceId_0 = 0, //欢迎光临
        VoiceId_1 = 1, //欢迎回家
        VoiceId_2 = 2, //一路平安
        VoiceId_3 = 3, //车已进场
        VoiceId_4 = 4, //剩余X天X小时X分
        VoiceId_5 = 5, //剩余X次
        VoiceId_6 = 6, //余额X元
        VoiceId_7 = 7, //未生效车, 即将收费
        VoiceId_8 = 8, //车辆过期, 即将收费
        VoiceId_9 = 9, //车位过期, 即将收费
        VoiceId_10 = 10, //车位有车, 即将收费
        VoiceId_11 = 11, //此车已过期 请缴费
        VoiceId_12 = 12, //请等待人工确认
        VoiceId_13 = 13, //未生效车
        VoiceId_14 = 14, //车辆过期
        VoiceId_15 = 15, //车位过期
        VoiceId_16 = 16, //车位有车
        VoiceId_17 = 17, //此车无效
        VoiceId_18 = 18, //此车已过期
        VoiceId_19 = 19, //车位已满
        VoiceId_20 = 20, //禁止通行
        VoiceId_21 = 21, //X元
        VoiceId_22 = 22, //缴费车
        VoiceId_23 = 23, //此车无权限
        VoiceId_24 = 24, //计费异常
        VoiceId_25 = 25, //车已出场
    } VoiceId;


    //gate
    struct Gate {
        int code = Cmd_Code_GATE;
        int msgid = 0;
        string sn;
        bool gate = false;
    };
    //screen_color
    struct ScreenColor {
        int code = Cmd_Code_SCREEN_COLOR;
        int msgid = 0;
        string sn;
    };
    //json_screen_ad
    struct ScreenAd {
        int code = Cmd_Code_SCREEN_AD;
        int msgid = 0;
        string sn;
        string ad;
        int line = 2;
    };
    //json_screen_time
    struct ScreenTime {
        int code = Cmd_Code_SCREEN_TIME;
        int msgid = 0;
        string sn;
    };


    //login
    struct LogIn_Req {
        int code = Cmd_Code_LOGIN_REQ;
        int msgid = 0;
        string gate_name;
        string gate_id;
        string garage_id;
        string user_name;
        string user_passwd;
        string gate_ids_str;
        int way = 0;////1普通登录，2快速换班
    };
    struct OpenReason {
        string reason_id;
        string reason_name;
        int reason_type;
    };
    struct LogInSuccess_Back {
        int code = Cmd_Code_LOGIN_SUCCESS_BCK;
        int msgid = 0;
        string result;
        string garage_id;
        string gate_id;
        vector<OpenReason> reason;
    };
    struct LoginFailed_Back {
        int code = Cmd_Code_LOGIN_BCK;
        int msgid = 0;
        string result;
        string error;
    };
    struct LoginCover_Back {
        int code = Cmd_Code_LOGIN_COVER_BCK;
        int msgid = 0;
        string garage_id;
        string gate_name;
        string user_name;
    };

    //gateinfo
    struct GateInfo_Req {
        int code = Cmd_Code_GATE_INFO_REQ;
        int msgid = 0;
    };
    struct GateItem {
        string gate_id;
        string gate_name;
        string garage_id;
        int is_on_watch;
        int in_out_type;
        string camera_addr;
    };
    struct GateInfo_Back {
        int code = Cmd_Code_GATE_INFO_BCK;
        int msgid = 0;
        int gate_number = 0;
        vector<GateItem> gate_array;
        double exception_price = 0.0;
    };

    //match
    struct Match_Req {
        int code = Cmd_Code_MATCH_REQ;
        int msgid = 0;
        string time;
        string car_num;
        string gate_id;
        string picture;
    };
    struct CarMatchItem {
        string car_num;
        string in_time;
        string car_picture;
    };
    struct Match_Back {
        int code = Cmd_Code_MATCH_BCK;
        int msgid = 0;
        int number = 0;
        string car_num;
        string car_picture;
        string space_id;
        double exception_price = 0.0;
        vector<CarMatchItem> car_array;
    };

    //pass_in
    struct PassIn_Req {
        int code = Cmd_Code_PASS_IN_REQ;
        int msgid = 0;
        string car_num;
        string camera_ip;
        string gate_id;
        string open_cause;
        string visitor_name;
        string visitor_phone;
        string visiting_address;
        string space_id;
        string checkin_str;
        int led_car_type;
        int car_type_id;
        long visitor_time = 0;
        string bussiness_id;
        string res_garage_id;
        int use_res_num;
        string car_owner_name;
        string car_owner_phone;
        bool is_empty_open;
        string picture;
    };
    struct PassIn_Back {
        int code = Cmd_Code_PASS_IN_BCK;
        int msgid = 0;
        string car_num;
        string camera_ip;
        string gate_id;
        string car_type;
        string car_statu;
        string space_id;
        string picture;
        string checkin_str;
        int led_car_type;
        int car_type_id;
        long visitor_time = 0;
        string bussiness_id;
        string res_garage_id;
        int use_res_num;
        string car_owner_name;
        string car_owner_phone;
        vector<OpenReason> reason;
    };

    struct PassOut_Req {
        int code = Cmd_Code_PASS_OUT_REQ;
        int msgid = 0;
        string car_num;
        string camera_ip;
        string gate_id;
        string preferential_reason;
        double preferential_amount = 0.0;
        double amount_due = 0.0;//total money
        double amount_paid = 0.0;//has paid
        double amount_to_paid = 0.0;//need to pay
        string username;
        string parking_id;
        string car_picture;
        string show;
        string voice;
        bool is_empty_open;
        int car_type_id;
        long visitor_time = 0;
        string bussiness_id;
        string res_garage_id;
        string target_garage_id;
        string space_id;
        int use_res_num;
        string car_owner_name;
        string car_owner_phone;
    };
    struct PassOut_Back {
        int code = Cmd_Code_PASS_OUT_BCK;
        int msgid = 0;
        string car_num;
        string camera_ip;
        string gate_id;
        string car_type;
        string car_statu;
        string in_time;
        string out_time;
        string stay_time;
        double amount_due = 0.0;
        double amount_paid = 0.0;
        double amount_to_paid = 0.0;
        double abnormal_amount = 0.0;
        int paid_way;
        double exception_price = 0.0;
        string note;
        string car_picture;
        string parking_id;
        string show;
        string voice;
        int car_type_id;
        long visitor_time = 0;
        string bussiness_id;
        string res_garage_id;
        string target_garage_id;
        string space_id;
        int use_res_num;
        string car_owner_name;
        string car_owner_phone;
        vector<OpenReason> reason;
    };

    //Charge_Amount
    struct ChargeAmount_Req {
        int code = Cmd_Code_CHARGE_AMOUNT_REQ;
        int msgid = 0;
        string user_name;
        string gate_id;
        string gate_ids_str;
    };
    struct ChargeAmount_Back {
        int code = Cmd_Code_CHARGE_AMOUNT_BCK;
        int msgid = 0;
        double total_amount = 0.0;
    };

    //Charge_Record
    struct ChargeRecord_Req {
        int code = Cmd_Code_CHARGE_RECORD_REQ;
        int msgid = 0;
        string user_name;
        string gate_id;
        string gate_ids_str;
        int start_number = 0;
        int lines = 0;//request item count
    };
    struct RecordItem {
        string car_num;
        double amount_due = 0.0;
        double preferential_amount = 0.0;
        string preferential_reason;
        double amount_paid = 0.0;
        double amount_to_paid = 0.0;
        double self_paid = 0.0;
    };
    struct ChargeRecord_Back {
        int code = Cmd_Code_CHARGE_RECORD_BCK;
        int msgid = 0;
        int total_number = 0;
        int number = 0;
        double total_amount = 0.0;
        double total_discount = 0.0;
        double total_selfpay = 0.0;
        double total_gate_charge = 0.0;
        vector<RecordItem> record_array;
    };

    //charge_content
    struct ChargeContent_Req {
        int code = Cmd_Code_CHANGE_CONTENT_REQ;
        int msgid = 0;
        string car_num;
        string garage_id;
        string gate_id;
        string gate_name;
        unsigned int start_line = 0;
        unsigned int lines = 0;
    };
    struct ContentItem {
        string garage_name;
        string gate_name;
        string in_time;
        string out_time;
        string stay_time;
        double parking_fee = 0.0;
    };
    struct ChargeContent_Back {
        int code = Cmd_Code_CHANGE_CONTENT_BCK;
        int msgid = 0;
        int number = 0;
        int total_number = 0;
        vector<ContentItem> car_array;
    };

    //change_password
    struct ChangePassword_Req {
        int code = Cmd_Code_CHANGE_PASSWORD_REQ;
        int msgid = 0;
        string user_name;
        string old_passwd;
        string new_passwd;
    };
    struct ChangePassword_Back {
        int code = Cmd_Code_CHANGE_PASSWORD_BCK;
        int msgid = 0;
        string result;
        string error;
    };

    //pay_online
    struct PayOnline_Req {
        int code = Cmd_Code_PAY_ONLINE_CHECK_REQ;
        int msgid = 0;
        string plat_nu;
        string gate_id;
        string order_id;
        string order_no;
        long request_time = 0;
        long pay_time = 0;//支付时间
        bool open_gate = false;
        double charge_nu = 0.0;//支付金额
    };
    struct PayOnline_Back {
        int code;
        int msgid = 0;
        string plat_nu;
        string gate_id;
        string order_id;
        string order_no;
        long request_time = 0;
        long start_time = 0;
        long secs = 0;
        double charge_nu = 0.0;

        double amount_to_pay = 0.0;
        double amount_paid = 0.0;
        int free_reason = 0;
    };

    //pay_online_community_req
    struct PayOnlineCommunity_Req {
        int code = Cmd_Code_PAY_ONLINE_COMMUNITY_REQ;
    };
    //pay_online_community_bak
    struct PayOnlineCommunity_Back {
        int code = Cmd_Code_PAY_ONLINE_COMMUNITY_BCK;
        string comm_id;
    };

    //json_pay_online_passin_bak
    struct PayOnlinePassIn_Back {
        int code = Cmd_Code_PAY_ONLINE_PASSIN_BCK;
        int msgid = 0;
        string comm_id;
        string plate_nu;
        string order_id;
        long in_time = 0;
        long request_time = 0;
        long free_secs = 0;
    };
    //json_pay_online_passout_bak
    struct PayOnlinePassOut_Back {
        int code = Cmd_Code_PAY_ONLINE_PASSOUT_BCK;
        int msgid = 0;
        string comm_id;
        string plate_nu;
        string order_id;
        long out_time = 0;
        long request_time = 0;
        long charge_secs = 0;
        double charge_nu = 0.0;
        string gate_id;
    };

    //pay_online_auto
    struct PayOnlineAutoEnd_Req {
        int code = Cmd_Code_PAY_ONLINE_AUTO_END_REQ;
        int msgid = 0;
        string comm_id;
        string plate_nu;
        string order_id;
        long request_time = 0;
        double charge_nu = 0.0;
        string gate_id;
    };
    //json_remote_login_req
    struct RemoteLogin_Req {
        int code = Cmd_Code_REMOTE_LOGIN_REQ;
        int msgid = 0;
        string username;
        string password;
    };
    //json_simple_bck
    struct Simple_Back {
        int code;
        int msgid = 0;
    };
    struct RemoteAllCamera_Req {
        int code = Cmd_Code_REMOTE_GET_ALL_CAMERA_REQ;
    };

    //json_remote_all_camera_bck
    struct RemoteAllCamera_Back {
        int code = Cmd_Code_REMOTE_ALL_CAMERA_BCK;
        int msgid = 0;
        vector<GateItem> camera_array;
        int camera_array_len = 0;
    };
    //json_remote_gate_reqbck
    struct RemoteGate_ReqBack {
        int code;
        int msgid = 0;
        string car_num;
        string show;
        string car_type;
        string display_name;

        string camera_ip;
        string parking_id;
        string gate_id;
        string bussiness_id;
        string res_garage_id;
        string target_garage_id;
        string space_id;
        long visitor_time = 0;
        int use_res_num;
        int car_type_id;

        string camera_sn;
        string car_picture;
        string voice;
        string car_owner_name;
        string car_owner_phone;
    };


    //---base

    //feedback
    int GetInfo_Feedback(Feedback &feedback, string json_str);

    int SetInfo_Feedback(string &json_str, Feedback feedback);

    //heartbeat
    int GetInfo_HeartBeat(HeartBeat &heartBeat, string json_str);

    int SetInfo_HeartBeat(string &json_str, HeartBeat heartBeat);

    //---device

    //camera
    int GetInfo_Camera(CameraInfo &cameraInfo, string json_str);

    int SetInfo_Camera(string &json_str, CameraInfo cameraInfo);

    //gate_rich
    int GetInfo_GateRich(GateRich &gateRich, string json_str);

    int SetInfo_GateRich(string &json_str, GateRich gateRich);

    //gate
    int GetInfo_Gate(Gate &gate, string json_str);

    int SetInfo_Gate(string &json_str, Gate gate);

    //screen_color
    int GetInfo_ScreenColor(ScreenColor &screenColor, string json_str);

    int SetInfo_ScreenColor(string &json_str, ScreenColor screenColor);

    //screen_ad
    int GetInfo_ScreenAd(ScreenAd &screenAd, string json_str);

    int SetInfo_ScreenAd(string &json_str, ScreenAd screenAd);

    //screen_time
    int GetInfo_ScreenTime(ScreenTime &screenTime, string json_str);

    int SetInfo_ScreenTime(string &json_str, ScreenTime screenTime);

    //---front

    //login_req
    int GetInfo_LogIn_Req(LogIn_Req &logInReq, string json_str);

    int SetInfo_LogIn_Req(string &json_str, LogIn_Req logInReq);

    //login_success_back
    int GetInfo_LogInSuccess_Back(LogInSuccess_Back &logInSuccessBack, string json_str);

    int SetInfo_LogInSuccess_Back(string &json_str, LogInSuccess_Back logInSuccessBack);

    //login_failed_back
    int GetInfo_LogInFailed_Back(LoginFailed_Back &loginFailedBack, string json_str);

    int SetInfo_LogInFailed_Back(string &json_str, LoginFailed_Back loginFailedBack);

    //login_cover_back
    int GetInfo_LogInCover_Back(LoginCover_Back &loginCoverBack, string json_str);

    int SetInfo_LogInCover_Back(string &json_str, LoginCover_Back loginCoverBack);

    //gate_info_req
    int GetInfo_GateInfo_Req(GateInfo_Req &gateInfoReq, string json_str);

    int SetInfo_GateInfo_Req(string &json_str, GateInfo_Req gateInfoReq);

    //gate_info_back
    int GetInfo_GateInfo_Back(GateInfo_Back &gateInfoBack, string json_str);

    int SetInfo_GateInfo_Back(string &json_str, GateInfo_Back gateInfoBack);

    //match_req
    int GetInfo_Match_Req(Match_Req &matchReq, string json_str);

    int SetInfo_Match_Req(string &json_str, Match_Req matchReq);

    //match_back
    int GetInfo_Match_Back(Match_Back &matchBack, string json_str);

    int SetInfo_Match_Back(string &json_str, Match_Back matchBack);

    //pass_in_req
    int GetInfo_PassIn_Req(PassIn_Req &passInReq, string json_str);

    int SetInfo_PassIn_Req(string &json_str, PassIn_Req passInReq);

    //pass_in_back
    int GetInfo_PassIn_Back(PassIn_Back &passInBack, string json_str);

    int SetInfo_PassIn_Back(string &json_str, PassIn_Back passInBack);

    //pass_out_req
    int GetInfo_PassOut_Req(PassOut_Req &passOutReq, string json_str);

    int SetInfo_PassOut_Req(string &json_str, PassOut_Req passOutReq);

    //pass_out_back
    int GetInfo_PassOut_Back(PassOut_Back &passOutBack, string json_str);

    int SetInfo_PassOut_Back(string &json_str, PassOut_Back passOutBack);

    //Charge_Amount_req
    int GetInfo_ChargeAmount_Req(ChargeAmount_Req &chargeAmountReq, string json_str);

    int SetInfo_ChargeAmount_Req(string &json_str, ChargeAmount_Req chargeAmountReq);

    //Charge_Amount_back
    int GetInfo_ChargeAmount_Back(ChargeAmount_Back &chargeAmountBack, string json_str);

    int SetInfo_ChargeAmount_Back(string &json_str, ChargeAmount_Back chargeAmountBack);

    //Charge_Record_req
    int GetInfo_ChargeRecord_Req(ChargeRecord_Req &chargeRecordReq, string json_str);

    int SetInfo_ChargeRecord_Req(string &json_str, ChargeRecord_Req chargeRecordReq);

    //Charge_Record_back
    int GetInfo_ChargeRecord_Back(ChargeRecord_Back &chargeRecordBack, string json_str);

    int SetInfo_ChargeRecord_Back(string &json_str, ChargeRecord_Back chargeRecordBack);

    //Charge_Content_req
    int GetInfo_ChargeContent_Req(ChargeContent_Req &chargeContentReq, string json_str);

    int SetInfo_ChargeContent_Req(string &json_str, ChargeContent_Req chargeContentReq);

    //Charge_Content_back
    int GetInfo_ChargeContent_Back(ChargeContent_Back &chargeContentBack, string json_str);

    int SetInfo_ChargeContent_Back(string &json_str, ChargeContent_Back chargeContentBack);

    //Change_Password_req
    int GetInfo_ChangePassword_Req(ChangePassword_Req &changePasswordReq, string json_str);

    int SetInfo_ChangePassword_Req(string &json_str, ChangePassword_Req changePasswordReq);

    //Change_Password_back
    int GetInfo_ChangePassword_Back(ChangePassword_Back &changePasswordBack, string json_str);

    int SetInfo_ChangePassword_Back(string &json_str, ChangePassword_Back changePasswordBack);

    //---pay

    //pay_online_req
    int GetInfo_PayOnline_Req(PayOnline_Req &payOnlineReq, string json_str);

    int SetInfo_PayOnline_Req(string &json_str, PayOnline_Req payOnlineReq);

    //pay_online_back
    int GetInfo_PayOnline_Back(PayOnline_Back &payOnlineBack, string json_str);

    int SetInfo_PayOnline_Back(string &json_str, PayOnline_Back payOnlineBack);

    //pay_online_community_req
    int GetInfo_PayOnlineCommunity_Req(PayOnlineCommunity_Req &payOnlineCommunityReq, string json_str);

    int SetInfo_PayOnlineCommunity_Req(string &json_str, PayOnlineCommunity_Req payOnlineCommunityReq);

    //pay_online_community_back
    int GetInfo_PayOnlineCommunity_Back(PayOnlineCommunity_Back &payOnlineCommunityBack, string json_str);

    int SetInfo_PayOnlineCommunity_Back(string &json_str, PayOnlineCommunity_Back payOnlineCommunityBack);

    //pay_online_passin_back
    int GetInfo_PayOnlinePassIn_Back(PayOnlinePassIn_Back &payOnlinePassInBack, string json_str);

    int SetInfo_PayOnlinePassIn_Back(string &json_str, PayOnlinePassIn_Back payOnlinePassInBack);

    //pay_online_passout_back
    int GetInfo_PayOnlinePassOut_Back(PayOnlinePassOut_Back &payOnlinePassOutBack, string json_str);

    int SetInfo_PayOnlinePassOut_Back(string &json_str, PayOnlinePassOut_Back payOnlinePassOutBack);

    //pay_online_auto_end
    int GetInfo_PayOnlineAutoEnd(PayOnlineAutoEnd_Req &payOnlineAutoReq, string json_str);

    int SetInfo_PayOnlineAutoEnd(string &json_str, PayOnlineAutoEnd_Req payOnlineAutoReq);

    //---remote

    //remote_login_req
    int GetInfo_RemoteLogIn_Req(RemoteLogin_Req &remoteLoginReq, string json_str);

    int SetInfo_RemoteLogIn_Req(string &json_str, RemoteLogin_Req remoteLoginReq);

    //simple_back
    int GetInfo_Simple_Back(Simple_Back &simpleBack, string json_str);

    int SetInfo_Simple_Back(string &json_str, Simple_Back simpleBack);

    //remote_all_camera_req
    int GetInfo_RemoteAllCamera_Req(RemoteAllCamera_Req &remoteAllCameraReq, string json_str);

    int SetInfo_RemoteAllCamera_Req(string &json_str, RemoteAllCamera_Req remoteAllCameraReq);

    //remote_all_camera_back
    int GetInfo_RemoteAllCamera_Back(RemoteAllCamera_Back &remoteAllCameraBack, string json_str);

    int SetInfo_RemoteAllCamera_Back(string &json_str, RemoteAllCamera_Back remoteAllCameraBack);

    //remote_gate_req
    int GetInfo_RemoteGate_Req(RemoteGate_ReqBack &remoteGateReqBack, string json_str);

    int SetInfo_RemoteGate_Req(string &json_str, RemoteGate_ReqBack remoteGateReqBack);

    //remote_gate_back
    int GetInfo_RemoteGate_Back(RemoteGate_ReqBack &remoteGateReqBack, string json_str);

    int SetInfo_RemoteGate_Back(string &json_str, RemoteGate_ReqBack remoteGateReqBack);

}
#endif //MY_RPI_DOOR_COMMON_JSON_H
