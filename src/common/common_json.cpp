//
// Created by root on 2021/3/30.
//
#include <iostream>

#include "common/common_json.h"

using namespace std;

namespace common_json {

    int GetInfo_Feedback(Feedback &feedback, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        feedback.msgid = root[common(msgid)].asInt();


        //data returnback
        feedback.returnback = root[common(data)][common(returnback)].asInt();
        return 0;
    }

    int SetInfo_Feedback(string &json_str, Feedback feedback) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = feedback.code;
        //root msgid
        root[common(msgid)] = feedback.msgid;

        //data returnback
        root[common(data)][common(returnback)] = feedback.returnback;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_HeartBeat(HeartBeat &heartBeat, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        heartBeat.msgid = root[common(msgid)].asInt();

        //data ip
        heartBeat.ip = root[common(data)][common(ip)].asString();

        return 0;
    }

    int SetInfo_HeartBeat(string &json_str, HeartBeat heartBeat) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = heartBeat.code;
        //root msgid
        root[common(msgid)] = heartBeat.msgid;

        //data ip 127.0.0.1
        root[common(data)][common(ip)] = heartBeat.ip;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_Camera(CameraInfo &cameraInfo, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        cameraInfo.msgid = root[common(msgid)].asInt();

        //data license
        cameraInfo.license = root[common(data)][common(license)].asString();
        //data time
        cameraInfo.captime = root[common(data)][common(time)].asDouble();
        //data sn
        cameraInfo.sn = root[common(data)][common(sn)].asString();
        //data devip
        cameraInfo.devip = root[common(data)][common(devip)].asString();
        //data province
        cameraInfo.province = root[common(data)][common(province)].asString();
        //data city
        cameraInfo.city = root[common(data)][common(city)].asString();
        //data number
        cameraInfo.number = root[common(data)][common(number)].asString();
        //data picture
        cameraInfo.picture = root[common(data)][common(picture)].asString();

        return 0;
    }

    int SetInfo_Camera(string &json_str, CameraInfo cameraInfo) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = cameraInfo.code;
        //root msgid
        root[common(msgid)] = cameraInfo.msgid;

        //data license
        root[common(data)][common(license)] = cameraInfo.license;
        //data time
        root[common(data)][common(time)] = (double) cameraInfo.captime;
        //data sn
        root[common(data)][common(sn)] = cameraInfo.sn;
        //data devip
        root[common(data)][common(devip)] = cameraInfo.devip;
        //data province
        root[common(data)][common(province)] = cameraInfo.province;
        //data city
        root[common(data)][common(city)] = cameraInfo.city;
        //data number
        root[common(data)][common(number)] = cameraInfo.number;
        //data picture
        root[common(data)][common(picture)] = cameraInfo.picture;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_GateRich(GateRich &gateRich, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        gateRich.msgid = root[common(msgid)].asInt();

        //data sn
        gateRich.sn = root[common(data)][common(sn)].asString();
        //data car_num
        gateRich.car_num = root[common(data)][common(car_num)].asString();
        //data show
        gateRich.show = root[common(data)][common(show)].asString();
        //data voice
        gateRich.voice = root[common(data)][common(voice)].asString();
        //data lock_queue_recv
        string lock_str = root[common(data)][common(lock)].asString();
        if (lock_str == "on") {
            gateRich.lock = true;
        } else if (lock_str == "off") {
            gateRich.lock = false;
        }
        //data gate
        string gate_str = root[common(data)][common(gate)].asString();
        if (gate_str == "on") {
            gateRich.gate = true;
        } else if (gate_str == "off") {
            gateRich.gate = false;
        }
        //data voice_id
        gateRich.voice_id = root[common(data)][common(voice_id)].asInt();
        //data voice_wparam
        gateRich.voice_wparam = root[common(data)][common(voice_wparam)].asDouble();
        //data voice_lparam
        gateRich.voice_lparam = root[common(data)][common(voice_lparam)].asDouble();

        return 0;
    }

    int SetInfo_GateRich(string &json_str, GateRich gateRich) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = gateRich.code;
        //root msgid
        root[common(msgid)] = gateRich.msgid;

        //data sn
        root[common(data)][common(sn)] = gateRich.sn;
        //data car_num
        root[common(data)][common(car_num)] = gateRich.car_num;
        //data show
        root[common(data)][common(show)] = gateRich.show;
        //data voice
        root[common(data)][common(voice)] = gateRich.voice;
        //data lock_queue_recv
        root[common(data)][common(lock)] = (gateRich.lock ? "on" : "off");
        //data gate
        root[common(data)][common(gate)] = (gateRich.gate ? "on" : "off");
        //data voice_id
        root[common(data)][common(voice_id)] = gateRich.voice_id;
        //data voice_wparam
        root[common(data)][common(voice_wparam)] = gateRich.voice_wparam;
        //data voice_lparam
        root[common(data)][common(voice_lparam)] = gateRich.voice_lparam;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_Gate(Gate &gate, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        gate.msgid = root[common(msgid)].asInt();

        //data sn
        gate.sn = root[common(data)][common(sn)].asString();
        //data gate
        string gate_str = root[common(data)][common(gate)].asString();
        if (gate_str == "on") {
            gate.gate = true;
        } else if (gate_str == "off") {
            gate.gate = false;
        }

        return 0;
    }

    int SetInfo_Gate(string &json_str, Gate gate) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = gate.code;
        //root msgid
        root[common(msgid)] = gate.msgid;

        //data sn
        root[common(data)][common(sn)] = gate.sn;
        //data gate
        root[common(data)][common(gate)] = (gate.gate ? "on" : "off");

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ScreenColor(ScreenColor &screenColor, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        screenColor.msgid = root[common(msgid)].asInt();

        //data sn
        screenColor.sn = root[common(data)][common(sn)].asString();

        return 0;
    }


    int SetInfo_ScreenColor(string &json_str, ScreenColor screenColor) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = screenColor.code;
        //root msgid
        root[common(msgid)] = screenColor.msgid;

        //data sn
        root[common(data)][common(sn)] = screenColor.sn;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ScreenAd(ScreenAd &screenAd, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        screenAd.msgid = root[common(msgid)].asInt();

        //data sn
        screenAd.sn = root[common(data)][common(sn)].asString();
        //data ad
        screenAd.ad = root[common(data)][common(ad)].asString();
        //data line
        screenAd.line = root[common(data)][common(line)].asInt();

        return 0;
    }

    int SetInfo_ScreenAd(string &json_str, ScreenAd screenAd) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = screenAd.code;
        //root msgid
        root[common(msgid)] = screenAd.msgid;

        //data sn
        root[common(data)][common(sn)] = screenAd.sn;
        //data ad
        root[common(data)][common(ad)] = screenAd.ad;
        //data line
        root[common(data)][common(line)] = screenAd.line;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ScreenTime(ScreenTime &screenTime, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        screenTime.msgid = root[common(msgid)].asInt();

        //data sn
        screenTime.sn = root[common(data)][common(sn)].asString();

        return 0;
    }

    int SetInfo_ScreenTime(string &json_str, ScreenTime screenTime) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = screenTime.code;
        //root msgid
        root[common(msgid)] = screenTime.msgid;

        //data sn
        root[common(data)][common(sn)] = screenTime.sn;

        json_str = fastWriter.write(root);

        return 0;
    }


    int GetInfo_LogIn_Req(LogIn_Req &logInReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        logInReq.msgid = root[common(msgid)].asInt();

        //data user_name
        logInReq.user_name = root[common(data)][common(user_name)].asString();
        //data user_passwd
        logInReq.user_passwd = root[common(data)][common(user_passwd)].asString();
        //data gate_name
        logInReq.gate_name = root[common(data)][common(gate_name)].asString();
        //data gate_id
        logInReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data gate_ids_str
        logInReq.gate_ids_str = root[common(data)][common(gate_ids_str)].asString();
        //data garage_id
        logInReq.garage_id = root[common(data)][common(garage_id)].asString();
        //data way
        logInReq.way = root[common(data)][common(way)].asInt();

        return 0;
    }


    int SetInfo_LogIn_Req(string &json_str, LogIn_Req logInReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = logInReq.code;
        //root msgid
        root[common(msgid)] = logInReq.msgid;

        //data user_name
        root[common(data)][common(user_name)] = logInReq.user_name;
        //data user_passwd
        root[common(data)][common(user_passwd)] = logInReq.user_passwd;
        //data gate_name
        root[common(data)][common(gate_name)] = logInReq.gate_name;
        //data gate_id
        root[common(data)][common(gate_id)] = logInReq.gate_id;
        //data gate_ids_str
        root[common(data)][common(gate_ids_str)] = logInReq.gate_ids_str;
        //data garage_id
        root[common(data)][common(garage_id)] = logInReq.garage_id;
        //data way
        root[common(data)][common(way)] = logInReq.way;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_LogInSuccess_Back(LogInSuccess_Back &logInSuccessBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        logInSuccessBack.msgid = root[common(msgid)].asInt();

        //data garage_id
        logInSuccessBack.garage_id = root[common(data)][common(garage_id)].asString();
        //data gate_id
        logInSuccessBack.gate_id = root[common(data)][common(gate_id)].asString();
        //data result
        logInSuccessBack.result = root[common(data)][common(result)].asString();
        //data reason
        for (auto iter:root[common(data)][common(reason)]) {
            OpenReason item;
            item.reason_type = iter[common(reason_type)].asInt();
            item.reason_id = iter[common(reason_id)].asString();
            item.reason_name = iter[common(reason_id)].asString();
            logInSuccessBack.reason.push_back(item);
        }
        return 0;
    }

    int SetInfo_LogInSuccess_Back(string &json_str, LogInSuccess_Back logInSuccessBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = logInSuccessBack.code;
        //root msgid
        root[common(msgid)] = logInSuccessBack.msgid;

        //data garage_id
        root[common(data)][common(garage_id)] = logInSuccessBack.garage_id;
        //data gate_id
        root[common(data)][common(gate_id)] = logInSuccessBack.gate_id;
        //data result
        root[common(data)][common(result)] = logInSuccessBack.result;
        //data reason
        Json::Value reason;
        for (auto iter:logInSuccessBack.reason) {
            Json::Value item;
            item[common(reason_type)] = iter.reason_type;
            item[common(reason_id)] = iter.reason_id;
            item[common(reason_name)] = iter.reason_name;
            reason.append(item);
        }
        root[common(data)][common(reason)] = reason;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_LogInFailed_Back(LoginFailed_Back &loginFailedBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        loginFailedBack.msgid = root[common(msgid)].asInt();

        //data error
        loginFailedBack.error = root[common(data)][common(error)].asString();
        //data result
        loginFailedBack.result = root[common(data)][common(result)].asString();

        return 0;
    }

    int SetInfo_LogInFailed_Back(string &json_str, LoginFailed_Back loginFailedBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = loginFailedBack.code;
        //root msgid
        root[common(msgid)] = loginFailedBack.msgid;

        //data error
        root[common(data)][common(error)] = loginFailedBack.error;
        //data result
        root[common(data)][common(result)] = loginFailedBack.result;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_LogInCover_Back(LoginCover_Back &loginCoverBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        loginCoverBack.msgid = root[common(msgid)].asInt();
        //data garage_id
        loginCoverBack.garage_id = root[common(data)][common(garage_id)].asString();
        //data gate_name
        loginCoverBack.gate_name = root[common(data)][common(gate_name)].asString();
        //data user_name
        loginCoverBack.user_name = root[common(data)][common(user_name)].asString();

        return 0;
    }


    int SetInfo_LogInCover_Back(string &json_str, LoginCover_Back loginCoverBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = loginCoverBack.code;
        //root msgid
        root[common(msgid)] = loginCoverBack.msgid;

        //data garage_id
        root[common(data)][common(garage_id)] = loginCoverBack.garage_id;
        //data gate_name
        root[common(data)][common(gate_name)] = loginCoverBack.gate_name;
        //data user_name
        root[common(data)][common(user_name)] = loginCoverBack.user_name;

        json_str = fastWriter.write(root);
        return 0;
    }

    int GetInfo_GateInfo_Req(GateInfo_Req &gateInfoReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }

        //root msgid
        gateInfoReq.msgid = root[common(msgid)].asInt();

        return 0;
    }

    int SetInfo_GateInfo_Req(string &json_str, GateInfo_Req gateInfoReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = gateInfoReq.code;
        //root msgid
        root[common(msgid)] = gateInfoReq.msgid;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_GateInfo_Back(GateInfo_Back &gateInfoBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }

        //root msgid
        gateInfoBack.msgid = root[common(msgid)].asInt();
        //data gate_number
        gateInfoBack.gate_number = root[common(data)][common(gate_number)].asInt();
        //data gate_array
        for (auto iter:root[common(data)][common(gate_array)]) {
            GateItem item;
            item.gate_id = iter[common(gate_id)].asString();
            item.gate_name = iter[common(gate_name)].asString();
            item.garage_id = iter[common(garage_id)].asString();
            item.camera_addr = iter[common(camera_addr)].asString();
            item.in_out_type = iter[common(in_out_type)].asInt();
            gateInfoBack.gate_array.push_back(item);
        }
        //data exception_price
        gateInfoBack.exception_price = root[common(data)][common(exception_price)].asDouble();

        return 0;
    }

    int SetInfo_GateInfo_Back(string &json_str, GateInfo_Back gateInfoBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = gateInfoBack.code;
        //root msgid
        root[common(msgid)] = gateInfoBack.msgid;
        //data gate_number
        root[common(data)][common(gate_number)] = gateInfoBack.gate_number;
        //data gate_array
        Json::Value gate_array;
        for (auto iter:gateInfoBack.gate_array) {
            Json::Value item;
            item[common(gate_id)] = iter.gate_id;
            item[common(gate_name)] = iter.gate_name;
            item[common(garage_id)] = iter.garage_id;
            item[common(camera_addr)] = iter.camera_addr;
            item[common(in_out_type)] = iter.in_out_type;
            gate_array.append(item);
        }
        root[common(data)][common(gate_array)] = gate_array;
        //data exception_price
        root[common(data)][common(exception_price)] = gateInfoBack.exception_price;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_Match_Req(Match_Req &matchReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        matchReq.msgid = root[common(msgid)].asInt();

        //data time
        matchReq.time = root[common(data)][common(time)].asString();
        //data car_num
        matchReq.car_num = root[common(data)][common(car_num)].asString();
        //data gate_id
        matchReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data picture
        matchReq.picture = root[common(data)][common(picture)].asString();

        return 0;
    }

    int SetInfo_Match_Req(string &json_str, Match_Req matchReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = matchReq.code;
        //root msgid
        root[common(msgid)] = matchReq.msgid;

        //data time
        root[common(data)][common(time)] = matchReq.time;
        //data car_num
        root[common(data)][common(car_num)] = matchReq.car_num;
        //data gate_id
        root[common(data)][common(gate_id)] = matchReq.gate_id;
        //data picture
        root[common(data)][common(picture)] = matchReq.picture;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_Match_Back(Match_Back &matchBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        matchBack.msgid = root[common(msgid)].asInt();
        //data number
        matchBack.number = root[common(data)][common(number)].asInt();
        //data car_num
        matchBack.car_num = root[common(data)][common(car_num)].asString();
        //data car_picture
        matchBack.car_picture = root[common(data)][common(car_picture)].asString();
        //data space_id
        matchBack.space_id = root[common(data)][common(space_id)].asString();
        //data exception_price
        matchBack.exception_price = root[common(data)][common(exception_price)].asDouble();
        //data car_array
        for (auto iter:root[common(data)][common(car_array)]) {
            CarMatchItem item;
            //car_num
            item.car_num = iter[common(car_num)].asString();
            //in_time
            item.in_time = iter[common(in_time)].asString();
            //car_picture
            item.car_picture = iter[common(car_picture)].asString();
            matchBack.car_array.push_back(item);
        }

        return 0;
    }

    int SetInfo_Match_Back(string &json_str, Match_Back matchBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = matchBack.code;
        //root msgid
        root[common(msgid)] = matchBack.msgid;

        //data number
        root[common(data)][common(number)] = matchBack.number;
        //data car_num
        root[common(data)][common(car_num)] = matchBack.car_num;
        //data car_picture
        root[common(data)][common(car_picture)] = matchBack.car_picture;
        //data space_id
        root[common(data)][common(space_id)] = matchBack.space_id;
        //data exception_price
        root[common(data)][common(exception_price)] = matchBack.exception_price;

        //data car_array
        Json::Value car_array;
        for (auto iter:matchBack.car_array) {
            Json::Value item;
            //car_num
            item[common(car_num)] = iter.car_num;
            //in_time
            item[common(in_time)] = iter.in_time;
            //car_picture
            item[common(car_picture)] = iter.car_picture;
            car_array.append(item);
        }
        root[common(data)][common(car_array)] = car_array;

        json_str = fastWriter.write(root);

        return 0;
    }


    int GetInfo_PassIn_Req(PassIn_Req &passInReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //msgid
        passInReq.msgid = root[common(msgid)].asInt();

        //data camera_ip
        passInReq.camera_ip = root[common(data)][common(camera_ip)].asString();
        //data car_num
        passInReq.car_num = root[common(data)][common(car_num)].asString();
        //data gate_id
        passInReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data open_cause
        passInReq.open_cause = root[common(data)][common(open_cause)].asString();
        //data visiting_address
        passInReq.visiting_address = root[common(data)][common(visiting_address)].asString();
        //data visitor_name
        passInReq.visitor_name = root[common(data)][common(visitor_name)].asString();
        //data visitor_phone
        passInReq.visitor_phone = root[common(data)][common(visitor_phone)].asString();
//        //data picture
//        passInReq.picture = root[common(data)][common(picture)].asString();
        //data space_id
        passInReq.space_id = root[common(data)][common(space_id)].asString();
        //data bussiness_id
        passInReq.bussiness_id = root[common(data)][common(bussiness_id)].asString();
        //data res_garage_id
        passInReq.res_garage_id = root[common(data)][common(res_garage_id)].asString();
        //data is_empty_open
        passInReq.is_empty_open = root[common(data)][common(is_empty_open)].asInt();
//        //data visitor_time
//        passInReq.visitor_time = root[common(data)][common(visitor_time)].asInt();
//        //data car_type_id
//        passInReq.car_type_id = root[common(data)][common(car_type_id)].asInt();
//        //data led_car_type
//        passInReq.led_car_type = root[common(data)][common(led_car_type)].asInt();
//        //data use_res_num
//        passInReq.use_res_num = root[common(data)][common(use_res_num)].asInt();
//        //data car_owner_name
//        passInReq.car_owner_name = root[common(data)][common(car_owner_name)].asString();
//        //data car_owner_phone
//        passInReq.car_owner_phone = root[common(data)][common(car_owner_phone)].asString();
//        //data checkin_str
//        passInReq.checkin_str = root[common(data)][common(checkin_str)].asString();

        return 0;
    }

    int SetInfo_PassIn_Req(string &json_str, PassIn_Req passInReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = passInReq.code;
        //root msgid
        root[common(msgid)] = passInReq.msgid;

        //data camera_ip
        root[common(data)][common(camera_ip)] = passInReq.camera_ip;
        //data car_num
        root[common(data)][common(car_num)] = passInReq.car_num;
        //data gate_id
        root[common(data)][common(gate_id)] = passInReq.gate_id;
        //data open_cause
        root[common(data)][common(open_cause)] = passInReq.open_cause;
        //data visiting_address
        root[common(data)][common(visiting_address)] = passInReq.visiting_address;
        //data visitor_name
        root[common(data)][common(visitor_name)] = passInReq.visitor_name;
        //data visitor_phone
        root[common(data)][common(visitor_phone)] = passInReq.visitor_phone;
//        //data picture
//      root[common(data)][common(picture)] = passInReq.picture;
        //data space_id
        root[common(data)][common(space_id)] = passInReq.space_id;
        //data bussiness_id
        root[common(data)][common(bussiness_id)] = passInReq.bussiness_id;
        //data res_garage_id
        root[common(data)][common(res_garage_id)] = passInReq.res_garage_id;
        //data is_empty_open
        root[common(data)][common(is_empty_open)] = passInReq.is_empty_open;
//        //data visitor_time
//        root[common(data)][common(visitor_time)] = (double)passInReq.visitor_time;
//        //data car_type_id
//        root[common(data)][common(car_type_id)] = passInReq.car_type_id;
//        //data led_car_type
//        root[common(data)][common(led_car_type)] = passInReq.led_car_type;
//        //data use_res_num
//        root[common(data)][common(use_res_num)] = passInReq.use_res_num;
//        //data car_owner_name
//        root[common(data)][common(car_owner_name)] = passInReq.car_owner_name;
//        //data car_owner_phone
//        root[common(data)][common(car_owner_phone)]= passInReq.car_owner_phone;
//        //data checkin_str
//        root[common(data)][common(checkin_str)] = passInReq.checkin_str;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PassIn_Back(PassIn_Back &passInBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //msgid
        passInBack.msgid = root[common(msgid)].asInt();
        //data camera_ip
        passInBack.camera_ip = root[common(data)][common(camera_ip)].asString();
        //data car_num
        passInBack.car_num = root[common(data)][common(car_num)].asString();
        //data car_statu
        passInBack.car_statu = root[common(data)][common(car_statu)].asString();
        //data car_type
        passInBack.car_type = root[common(data)][common(car_type)].asString();
        //data gate_id
        passInBack.gate_id = root[common(data)][common(gate_id)].asString();
        //data space_id
        passInBack.space_id = root[common(data)][common(space_id)].asString();
        //data picture
        passInBack.picture = root[common(data)][common(picture)].asString();
        //data bussiness_id
        passInBack.bussiness_id = root[common(data)][common(bussiness_id)].asString();
        //data res_garage_id
        passInBack.res_garage_id = root[common(data)][common(res_garage_id)].asString();
        //data visitor_time
        passInBack.visitor_time = root[common(data)][common(visitor_time)].asDouble();
        //data car_type_id
        passInBack.car_type_id = root[common(data)][common(car_type_id)].asInt();
        //data use_res_num
        passInBack.use_res_num = root[common(data)][common(use_res_num)].asInt();
        //data led_car_type
        passInBack.led_car_type = root[common(data)][common(led_car_type)].asInt();
        //data car_owner_name
        passInBack.car_owner_name = root[common(data)][common(car_owner_name)].asString();
        //data car_owner_phone
        passInBack.car_owner_phone = root[common(data)][common(car_owner_phone)].asString();
        //data checkin_str
        passInBack.checkin_str = root[common(data)][common(checkin_str)].asString();
        //data reason
        for (auto iter:root[common(data)][common(reason)]) {
            OpenReason item;
            //data reason reason_type
            item.reason_type = iter[common(reason_type)].asInt();
            //data reason reason_id
            item.reason_id = iter[common(reason_id)].asString();
            //data reason reason_name
            item.reason_name = iter[common(reason_name)].asString();
            passInBack.reason.push_back(item);
        }

        return 0;
    }

    int SetInfo_PassIn_Back(string &json_str, PassIn_Back passInBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = passInBack.code;
        //root msgid
        root[common(msgid)] = passInBack.msgid;

        //data camera_ip
        root[common(data)][common(camera_ip)] = passInBack.camera_ip;
        //data car_num
        root[common(data)][common(car_num)] = passInBack.car_num;
        //data car_statu
        root[common(data)][common(car_statu)] = passInBack.car_statu;
        //data car_type
        root[common(data)][common(car_type)] = passInBack.car_type;
        //data gate_id
        root[common(data)][common(gate_id)] = passInBack.gate_id;
        //data space_id
        root[common(data)][common(space_id)] = passInBack.space_id;
        //data picture
        root[common(data)][common(picture)] = passInBack.picture;
        //data bussiness_id
        root[common(data)][common(bussiness_id)] = passInBack.bussiness_id;
        //data res_garage_id
        root[common(data)][common(res_garage_id)] = passInBack.res_garage_id;
        //data visitor_time
        root[common(data)][common(visitor_time)] = (double) passInBack.visitor_time;
        //data car_type_id
        root[common(data)][common(car_type_id)] = passInBack.car_type_id;
        //data use_res_num
        root[common(data)][common(use_res_num)] = passInBack.use_res_num;
        //data led_car_type
        root[common(data)][common(led_car_type)] = passInBack.led_car_type;
        //data car_owner_name
        root[common(data)][common(car_owner_name)] = passInBack.car_owner_name;
        //data car_owner_phone
        root[common(data)][common(car_owner_phone)] = passInBack.car_owner_phone;
        //data checkin_str
        root[common(data)][common(checkin_str)] = passInBack.checkin_str;

        //data reason
        Json::Value reason;
        for (auto iter:passInBack.reason) {
            Json::Value item;
            //data reason reason_type
            item[common(reason_type)] = iter.reason_type;
            //data reason reason_id
            item[common(reason_id)] = iter.reason_id;
            //data reason reason_name
            item[common(reason_name)] = iter.reason_name;
            reason.append(item);
        }
        root[common(data)][common(reason)] = reason;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PassOut_Req(PassOut_Req &passOutReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        passOutReq.msgid = root[common(msgid)].asInt();

        //data camera_ip
        passOutReq.camera_ip = root[common(data)][common(camera_ip)].asString();
        //data car_num
        passOutReq.car_num = root[common(data)][common(car_num)].asString();
        //data gate_id
        passOutReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data preferential_reason
        passOutReq.preferential_reason = root[common(data)][common(preferential_reason)].asString();
        //data username
        passOutReq.username = root[common(data)][common(username)].asString();
        //data amount_due
        passOutReq.amount_due = root[common(data)][common(amount_due)].asDouble();
        //data amount_paid
        passOutReq.amount_paid = root[common(data)][common(amount_paid)].asDouble();
        //data amount_to_paid
        passOutReq.amount_to_paid = root[common(data)][common(amount_to_paid)].asDouble();
        //data preferential_amount
        passOutReq.preferential_amount = root[common(data)][common(preferential_amount)].asDouble();
//        //data parking_id
//        passOutReq.parking_id = root[common(data)][common(parking_id)].asString();
//        //data show
//        passOutReq.show = root[common(data)][common(show)].asString();
//        //data voice
//        passOutReq.voice = root[common(data)][common(voice)].asString();
//        //data car_picture
//        passOutReq.car_picture = root[common(data)][common(car_picture)].asString();
        //data space_id
        passOutReq.space_id = root[common(data)][common(space_id)].asString();
//        //data bussiness_id
//        passOutReq.bussiness_id = root[common(data)][common(bussiness_id)].asString();
//        //data res_garage_id
//        passOutReq.res_garage_id = root[common(data)][common(res_garage_id)].asString();
//        //data target_garage_id
//        passOutReq.target_garage_id = root[common(data)][common(target_garage_id)].asString();
        //data is_empty_open
        passOutReq.is_empty_open = root[common(data)][common(is_empty_open)].asBool();
//        //data visitor_time
//        passOutReq.visitor_time = root[common(data)][common(visitor_time)].asDouble();
//        //data car_type_id
//        passOutReq.car_type_id = root[common(data)][common(car_type_id)].asInt();
//        //data use_res_num
//        passOutReq.use_res_num = root[common(data)][common(use_res_num)].asInt();
//        //data car_owner_name
//        passOutReq.car_owner_name = root[common(data)][common(car_owner_name)].asString();
//        //data car_owner_phone
//        passOutReq.car_owner_phone = root[common(data)][common(car_owner_phone)].asString();

        return 0;
    }

    int SetInfo_PassOut_Req(string &json_str, PassOut_Req passOutReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = passOutReq.code;
        //root msgid
        root[common(msgid)] = passOutReq.msgid;
        //data camera_ip
        root[common(data)][common(camera_ip)] = passOutReq.camera_ip;
        //data car_num
        root[common(data)][common(car_num)] = passOutReq.car_num;
        //data gate_id
        root[common(data)][common(gate_id)] = passOutReq.gate_id;
        //data preferential_reason
        root[common(data)][common(preferential_reason)] = passOutReq.preferential_reason;
        //data username
        root[common(data)][common(username)] = passOutReq.username;
        //data amount_due
        root[common(data)][common(amount_due)] = passOutReq.amount_due;
        //data amount_paid
        root[common(data)][common(amount_paid)] = passOutReq.amount_paid;
        //data amount_to_paid
        root[common(data)][common(amount_to_paid)] = passOutReq.amount_to_paid;
        //data preferential_amount
        root[common(data)][common(preferential_amount)] = passOutReq.preferential_amount;
//        //data parking_id
//        root[common(data)][common(parking_id)] = passOutReq.parking_id;
//        //data show
//        root[common(data)][common(show)] = passOutReq.show;
//        //data voice
//        root[common(data)][common(voice)] = passOutReq.voice;
//        //data car_picture
//        root[common(data)][common(car_picture)] = passOutReq.car_picture;
        //data space_id
        root[common(data)][common(space_id)] = passOutReq.space_id;
//        //data bussiness_id
//        root[common(data)][common(bussiness_id)] = passOutReq.bussiness_id;
//        //data res_garage_id
//        root[common(data)][common(res_garage_id)]=passOutReq.res_garage_id;
//        //data target_garage_id
//        root[common(data)][common(target_garage_id)]=passOutReq.target_garage_id;
        //data is_empty_open
        root[common(data)][common(is_empty_open)] = passOutReq.is_empty_open;
//        //data visitor_time
//        root[common(data)][common(visitor_time)]=(double)passOutReq.visitor_time;
//        //data car_type_id
//        root[common(data)][common(car_type_id)]=passOutReq.car_type_id;
//        //data use_res_num
//        root[common(data)][common(use_res_num)]=passOutReq.use_res_num;
//        //data car_owner_name
//        root[common(data)][common(car_owner_name)]=passOutReq.car_owner_name;
//        //data car_owner_phone
//        root[common(data)][common(car_owner_phone)]=passOutReq.car_owner_phone;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PassOut_Back(PassOut_Back &passOutBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        passOutBack.msgid = root[common(msgid)].asInt();
        //data camera_ip
        passOutBack.camera_ip = root[common(data)][common(camera_ip)].asString();
        //data car_num
        passOutBack.car_num = root[common(data)][common(car_num)].asString();
        //data car_statu
        passOutBack.car_statu = root[common(data)][common(car_statu)].asString();
        //data car_type
        passOutBack.car_type = root[common(data)][common(car_type)].asString();
        //data gate_id
        passOutBack.gate_id = root[common(data)][common(gate_id)].asString();
        //data car_picture
        passOutBack.car_picture = root[common(data)][common(car_picture)].asString();
        //data picture
        passOutBack.car_picture = root[common(data)][common(picture)].asString();
        //data in_time
        passOutBack.in_time = root[common(data)][common(in_time)].asString();
        //data out_time
        passOutBack.out_time = root[common(data)][common(out_time)].asString();
        //data note
        passOutBack.note = root[common(data)][common(note)].asString();
        //data stay_time
        passOutBack.stay_time = root[common(data)][common(stay_time)].asString();
        //data parking_id
        passOutBack.parking_id = root[common(data)][common(parking_id)].asString();
        //data voice
        passOutBack.voice = root[common(data)][common(voice)].asString();
        //data show
        passOutBack.show = root[common(data)][common(show)].asString();
        //data amount_due
        passOutBack.amount_due = root[common(data)][common(amount_due)].asDouble();
        //data amount_to_paid
        passOutBack.amount_to_paid = root[common(data)][common(amount_to_paid)].asDouble();
        //data abnormal_amount
        passOutBack.abnormal_amount = root[common(data)][common(abnormal_amount)].asDouble();
        //data exception_price
        passOutBack.exception_price = root[common(data)][common(exception_price)].asDouble();
        //data space_id
        passOutBack.space_id = root[common(data)][common(space_id)].asString();
        //data bussiness_id
        passOutBack.bussiness_id = root[common(data)][common(bussiness_id)].asString();
        //data res_garage_id
        passOutBack.res_garage_id = root[common(data)][common(res_garage_id)].asString();
        //data target_garage_id
        passOutBack.target_garage_id = root[common(data)][common(target_garage_id)].asString();
        //data visitor_time
        passOutBack.visitor_time = root[common(data)][common(visitor_time)].asDouble();
        //data car_type_id
        passOutBack.car_type_id = root[common(data)][common(car_type_id)].asInt();
        //data use_res_num
        passOutBack.use_res_num = root[common(data)][common(use_res_num)].asInt();
        //data car_owner_name
        passOutBack.car_owner_name = root[common(data)][common(car_owner_name)].asString();
        //data car_owner_phone
        passOutBack.car_owner_phone = root[common(data)][common(car_owner_phone)].asString();
        //data reason
        for (auto iter:root[common(data)][common(reason)]) {
            OpenReason item;
            //data reason reason_type
            item.reason_type = iter[common(reason_type)].asInt();
            //data reason reason_id
            item.reason_id = iter[common(reason_id)].asString();
            //data reason reason_name
            item.reason_name = iter[common(reason_name)].asString();
            passOutBack.reason.push_back(item);
        }

        return 0;
    }

    int SetInfo_PassOut_Back(string &json_str, PassOut_Back passOutBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = passOutBack.code;
        //root msgid
        root[common(msgid)] = passOutBack.msgid;

        //data camera_ip
        root[common(data)][common(camera_ip)] = passOutBack.camera_ip;
        //data car_num
        root[common(data)][common(car_num)] = passOutBack.car_num;
        //data car_statu
        root[common(data)][common(car_statu)] = passOutBack.car_statu;
        //data car_type
        root[common(data)][common(car_type)] = passOutBack.car_type;
        //data gate_id
        root[common(data)][common(gate_id)] = passOutBack.gate_id;
        //data car_picture
        root[common(data)][common(car_picture)] = passOutBack.car_picture;
        //data picture
        root[common(data)][common(picture)] = passOutBack.car_picture;
        //data in_time
        root[common(data)][common(in_time)] = passOutBack.in_time;
        //data out_time
        root[common(data)][common(out_time)] = passOutBack.out_time;
        //data note
        root[common(data)][common(note)] = passOutBack.note;
        //data stay_time
        root[common(data)][common(stay_time)] = passOutBack.stay_time;
        //data parking_id
        root[common(data)][common(parking_id)] = passOutBack.parking_id;
        //data voice
        root[common(data)][common(voice)] = passOutBack.voice;
        //data show
        root[common(data)][common(show)] = passOutBack.show;
        //data amount_due
        root[common(data)][common(amount_due)] = passOutBack.amount_due;
        //data amount_to_paid
        root[common(data)][common(amount_to_paid)] = passOutBack.amount_to_paid;
        //data abnormal_amount
        root[common(data)][common(abnormal_amount)] = passOutBack.abnormal_amount;
        //data exception_price
        root[common(data)][common(exception_price)] = passOutBack.exception_price;
        //data space_id
        root[common(data)][common(space_id)] = passOutBack.space_id;
        //data bussiness_id
        root[common(data)][common(bussiness_id)] = passOutBack.bussiness_id;
        //data res_garage_id
        root[common(data)][common(res_garage_id)] = passOutBack.res_garage_id;
        //data target_garage_id
        root[common(data)][common(target_garage_id)] = passOutBack.target_garage_id;
        //data visitor_time
        root[common(data)][common(visitor_time)] = (double) passOutBack.visitor_time;
        //data car_type_id
        root[common(data)][common(car_type_id)] = passOutBack.car_type_id;
        //data use_res_num
        root[common(data)][common(use_res_num)] = passOutBack.use_res_num;
        //data car_owner_name
        root[common(data)][common(car_owner_name)] = passOutBack.car_owner_name;
        //data car_owner_phone
        root[common(data)][common(car_owner_phone)] = passOutBack.car_owner_phone;
        //data reason
        Json::Value reason;
        for (auto iter:passOutBack.reason) {
            Json::Value item;
            //data reason reason_type
            item[common(reason_type)] = iter.reason_type;
            //data reason reason_id
            item[common(reason_id)] = iter.reason_id;
            //data reason reason_name
            item[common(reason_name)] = iter.reason_name;
            reason.append(item);
        }
        root[common(data)][common(reason)] = reason;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ChargeAmount_Req(ChargeAmount_Req &chargeAmountReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        chargeAmountReq.msgid = root[common(msgid)].asInt();

        //data user_name
        chargeAmountReq.user_name = root[common(data)][common(user_name)].asString();
        //data gate_id
        chargeAmountReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data gate_ids_str
        chargeAmountReq.gate_ids_str = root[common(data)][common(gate_ids_str)].asString();

        return 0;
    }

    int SetInfo_ChargeAmount_Req(string &json_str, ChargeAmount_Req chargeAmountReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = chargeAmountReq.code;
        //root msgid
        root[common(msgid)] = chargeAmountReq.msgid;
        //data user_name
        root[common(data)][common(user_name)] = chargeAmountReq.user_name;
        //data gate_id
        root[common(data)][common(gate_id)] = chargeAmountReq.gate_id;
        //data gate_ids_str
        root[common(data)][common(gate_ids_str)] = chargeAmountReq.gate_ids_str;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ChargeAmount_Back(ChargeAmount_Back &chargeAmountBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        chargeAmountBack.msgid = root[common(msgid)].asInt();
        //data total_amount
        chargeAmountBack.total_amount = root[common(data)][common(total_amount)].asDouble();

        return 0;
    }

    int SetInfo_ChargeAmount_Back(string &json_str, ChargeAmount_Back chargeAmountBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = chargeAmountBack.code;
        //root msgid
        root[common(msgid)] = chargeAmountBack.msgid;

        //data total_amount
        root[common(data)][common(total_amount)] = chargeAmountBack.total_amount;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ChargeRecord_Req(ChargeRecord_Req &chargeRecordReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        chargeRecordReq.msgid = root[common(msgid)].asInt();

        //data user_name
        chargeRecordReq.user_name = root[common(data)][common(user_name)].asString();
        //data gate_id
        chargeRecordReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data start_number
        chargeRecordReq.start_number = root[common(data)][common(start_number)].asInt();
        //data lines
        chargeRecordReq.lines = root[common(data)][common(lines)].asInt();
        //data gate_ids_str
        chargeRecordReq.gate_ids_str = root[common(data)][common(gate_ids_str)].asString();

        return 0;
    }

    int SetInfo_ChargeRecord_Req(string &json_str, ChargeRecord_Req chargeRecordReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = chargeRecordReq.code;
        //root msgid
        root[common(msgid)] = chargeRecordReq.msgid;
        //data user_name
        root[common(data)][common(user_name)] = chargeRecordReq.user_name;
        //data gate_id
        root[common(data)][common(gate_id)] = chargeRecordReq.gate_id;
        //data start_number
        root[common(data)][common(start_number)] = chargeRecordReq.start_number;
        //data lines
        root[common(data)][common(lines)] = chargeRecordReq.lines;
        //data gate_ids_str
        root[common(data)][common(gate_ids_str)] = chargeRecordReq.gate_ids_str;

        json_str = fastWriter.write(root);
        return 0;
    }

    int GetInfo_ChargeRecord_Back(ChargeRecord_Back &chargeRecordBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        chargeRecordBack.msgid = root[common(msgid)].asInt();

        //data total_amount
        chargeRecordBack.total_amount = root[common(data)][common(total_amount)].asDouble();
        //data total_discount
        chargeRecordBack.total_discount = root[common(data)][common(total_discount)].asDouble();
        //data total_gate_charge
        chargeRecordBack.total_gate_charge = root[common(data)][common(total_gate_charge)].asDouble();
        //data total_number
        chargeRecordBack.total_number = root[common(data)][common(total_number)].asInt();
        //data number
        chargeRecordBack.number = root[common(data)][common(number)].asInt();
        //data total_selfpay
        chargeRecordBack.total_selfpay = root[common(data)][common(total_selfpay)].asDouble();

        //data record_array
        for (auto iter: root[common(data)][common(record_array)]) {
            RecordItem item;
            //car_num
            item.car_num = iter[common(car_num)].asString();
            //preferential_reason
            item.preferential_reason = iter[common(preferential_reason)].asString();
            //preferential_amount
            item.preferential_amount = iter[common(preferential_amount)].asDouble();
            //amount_due
            item.amount_due = iter[common(amount_due)].asDouble();
            //amount_paid
            item.amount_paid = iter[common(amount_paid)].asDouble();
//            //amount_to_paid
//            item.amount_to_paid=iter[common(amount_to_paid)].asDouble();
            //selfpay
            item.self_paid = iter[common(selfpay)].asDouble();

            chargeRecordBack.record_array.push_back(item);
        }

        return 0;
    }


    int SetInfo_ChargeRecord_Back(string &json_str, ChargeRecord_Back chargeRecordBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = chargeRecordBack.code;
        //root msgid
        root[common(msgid)] = chargeRecordBack.msgid;

        //data total_amount
        root[common(data)][common(total_amount)] = chargeRecordBack.total_amount;
        //data total_discount
        root[common(data)][common(total_discount)] = chargeRecordBack.total_discount;
        //data total_gate_charge
        root[common(data)][common(total_gate_charge)] = chargeRecordBack.total_gate_charge;
        //data total_number
        root[common(data)][common(total_number)] = chargeRecordBack.total_number;
        //data number
        root[common(data)][common(number)] = chargeRecordBack.number;
        //data total_selfpay
        root[common(data)][common(total_selfpay)] = chargeRecordBack.total_selfpay;

        //data record_array
        Json::Value record_array;
        for (auto iter:chargeRecordBack.record_array) {
            Json::Value item;
            //car_num
            item[common(car_num)] = iter.car_num;
            //preferential_reason
            item[common(preferential_reason)] = iter.preferential_reason;
            //preferential_amount
            item[common(preferential_amount)] = iter.preferential_amount;
            //amount_due
            item[common(amount_due)] = iter.amount_due;
            //amount_paid
            item[common(amount_paid)] = iter.amount_paid;
//            //amount_to_paid
//            item[common(amount_to_paid)] = iter.amount_to_paid;
            //selfpay
            item[common(selfpay)] = iter.self_paid;

            record_array.append(item);
        }
        root[common(data)][common(record_array)] = record_array;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ChargeContent_Req(ChargeContent_Req &chargeContentReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        chargeContentReq.msgid = root[common(msgid)].asInt();

        //data car_num
        chargeContentReq.car_num = root[common(data)][common(car_num)].asString();
        //data garage_id
        chargeContentReq.garage_id = root[common(data)][common(garage_id)].asString();
        //data gate_id
        chargeContentReq.gate_id = root[common(data)][common(gate_id)].asString();
        //data gate_name
        chargeContentReq.gate_name = root[common(data)][common(gate_name)].asString();
        //data start_line
        chargeContentReq.start_line = root[common(data)][common(start_line)].asInt();
        //data lines
        chargeContentReq.lines = root[common(data)][common(lines)].asInt();

        return 0;
    }

    int SetInfo_ChargeContent_Req(string &json_str, ChargeContent_Req chargeContentReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = chargeContentReq.code;
        //root msgid
        root[common(msgid)] = chargeContentReq.msgid;

        //data car_num
        root[common(data)][common(car_num)] = chargeContentReq.car_num;
        //data garage_id
        root[common(data)][common(garage_id)] = chargeContentReq.garage_id;
        //data gate_id
        root[common(data)][common(gate_id)] = chargeContentReq.gate_id;
        //data gate_name
        root[common(data)][common(gate_name)] = chargeContentReq.gate_name;
        //data start_line
        root[common(data)][common(start_line)] = chargeContentReq.start_line;
        //data lines
        root[common(data)][common(lines)] = chargeContentReq.lines;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ChargeContent_Back(ChargeContent_Back &chargeContentBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        chargeContentBack.msgid = root[common(msgid)].asInt();

        //data number
        chargeContentBack.number = root[common(data)][common(number)].asInt();
        //data total_number
        chargeContentBack.total_number = root[common(data)][common(total_number)].asInt();
        //data car_array
        for (auto iter:root[common(data)][common(car_array)]) {
            ContentItem item;
            //garage_name
            item.garage_name = iter[common(garage_name)].asString();
            //gate_name
            item.gate_name = iter[common(gate_name)].asString();
            //in_time
            item.in_time = iter[common(in_time)].asString();
            //out_time
            item.out_time = iter[common(out_time)].asString();
            //stay_time
            item.stay_time = iter[common(stay_time)].asString();
            //parking_fee
            item.parking_fee = iter[common(parking_fee)].asDouble();

            chargeContentBack.car_array.push_back(item);
        }

        return 0;
    }

    int SetInfo_ChargeContent_Back(string &json_str, ChargeContent_Back chargeContentBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = chargeContentBack.code;
        //root msgid
        root[common(msgid)] = chargeContentBack.msgid;

        //data number
        root[common(data)][common(number)] = chargeContentBack.number;
        //data total_number
        root[common(data)][common(total_number)] = chargeContentBack.total_number;
        //data car_array
        Json::Value car_array;
        for (auto iter:chargeContentBack.car_array) {
            Json::Value item;
            //garage_name
            item[common(garage_name)] = iter.garage_name;
            //gate_name
            item[common(gate_name)] = iter.gate_name;
            //in_time
            item[common(in_time)] = iter.in_time;
            //out_time
            item[common(out_time)] = iter.out_time;
            //stay_time
            item[common(stay_time)] = iter.stay_time;
            //parking_fee
            item[common(parking_fee)] = iter.parking_fee;

            car_array.append(item);
        }
        root[common(data)][common(car_array)] = car_array;

        json_str = fastWriter.write(root);

        return 0;
    }


    int GetInfo_ChangePassword_Req(ChangePassword_Req &changePasswordReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        changePasswordReq.msgid = root[common(msgid)].asInt();

        //data user_name
        changePasswordReq.user_name = root[common(data)][common(user_name)].asString();
        //data old_passwd
        changePasswordReq.old_passwd = root[common(data)][common(old_passwd)].asString();
        //data new_passwd
        changePasswordReq.new_passwd = root[common(data)][common(new_passwd)].asString();

        return 0;
    }

    int SetInfo_ChangePassword_Req(string &json_str, ChangePassword_Req changePasswordReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = changePasswordReq.code;
        //root msgid
        root[common(msgid)] = changePasswordReq.msgid;
        //data user_name
        root[common(data)][common(user_name)] = changePasswordReq.user_name;
        //data old_passwd
        root[common(data)][common(old_passwd)] = changePasswordReq.old_passwd;
        //data new_passwd
        root[common(data)][common(new_passwd)] = changePasswordReq.new_passwd;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_ChangePassword_Back(ChangePassword_Back &changePasswordBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        changePasswordBack.msgid = root[common(msgid)].asInt();

        //data result
        changePasswordBack.result = root[common(data)][common(result)].asString();
        //data error
        changePasswordBack.error = root[common(data)][common(error)].asString();

        return 0;
    }

    int SetInfo_ChangePassword_Back(string &json_str, ChangePassword_Back changePasswordBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = changePasswordBack.code;
        //root msgid
        root[common(msgid)] = changePasswordBack.msgid;

        //data result
        root[common(data)][common(result)] = changePasswordBack.result;
        //data error
        root[common(data)][common(error)] = changePasswordBack.error;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PayOnline_Req(PayOnline_Req &payOnlineReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(body)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        payOnlineReq.msgid = root[common(msgid)].asInt();

        //body request_time
        payOnlineReq.request_time = root[common(body)][common(request_time)].asDouble();
        //body gate_id
        payOnlineReq.gate_id = root[common(body)][common(gate_id)].asString();
        //body plat_nu
        payOnlineReq.plat_nu = root[common(body)][common(plat_nu)].asString();
        //body charge_nu
        payOnlineReq.charge_nu = root[common(body)][common(charge_nu)].asDouble();
        //body open_gate
        payOnlineReq.open_gate = root[common(body)][common(open_gate)].asBool();
        //body order_id
        payOnlineReq.order_id = root[common(body)][common(order_id)].asString();
        //body order_no
        payOnlineReq.order_no = root[common(body)][common(order_no)].asString();
        //body pay_time
        payOnlineReq.pay_time = root[common(body)][common(pay_time)].asDouble();

        return 0;
    }

    int SetInfo_PayOnline_Req(string &json_str, PayOnline_Req payOnlineReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlineReq.code;
        //root msgid
        root[common(msgid)] = payOnlineReq.msgid;

        //body request_time
        root[common(body)][common(request_time)] = (double) payOnlineReq.request_time;
        //body gate_id
        root[common(body)][common(gate_id)] = payOnlineReq.gate_id;
        //body plat_nu
        root[common(body)][common(plat_nu)] = payOnlineReq.plat_nu;
        //body charge_nu
        root[common(body)][common(charge_nu)] = payOnlineReq.charge_nu;
        //body open_gate
        root[common(body)][common(open_gate)] = payOnlineReq.open_gate;
        //body order_id
        root[common(body)][common(order_id)] = payOnlineReq.order_id;
        //body order_no
        root[common(body)][common(order_no)] = payOnlineReq.order_no;
        //body pay_time
        root[common(body)][common(pay_time)] = (double) payOnlineReq.pay_time;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PayOnline_Back(PayOnline_Back &payOnlineBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(body)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        payOnlineBack.msgid = root[common(msgid)].asInt();

        //body gate_id
        payOnlineBack.gate_id = root[common(body)][common(gate_id)].asString();
        //body plat_nu
        payOnlineBack.plat_nu = root[common(body)][common(plat_nu)].asString();
        //body order_id
        payOnlineBack.order_id = root[common(body)][common(order_id)].asString();
        //body order_no
        payOnlineBack.order_no = root[common(body)][common(order_no)].asString();
        //body charge_nu
        payOnlineBack.charge_nu = root[common(body)][common(charge_nu)].asDouble();
        //body request_time
        payOnlineBack.request_time = root[common(body)][common(request_time)].asDouble();
        //body in_park_time
        payOnlineBack.start_time = root[common(body)][common(in_park_time)].asDouble();
        //body stay_time
        payOnlineBack.secs = root[common(body)][common(stay_time)].asDouble();
        //body amount_to_pay
        payOnlineBack.amount_to_pay = root[common(body)][common(amount_to_pay)].asDouble();
        //body amount_paid
        payOnlineBack.amount_paid = root[common(body)][common(amount_paid)].asDouble();
        //body free_reason
        payOnlineBack.free_reason = root[common(body)][common(free_reason)].asInt();

        return 0;
    }

    int SetInfo_PayOnline_Back(string &json_str, PayOnline_Back payOnlineBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlineBack.code;
        //root msgid
        root[common(msgid)] = payOnlineBack.msgid;

        //body gate_id
        root[common(body)][common(gate_id)] = payOnlineBack.gate_id;
        //body plat_nu
        root[common(body)][common(plat_nu)] = payOnlineBack.plat_nu;
        //body order_id
        root[common(body)][common(order_id)] = payOnlineBack.order_id;
        //body order_no
        root[common(body)][common(order_no)] = payOnlineBack.order_no;
        //body charge_nu
        root[common(body)][common(charge_nu)] = payOnlineBack.charge_nu;
        //body request_time
        root[common(body)][common(request_time)] = (double) payOnlineBack.request_time;
        //body in_park_time
        root[common(body)][common(in_park_time)] = (double) payOnlineBack.start_time;
        //body stay_time
        root[common(body)][common(stay_time)] = (double) payOnlineBack.secs;
        //body amount_to_pay
        root[common(body)][common(amount_to_pay)] = payOnlineBack.amount_to_pay;
        //body amount_paid
        root[common(body)][common(amount_paid)] = payOnlineBack.amount_paid;
        //body free_reason
        root[common(body)][common(free_reason)] = payOnlineBack.free_reason;

        json_str = fastWriter.write(root);

        return 0;
    }


    int GetInfo_PayOnlineCommunity_Req(PayOnlineCommunity_Req &payOnlineCommunityReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }

        return 0;
    }

    int SetInfo_PayOnlineCommunity_Req(string &json_str, PayOnlineCommunity_Req payOnlineCommunityReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlineCommunityReq.code;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PayOnlineCommunity_Back(PayOnlineCommunity_Back &payOnlineCommunityBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(body)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //body comm_id
        payOnlineCommunityBack.comm_id = root[common(body)][common(comm_id)].asString();

        return 0;
    }


    int SetInfo_PayOnlineCommunity_Back(string &json_str, PayOnlineCommunity_Back payOnlineCommunityBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlineCommunityBack.code;

        //body comm_id
        root[common(body)][common(comm_id)] = payOnlineCommunityBack.comm_id;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PayOnlinePassIn_Back(PayOnlinePassIn_Back &payOnlinePassInBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(body)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //data msgid
        payOnlinePassInBack.msgid = root[common(msgid)].asInt();

        //body comm_id
        payOnlinePassInBack.comm_id = root[common(body)][common(comm_id)].asString();
        //body plate_nu
        payOnlinePassInBack.plate_nu = root[common(body)][common(plate_nu)].asString();
        //body order_id
        payOnlinePassInBack.order_id = root[common(body)][common(order_id)].asString();
        //body request_time
        payOnlinePassInBack.request_time = root[common(body)][common(request_time)].asDouble();
        //body in_time
        payOnlinePassInBack.in_time = root[common(body)][common(in_time)].asDouble();
        //body free_secs
        payOnlinePassInBack.free_secs = root[common(body)][common(free_secs)].asDouble();

        return 0;
    }


    int SetInfo_PayOnlinePassIn_Back(string &json_str, PayOnlinePassIn_Back payOnlinePassInBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlinePassInBack.code;
        //root msgid
        root[common(msgid)] = payOnlinePassInBack.msgid;

        //body comm_id
        root[common(body)][common(comm_id)] = payOnlinePassInBack.comm_id;
        //body plate_nu
        root[common(body)][common(plate_nu)] = payOnlinePassInBack.plate_nu;
        //body order_id
        root[common(body)][common(order_id)] = payOnlinePassInBack.order_id;
        //body request_time
        root[common(body)][common(request_time)] = (double) payOnlinePassInBack.request_time;
        //body in_time
        root[common(body)][common(in_time)] = (double) payOnlinePassInBack.in_time;
        //body free_secs
        root[common(body)][common(free_secs)] = (double) payOnlinePassInBack.free_secs;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PayOnlinePassOut_Back(PayOnlinePassOut_Back &payOnlinePassOutBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(body)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //data msgid
        payOnlinePassOutBack.msgid = root[common(msgid)].asInt();

        //body comm_id
        payOnlinePassOutBack.comm_id = root[common(body)][common(comm_id)].asString();
        //body plate_nu
        payOnlinePassOutBack.plate_nu = root[common(body)][common(plate_nu)].asString();
        //body order_id
        payOnlinePassOutBack.order_id = root[common(body)][common(order_id)].asString();
        //body gate_id
        payOnlinePassOutBack.gate_id = root[common(body)][common(gate_id)].asString();
        //body request_time
        payOnlinePassOutBack.request_time = root[common(body)][common(request_time)].asDouble();
        //body out_time
        payOnlinePassOutBack.out_time = root[common(body)][common(out_time)].asDouble();
        //body charge_secs
        payOnlinePassOutBack.charge_secs = root[common(body)][common(charge_secs)].asDouble();
        //body charge_nu
        payOnlinePassOutBack.charge_nu = root[common(body)][common(charge_nu)].asDouble();

        return 0;
    }

    int SetInfo_PayOnlinePassOut_Back(string &json_str, PayOnlinePassOut_Back payOnlinePassOutBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlinePassOutBack.code;
        //root msgid
        root[common(msgid)] = payOnlinePassOutBack.msgid;

        //body comm_id
        root[common(body)][common(comm_id)] = payOnlinePassOutBack.comm_id;
        //body plate_nu
        root[common(body)][common(plate_nu)] = payOnlinePassOutBack.plate_nu;
        //body order_id
        root[common(body)][common(order_id)] = payOnlinePassOutBack.order_id;
        //body gate_id
        root[common(body)][common(gate_id)] = payOnlinePassOutBack.gate_id;
        //body request_time
        root[common(body)][common(request_time)] = (double) payOnlinePassOutBack.request_time;
        //body out_time
        root[common(body)][common(out_time)] = (double) payOnlinePassOutBack.out_time;
        //body charge_secs
        root[common(body)][common(charge_secs)] = (double) payOnlinePassOutBack.charge_secs;

        //body charge_nu
        char charge_nu[20];
        snprintf(charge_nu, sizeof(charge_nu) - 1, "%.1f", payOnlinePassOutBack.charge_nu);
        root[common(body)][common(charge_nu)] = charge_nu;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_PayOnlineAutoEnd(PayOnlineAutoEnd_Req &payOnlineAutoReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(body)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        payOnlineAutoReq.msgid = root[common(msgid)].asInt();

        //body comm_id
        payOnlineAutoReq.comm_id = root[common(body)][common(comm_id)].asString();
        //body plate_nu
        payOnlineAutoReq.plate_nu = root[common(body)][common(plate_nu)].asString();
        //body order_id
        payOnlineAutoReq.order_id = root[common(body)][common(order_id)].asString();
        //body gate_id
        payOnlineAutoReq.gate_id = root[common(body)][common(gate_id)].asString();
        //body charge_nu
        payOnlineAutoReq.charge_nu = root[common(body)][common(charge_nu)].asDouble();
        //body request_time
        payOnlineAutoReq.request_time = root[common(body)][common(request_time)].asDouble();

        return 0;
    }

    int SetInfo_PayOnlineAutoEnd(string &json_str, PayOnlineAutoEnd_Req payOnlineAutoReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = payOnlineAutoReq.code;
        //root msgid
        root[common(msgid)] = payOnlineAutoReq.msgid;

        //body comm_id
        root[common(body)][common(comm_id)] = payOnlineAutoReq.comm_id;
        //body plate_nu
        root[common(body)][common(plate_nu)] = payOnlineAutoReq.plate_nu;
        //body order_id
        root[common(body)][common(order_id)] = payOnlineAutoReq.order_id;
        //body gate_id
        root[common(body)][common(gate_id)] = payOnlineAutoReq.gate_id;
        //body charge_nu
        root[common(body)][common(charge_nu)] = payOnlineAutoReq.charge_nu;
        //body request_time
        root[common(body)][common(request_time)] = (double) payOnlineAutoReq.request_time;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_RemoteLogIn_Req(RemoteLogin_Req &remoteLoginReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        remoteLoginReq.msgid = root[common(msgid)].asInt();

        //data username
        remoteLoginReq.username = root[common(data)][common(username)].asString();
        //data password
        remoteLoginReq.password = root[common(data)][common(password)].asString();

        return 0;
    }

    int SetInfo_RemoteLogIn_Req(string &json_str, RemoteLogin_Req remoteLoginReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = remoteLoginReq.code;
        //root msgid
        root[common(msgid)] = remoteLoginReq.msgid;

        //data username
        root[common(data)][common(username)] = remoteLoginReq.username;
        //data password
        root[common(data)][common(password)] = remoteLoginReq.password;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_Simple_Back(Simple_Back &simpleBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data msgid
        simpleBack.msgid = root[common(msgid)].asInt();

        return 0;
    }

    int SetInfo_Simple_Back(string &json_str, Simple_Back simpleBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = simpleBack.code;
        //root msgid
        root[common(msgid)] = simpleBack.msgid;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_RemoteAllCamera_Req(RemoteAllCamera_Req &remoteAllCameraReq, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }

        return 0;
    }

    int SetInfo_RemoteAllCamera_Req(string &json_str, RemoteAllCamera_Req remoteAllCameraReq) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = remoteAllCameraReq.code;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_RemoteAllCamera_Back(RemoteAllCamera_Back &remoteAllCameraBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }

        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        remoteAllCameraBack.msgid = root[common(msgid)].asInt();

        //data camera_array
        for (auto iter:root[common(data)][common(camera_array)]) {
            GateItem item;
            //camera_addr
            item.camera_addr = iter[common(camera_addr)].asString();
            //garage_id
            item.garage_id = iter[common(garage_id)].asString();
            //gate_id
            item.gate_id = iter[common(gate_id)].asString();
            //gate_name
            item.gate_name = iter[common(gate_name)].asString();
            //in_out_type
            item.in_out_type = iter[common(in_out_type)].asInt();
            //is_on_watch
            item.is_on_watch = iter[common(is_on_watch)].asInt();

            remoteAllCameraBack.camera_array.push_back(item);
        }

        remoteAllCameraBack.camera_array_len = root[common(data)][common(camera_array_len)].asInt();

        return 0;
    }

    int SetInfo_RemoteAllCamera_Back(string &json_str, RemoteAllCamera_Back remoteAllCameraBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = remoteAllCameraBack.code;
        //root msgid
        root[common(msgid)] = remoteAllCameraBack.msgid;

        //data camera_array
        Json::Value camera_array;
        for (auto iter:remoteAllCameraBack.camera_array) {
            Json::Value item;
            //camera_addr
            item[common(camera_addr)] = iter.camera_addr;
            //garage_id
            item[common(garage_id)] = iter.garage_id;
            //gate_id
            item[common(gate_id)] = iter.gate_id;
            //gate_name
            item[common(gate_name)] = iter.gate_name;
            //in_out_type
            item[common(in_out_type)] = iter.in_out_type;
            //is_on_watch
            item[common(is_on_watch)] = iter.is_on_watch;

            camera_array.append(item);
        }
        root[common(data)][common(camera_array)] = camera_array;

        root[common(data)][common(camera_array_len)] = remoteAllCameraBack.camera_array_len;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_RemoteGate_Req(RemoteGate_ReqBack &remoteGateReqBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }
        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        remoteGateReqBack.msgid = root[common(msgid)].asInt();

        //data camera_ip
        remoteGateReqBack.camera_ip = root[common(data)][common(camera_ip)].asString();
        //data car_num
        remoteGateReqBack.car_num = root[common(data)][common(car_num)].asString();
        //data show
        remoteGateReqBack.show = root[common(data)][common(show)].asString();
        //data car_type
        remoteGateReqBack.car_type = root[common(data)][common(car_type)].asString();
        //data display_name
        remoteGateReqBack.display_name = root[common(data)][common(display_name)].asString();
        //data parking_id
        remoteGateReqBack.parking_id = root[common(data)][common(parking_id)].asString();
        //data gate_id
        remoteGateReqBack.gate_id = root[common(data)][common(gate_id)].asString();
        //data bussiness_id
        remoteGateReqBack.bussiness_id = root[common(data)][common(bussiness_id)].asString();
        //data res_garage_id
        remoteGateReqBack.res_garage_id = root[common(data)][common(res_garage_id)].asString();
        //data target_garage_id
        remoteGateReqBack.target_garage_id = root[common(data)][common(target_garage_id)].asString();
        //data space_id
        remoteGateReqBack.space_id = root[common(data)][common(space_id)].asString();
        //data visitor_time
        remoteGateReqBack.visitor_time = root[common(data)][common(visitor_time)].asDouble();
        //data use_res_num
        remoteGateReqBack.use_res_num = root[common(data)][common(use_res_num)].asInt();
        //data car_type_id
        remoteGateReqBack.car_type_id = root[common(data)][common(car_type_id)].asInt();

        return 0;
    }

    int SetInfo_RemoteGate_Req(string &json_str, RemoteGate_ReqBack remoteGateReqBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = remoteGateReqBack.code;
        //root msgid
        root[common(msgid)] = remoteGateReqBack.msgid;

        //data camera_ip
        root[common(data)][common(camera_ip)] = remoteGateReqBack.camera_ip;
        //data car_num
        root[common(data)][common(car_num)] = remoteGateReqBack.car_num;
        //data show
        root[common(data)][common(show)] = remoteGateReqBack.show;
        //data car_type
        root[common(data)][common(car_type)] = remoteGateReqBack.car_type;
        //data display_name
        root[common(data)][common(display_name)] = remoteGateReqBack.display_name;
        //data parking_id
        root[common(data)][common(parking_id)] = remoteGateReqBack.parking_id;
        //data gate_id
        root[common(data)][common(gate_id)] = remoteGateReqBack.gate_id;
        //data bussiness_id
        root[common(data)][common(bussiness_id)] = remoteGateReqBack.bussiness_id;
        //data res_garage_id
        root[common(data)][common(res_garage_id)] = remoteGateReqBack.res_garage_id;
        //data target_garage_id
        root[common(data)][common(target_garage_id)] = remoteGateReqBack.target_garage_id;
        //data space_id
        root[common(data)][common(space_id)] = remoteGateReqBack.space_id;
        //data visitor_time
        root[common(data)][common(visitor_time)] = (double) remoteGateReqBack.visitor_time;
        //data use_res_num
        root[common(data)][common(use_res_num)] = remoteGateReqBack.use_res_num;
        //data car_type_id
        root[common(data)][common(car_type_id)] = remoteGateReqBack.car_type_id;

        json_str = fastWriter.write(root);

        return 0;
    }

    int GetInfo_RemoteGate_Back(RemoteGate_ReqBack &remoteGateReqBack, string json_str) {
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(json_str, root, false)) {
            cout << "not json drop" << endl;
            return -1;
        }

        //data
        if (!root[common(data)].isObject()) {
            cout << "json no data" << endl;
            return -1;
        }
        //root msgid
        remoteGateReqBack.msgid = root[common(msgid)].asInt();

        //data bussiness_id
        remoteGateReqBack.bussiness_id = root[common(data)][common(bussiness_id)].asString();
        //data camera_ip
        remoteGateReqBack.camera_ip = root[common(data)][common(camera_ip)].asString();
        //data camera_sn
        remoteGateReqBack.camera_sn = root[common(data)][common(camera_sn)].asString();
        //data car_num
        remoteGateReqBack.car_num = root[common(data)][common(car_num)].asString();
        //data car_owner_name
        remoteGateReqBack.car_owner_name = root[common(data)][common(car_owner_name)].asString();
        //data car_owner_phone
        remoteGateReqBack.car_owner_phone = root[common(data)][common(car_owner_phone)].asString();
        //data car_picture
        remoteGateReqBack.car_picture = root[common(data)][common(car_picture)].asString();
        //data car_type
        remoteGateReqBack.car_type = root[common(data)][common(car_type)].asString();
        //data display_name
        remoteGateReqBack.display_name = root[common(data)][common(display_name)].asString();
        //data gate_id
        remoteGateReqBack.gate_id = root[common(data)][common(gate_id)].asString();
        //data parking_id
        remoteGateReqBack.parking_id = root[common(data)][common(parking_id)].asString();
        //data res_garage_id
        remoteGateReqBack.res_garage_id = root[common(data)][common(res_garage_id)].asString();
        //data show
        remoteGateReqBack.show = root[common(data)][common(show)].asString();
        //data space_id
        remoteGateReqBack.space_id = root[common(data)][common(space_id)].asString();
        //data target_garage_id
        remoteGateReqBack.target_garage_id = root[common(data)][common(target_garage_id)].asString();
        //data voice
        remoteGateReqBack.voice = root[common(data)][common(voice)].asString();
        //data use_res_num
        remoteGateReqBack.use_res_num = root[common(data)][common(use_res_num)].asInt();
        //data visitor_time
        remoteGateReqBack.visitor_time = root[common(data)][common(visitor_time)].asDouble();
        //data car_type_id
        remoteGateReqBack.car_type_id = root[common(data)][common(car_type_id)].asInt();

        return 0;
    }


    int SetInfo_RemoteGate_Back(string &json_str, RemoteGate_ReqBack remoteGateReqBack) {
        Json::FastWriter fastWriter;
        Json::Value root;
        //root code
        root[common(code)] = remoteGateReqBack.code;
        //root msgid
        root[common(msgid)] = remoteGateReqBack.msgid;

        //data bussiness_id
        root[common(data)][common(bussiness_id)] = remoteGateReqBack.bussiness_id;
        //data camera_ip
        root[common(data)][common(camera_ip)] = remoteGateReqBack.camera_ip;
        //data camera_sn
        root[common(data)][common(camera_sn)] = remoteGateReqBack.camera_sn;
        //data car_num
        root[common(data)][common(car_num)] = remoteGateReqBack.car_num;
        //data car_owner_name
        root[common(data)][common(car_owner_name)] = remoteGateReqBack.car_owner_name;
        //data car_owner_phone
        root[common(data)][common(car_owner_phone)] = remoteGateReqBack.car_owner_phone;
        //data car_picture
        root[common(data)][common(car_picture)] = remoteGateReqBack.car_picture;
        //data car_type
        root[common(data)][common(car_type)] = remoteGateReqBack.car_type;
        //data display_name
        root[common(data)][common(display_name)] = remoteGateReqBack.display_name;
        //data gate_id
        root[common(data)][common(gate_id)] = remoteGateReqBack.gate_id;
        //data parking_id
        root[common(data)][common(parking_id)] = remoteGateReqBack.parking_id;
        //data res_garage_id
        root[common(data)][common(res_garage_id)] = remoteGateReqBack.res_garage_id;
        //data show
        root[common(data)][common(show)] = remoteGateReqBack.show;
        //data space_id
        root[common(data)][common(space_id)] = remoteGateReqBack.space_id;
        //data target_garage_id
        root[common(data)][common(target_garage_id)] = remoteGateReqBack.target_garage_id;
        //data voice
        root[common(data)][common(voice)] = remoteGateReqBack.voice;
        //data use_res_num
        root[common(data)][common(use_res_num)] = remoteGateReqBack.use_res_num;
        //data visitor_time
        root[common(data)][common(visitor_time)] = (double) remoteGateReqBack.visitor_time;
        //data car_type_id
        root[common(data)][common(car_type_id)] = remoteGateReqBack.car_type_id;

        json_str = fastWriter.write(root);

        return 0;
    }

}
