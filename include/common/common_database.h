//
// Created by root on 2021/3/31.
//

#ifndef MY_RPI_DOOR_COMMON_DATABASE_H
#define MY_RPI_DOOR_COMMON_DATABASE_H

#include "database/DataBase.h"
#include <string>
#include <time.h>
#include <vector>

using namespace std;

namespace common_database {

#define STR(p) (p == NULL ? (char *)"": p)

    //tables
    //子订单表
    struct DbTable_business_charge_child_order {
        string garage_id;//车库ID
        string child_order;//子订单
        string plate_nu;//车牌号
        long start_time;//订单开始时间
        long end_time;//订单结束时间
        double charge_nu;//产生的费用
        int charge_times; //计次卡应扣次数
        int car_type;//车辆类型
        int source_nu;//资源数量
        int is_open;//0关闭1打开
        string gate_id;//门岗id
        int del_flag;//是否标记已删除1已删除2未删除
        string total_order;//总订单号
        string res_garage_id;//身份所在车库ID
    };

//总订单表
    struct DbTable_business_charge_total_order {
        string total_order;//主订单
        string parking_id;//车场ID
        string plate_nu;//车牌号
        string gate_id;//门岗id
        int is_open;//0关闭1打开
        long start_time;//订单开始时间
        long end_time;//订单结束时间
        double charge_nu;//订单应缴费用
        int charge_times;//计次卡应扣次数
        string sell_reason;//优惠事由
        double front_charge_nu;//门岗收费金额
        double card_charge_nu;//储值卡缴费金额
        double cloud_charge_nu;//线上缴费金额
        double sell_nu;//优惠金额
        int del_flag;//是否标记已删除1已删除2未删除
        int is_exception;//是否异常
        long auto_insert_time;//记录生成时间
        long auto_update_time;//记录最后修改时间
        string guard_name;//门岗名字（只有门岗收费才写
        string cloud_order_no;//云端订单号
        int paid_times;//计次卡已扣次数
        long prepay_time;
    };

//在场车辆
    struct DbTable_car_in_parking {
        string parking_id;//车场ID
        string garage_id;//车库ID
        string plate_nu;//车牌号
        int car_type;//车辆类型
        int space_type;//车位类型
        string space_id;//车位ID
        string gate_id;//门岗ID
        int state;//车位状态:1空闲，2停放，3占用
        int is_lock;//锁车状态
        int is_in_parking;//是否在场:0不在场 >0在场
        long create_time;//创建时间
        long update_time;//更新时间
        int lock_type;//锁车类型1物业2业主
    };
//通行记录
    struct DbTable_car_through_log {
        string parking_id;//车场ID
        string garage_id;//车库ID
        string plate_nu;//车牌号
        string space_id;//车位ID
        int car_type;//车辆类型
        string camera_ip_addr;//设备的ip地址
        int in_out_type;//进出类型
        string car_pictour_addr;//车辆图片地址
        string gate_id;//门岗ID
        string led_display_info;//led显示信息
        string reason_id;//事由ID
        int gate_control;//道闸开启关闭状态
        int gate_way;//开闸方式（1.未抬杆2.收费抬杆3.异常收费抬杆4.自动识别抬杆5.远程抬杆6.手动抬杆
        int through_type;//进出类型(1 进2出)
        long scan_time;//扫描时间
        string car_owner_name;//车主姓名
        string car_owner_phone;//车主手机号
        string unusual_open_gate;//异常抬杆记录
        string bussiness_id;//商户id
        int lock_car;//0未锁车1锁车
        int led_car_type;//LED指向的身份
        int garage_mode;//车库类型
    };


    // way of open gate
//开闸方式（1.未抬杆2.收费抬杆3.异常收费抬杆4.自动识别抬杆5.远程抬杆6.手动抬杆90释放资源95占用资源）
    enum GATE_OPEN_WAY {
        GOW_NO = 1,
        GOW_CHARGE = 2,
        GOW_INVALID_CHARGE = 3,
        GOW_AUTO = 4,
        GOW_REMOTE = 5,
        GOW_FRONT_PASS_IN = 6,
        GOW_INVISIBLE_RELEASE = 90,
        GOW_INVISIBLE_USE = 95//TEMP RECORD
    };


    struct DbTable_card_used_record {
        string garage_id;//车库ID
        string plate_nu;//车牌号
        long start_time;//订单开始时间
        long end_time;//订单结束时间
        string card_id;//卡ID
        int card_type;//卡类型
        double use_money;//使用金额
        int use_times;//使用次数
        string total_order;//关联的总订单号
    };

//异常稽查
    struct DbTable_data_default_info {
        int terminal_id;//自增主键
        long action_time;//动作时间
        int action_id;//缺省ID
        int action_type;//缺省类型
    };

    struct DbTable_device_state_record {
        int terminal_id;//终端ID
        int device_type;//设备类型
        string camera_ip;//设备ip地址
        string camera_sn;//设备sn号
        int device_state;//设备状态
    };
//异常记录
    struct DbTable_fault_info {
        int terminal_id;//自增主键
        int fault_type;//错误类型
        long fault_time;//错误时间
    };
//停车收费规则
    struct DbTable_feescale_info {
        string garage_id;//车库ID
        int res_type;//资源类型（1固定2商户3弹性4临停）
        string name;//收费标准名称
        int type;//计费类型(0:按次收费;1:按时段收费;2:混合收费)
        string charge_id;//付费模板ID
        string cloud_id_uniq;//云端id，唯一ID
    };

    enum Charge_Type {
        CID_BY_NULL = 0,
        CID_BY_COUNT = 1,       //001
        CID_BY_TIME_INTERVAL,   //002
        // CID_BY_MINUTES,      //003
        CID_BY_MIX,             //004
        CID_BY_TIMES_CARD,      //000
        CID_BY_INTERVAL,
    };

    struct DbTable_front_charge_record {
        string gate_id;//门id
        double real_charge;//门岗实际收费
        double charge_nu;//应收总额
        double sell_nu;//优惠金额
        string sell_reason;//优惠事由
        long create_time;//收费时间
        double self_paid;//已缴金额
        string plate_nu;//车牌
    };

    struct DbTable_front_record {
        string record_no;//UUID唯一标识
        int in_out_type;//'1进2出',
        string plate_nu;//车牌
        string camera_ip;//摄像头IP
        int car_type;//车辆类型
        string gate_id;//门ID
        string bussiness_id;//商户id
        string space_id;//固定车位id
        string res_garage_id;//资源所在车库id
        string target_garage_id;//目标车库id
        int use_res_num;//资源使用数量
        string picture;//车辆图片oss相对地址
        string owner_name;//车主姓名
        string owner_phone;//车主手机
        string checkin_str;//入场排查
        int led_car_type;//led显示的车辆类型
        long visitor_time;//访客时间'
        string led;//led显示
        string voice;//声音播报
        double exception_price;//配置的异常缴费金额
        long in_time;//入场时间，字符串
        long out_time;//出场时间，字符串
        string stay_time;//停留时间，字符串
        string car_type_name;//车辆类型名称
        string car_state;//车辆状态
        double amount_due;//应缴金额
        double amount_paid;//已缴金额(在班次已收记录中为车主自缴金额)
        double amount_to_paid;//待缴金额(在班次已收记录中为门岗收费金额)
        double abnormal_amount;//异常收费金额
        string note;//备注
    };

//车库配置
    struct DbTable_garage_config {
        string garage_id;//车库ID
        string config;//配置值
        string value;//value值
    };
//车库信息
    struct DbTable_garage_info {
        string garage_id;//车库ID
        int garage_mode;//车库类型 1 住宅库 2 商住库 3 商停库
        int garage_level;//车库级别
        string parent_id;//父库ID
        string garage_name;//车库名称
    };

    enum LOGIC_GARAGE_MODE {
        LGM_OWNER = 1,
        LGM_BUSINESS = 3,
        LGM_MIX = 2,
    };

    struct DbTable_gate_record {
        string parking_id;//场ID
        string garage_id;//车库ID
        string gate_id;//门岗ID
        long sign_time;//门岗登录时间
        string user_name;//用户名
        string user_key;//密码
        int sign_state;//登录状态
    };


    struct DbTable_guard_info {
        string user_name;
        string user_key;
        int first_login;//标记位，是否第一次登陆
        string full_name;
    };

    struct DbTable_iot_owe_info {
        string plate_nu;//车牌
        string relate_name;//项目名
        int owe_days;//欠费天数（负数是还没欠费的天数，正数是已经欠费天数）
        int owe_state;//标记位，1欠费前2欠费后
        int control_type;//管控类型，1示警2管控
    };
//锁车信息
    struct DbTable_lock_info {
        string car_id;//车辆ID
        string plate_nu;//车牌号
        int lock_type;//锁车类型  1 临时锁车 2 定时锁车
        int lock_enable;//是否开启(1 开启 0 关闭)
        string lock_period;//锁车周期
        long lock_start_time;//锁车开始时间
        long lock_end_time;//锁车结束时间
        long unlock_finish_time;//开锁时效
        string uniq_id;//唯一标识，与云端的id对应，用于更新记录
    };
//线上支付
    struct DbTable_online_payment {
        int record_id;//订单ID
        string plate_number;//车牌号
        long charge_time;//缴费时间
        int charge_nu;//缴费金额
        char gate_id;//门岗ID
        int use_state;//
        string cloud_id;//
    };
//车场配置
    struct DbTable_parking_config {
        string parking_id;//车库ID
        string config;//配置值
        string value;//value值
        string community_id;//社区id
    };
//事由表
    struct DbTable_reason_open_discounts {
        string parking_id;//车场ID
        string reason_id;//事由ID
        int reason_type;//事由类型
        string reason_name;//事由名称
        string community_id;//社区id
    };
//临停弹性车位表
    struct DbTable_space_number {
        string parking_id;//车场ID
        string garage_id;//车库ID
        int flexible_total;//弹性车位数
        int temporary_total;//临停车位数
        int flexible_number;//剩余弹性车位数
        int temporary_number;//剩余临停车位数
    };
//异常表
    struct DbTable_suspect_violation_log {
        string community_id;//社区id
        string garage_id;//车库id
        string plate_number;//车牌号
        int violation_type;//疑似违规类型 1 违规抬杆 2 门岗漏费 3 超时停放
        string violation_reason;//疑似违规原因
        int violation_count;//疑似违规次数/超时停放天数
        double unpaid_cost;//疑似漏缴费用
        string violation_doorway_id;//疑似违规门口id
        string doorway_name;//疑似违规门口
        long enter_park_time;//车辆入库时间
        long violation_time;//车辆违规时间
        int car_type;//车辆类型（漏费专用）
        int is_in_parking;//是否在场（漏费专用）
        string total_order;//订单号（漏费专用，防重复的唯一标识）
    };

    struct DbTable_temp_charge_count_rules {
        string charge_id;//收费模板ID
        int charge_type;//收费类型（按次收费）
        string charge_name;//收费名称
        int free_time;//免费时长（分钟）
        int is_add_free_time;//免费时长是否计入停车时长
        double max_price;//当日封顶额度
        double price;//价格
        int is_repeat_across_day;//是否计算跨天费用
        double across_day_price;//跨天加收
        int day_type;//天的定义:1按日期，2按24小时',
    };

    struct DbTable_temp_charge_hybrid_basic {
        int major_id;
        string charge_id;//收费模板ID
        int charge_type;//收费类型（按次收费）
        string charge_name;//收费名称
        int free_time;//免费时长（分钟）
        int is_add_free_time;//免费时长是否计入停车时长
        double max_price;//当日封顶额度
        int is_limit_max_price;//是否限制最高收费
        double price;//单价（按次）',
    };


    struct DbTable_temp_charge_hybrid_rules {
        int major_id;
        string charge_id;//收费模板ID
        double max_price;//时段封顶额度
        int is_limit_max_price;//是否限制时段最高收费
        int overflow_minute;//超出时段后，分钟数(按时段)
        int type;//类型（1按时段，2按次）
        double overflow_price;//超出时段后，单价(按时段)
        int need_special_pay;//不满1小时按最后一个时段标准收费（按次）
        int free_in_or_out;//达到最高收费后该时段内随意进出(按次)
        int no_full_need_pay_for_time;//不满一个时段不收费(按时段)
        long start_time;//开始时段
        long end_time;//结束时段
        double price;//单价（按次）
        string cloud_id;//
    };

    struct DbTable_temp_charge_hybrid_rules_detail {
        int mix_id;//混合模板ID（major_id字段）
        int start_minutes;//开始时间段
        int end_minutes;//结束时间段
        int minutes;//单位分钟数
        double price;//单位分钟数价格
        string cloud_id;//
    };

    struct DbTable_temp_charge_tcard_rules {
        int free_time;//免费时长
        int is_repeat_across_day;//是否重复计算天数 1:是 0:否
        int is_repeat_across;//是否跨天加收1次:是 0:否
        int day_type;//跨天规则 1:超过23:59:59 2:累计24小时
        int calculation;//超出后计入停车时长 1计入 0 不计入
        double exception_price;//异常收费金额
        int uniq_id;//
    };

    struct DbTable_temp_charge_time_rules {
        string charge_id;//收费模板ID
        int charge_type;//收费类型（按次收费）
        string charge_name;//收费名称
        int free_time;//免费时长（分钟）
        int is_add_free_time;//免费时长是否计入停车时长
        double max_price;//当日封顶额度
        int overflow_hour;//超出小时数
        int overflow_minute_unit;//超出后时间单位制
        double overflow_minute_price;//超出后单价
        int no_full_need_pay;//不满一个时段是否计费
        int is_limit_max_price;//是否限制当日最高价
    };

    struct hour_price {
        int hours;
        double price;
    };


    struct DbTable_temp_charge_time_rules_detail {
        string charge_id;//付费模板ID
        int hours;//小时数
        double price;//价格
    };

    struct DbTable_terminal_info {
        string ticket;//ticket值
        int camera_number;//摄像头数量
        int display_number;//IP显示屏的数量
        string terminal_name;//终端名称
        string community_id;//社区id
    };

    struct DbTable_terminal_passing_record {
        string parking_id;//车场id
        string garage_id;//车库id
        string gate_id;//门岗id
        string plate_nu;//
        int group_id;//车辆类型
        string space_id;//车位信息
        string bussiness_id;//商户ID
        int resunum;//
        string res_garage_id;//
        long timestamp;//
        int through_type;//
        int situstion_type;//
        long visitor_time;//
        int charge_id;//
        string dev_sn;//
        string car_pic;//
        int is_open;//
        string car_owner_name;//车主姓名
        string car_owner_phone;//车主手机号
        string checkin_str;//入库信息缓存
        int led_car_type;//入库LED车辆类型
    };

    struct DbTable_upgrade_record {
        string new_version;//当前版本号
        string old_version;//上一个版本号
        int result;//升级结果
        string errmsg;//升级失败原因
        string id;//云端的ID
        long update_time;//升级时间
        long auto_cur_time;//
    };

    struct DbTable_upload_record {
        int type;//消息类型
        int state;//上传状态
        int key;//消息唯一ID
        string content;//消息json
        long scan_time;//
    };
    struct DbTable_v3_car_info {
        string id;//
        string commuityId;//
        string parkingRelationId;//
        string parkingRelationName;//
        string carId;//
        int carType;//
        string plateNumber;//
        long startDate;//
        long endDate;//
        double balance;//
        string carOwnerName;//
        string carOwnerPhone;//
        long createTime;//
    };

    struct DbTable_v3_device_info {
        string id;//
        string communityId;//
        string garageId;//
        string doorwayId;//
        int throughType;//1入口2直接出社区3直接入社区
        string fatherGarageId;//
        string deviceSn;//
        string ipAddr;//
        string webcamLed;//
    };

    struct DbTable_v3_door_way {
        string id;//
        string communityId;//
        string doorwayName;//
        int doorwayType;//
    };

    struct DbTable_v3_garage_with_fix_space {
        string space_id;//
        string garage_id;//
    };

    struct DbTable_v3_garage_with_merchant {
        string space_id;//
        string garage_id;//
    };

    struct DbTable_v3_parking_space_fix {
        string id;//
        string communityId;//
        long startDate;//
        long endDate;//
        string spaceName;//
        string ownerName;//
        string ownerPhone;//
        int activationState;//
    };

    struct DbTable_v3_parking_space_merchant {
        string id;//
        string communityId;//
        string merchantName;//
        int spaceNum;//
        int lastNum;//
        long startDate;//
        long endDate;//
    };
    //views
    struct DbView_bussiness_space {
        int parking_id;
        string garage_id;
        string bussiness_id;
        long start_service;
        long end_service;
        int last_number;
        int all_number;
        string bussiness_name;
    };

    struct DbView_car_type {
        string parking_id;
        string garage_id;
        string plate_nu;
        int car_type;
        string car_type_name;
        string plate_id;
        long visitor_time;
        long start_service;
        long end_service;
        string space_id;
        string bussiness_id;
        long bind_time;
        string car_owner_name;
        string car_owner_phone;
        string cloud_id;
    };
    enum CarType_Local {
        CarType_Black = 0,//黑名单
        CarType_White = 1,//白名单
        CarType_Owner = 2,//业主
        CarType_Visitor = 3,//访客
        CarType_Elastic = 4,//弹性
        CarType_Business = 5,//商户
        CarType_SuperClockCard = 6,//计时
        CarType_TimesCard = 7,//计次
        CarType_MoneyCard = 8,//储值卡
        CarType_Social = 9,//社会
        CarType_Count,
    };

    enum CarType_Cloud {
        CarTypeCloud_Black = 7,//黑名单
        CarTypeCloud_White = 6,//白名单
        CarTypeCloud_Owner = 1,//业主
        CarTypeCloud_Visitor = 4,//访客
        CarTypeCloud_Elastic = 3,//弹性
        CarTypeCloud_Business = 2,//商户
        CarTypeCloud_SuperClockCard = 8,//计时
        CarTypeCloud_TimesCard = 9,//计次
        CarTypeCloud_MoneyCard = 10,//储值卡
        CarTypeCloud_Social = 5,//社会
    };


    struct DbView_card_info {
        string card_name;
        string card_id;
        int card_type;
        string plate_nu;
        long times_last;
        double money_last;
        long start_time;
        long end_time;
    };

#define CARTYPE_LOCAL 0
#ifdef CARTYPE_LOCAL
    enum CARD_TYPE {
        CT_TIMES = 1,
        CT_CLOCK = 2,
        CT_MONEY = 3
    };
#else
    enum CARD_TYPE
    {
        CT_CLOCK = 70,
        CT_TIMES = 71,
        CT_MONEY = 72
    };
#endif


    struct DbView_device_info {
        int parking_id;
        string garage_id;
        string gate_id;
        string camera_ip_addr;
        string camera_factory_string;
        int camera_factory;
        string display_ip_addr;
        string display_factory_string;
        int display_factory;
        string sn;
        int in_out_type;
        int is_on_watch;
        string father_garage_id;
        int garage_level;
        int through_out_type;
        string device_id;
    };
    enum CameraBrand {
        CameraZhenshi = 1,
        CameraZhixin = 2,
        CameraZhendi = 3,
        CameraDahua = 4,
        CameraHik = 5
    };

    struct DbView_fixed_space {
        string garage_id;
        string space_id;
        string space_name;
        string user_name;
        string user_phone_number;
        time_t start_service;
        time_t end_service;
        int activation_state;
    };

    struct DbView_gate_info {
        string garage_id;
        string gate_id;
        int is_on_watch;
        string gate_name;
        string user_name;
        string user_key;
        int sign_in;
    };

    enum Is_On_Watch {
        NULL_Watch = 0,
        On_Watch = 1,
        Off_Watch = 2,
    };

    struct DbView_view_car_in_out {
        string plate_nu;
        int car_type;
        string garage_id;
        int through_type;
        long scan_time;
        string space_id;
        int state;
        int is_in_parking;
        double counts_last;
        double money_last;
        string cloud_id;
        int time_seconds_last;
        int in_parking_car_number;
    };

    struct DbView_view_car_in_out_1002 {
        int gate_way;
        string plate_nu;
        int car_type;
        string car_type_name;
        string garage_id;
        string garage_name;
        int through_type;//进出类型
        int state;//固定车位状态
        int valid;
        int is_in_parking;
        int in_parking_car_number;
        string space_id;
        int counts_last;
        double money_last;
        string cloud_id;
        int time_seconds_last;
        time_t scan_time;
    };

    struct DbView_view_car_in_parking_v2 {
        string parking_id;
        string garage_id;
        string garage_name;
        string parent_id;
        string plate_nu;
        int car_type;
        string space_id;
        int is_in_parking;
    };

    struct DbView_view_car_through_log {
        string parking_id;
        string garage_id;
        string plate_nu;
        string space_id;
        int car_type;
        string camera_ip_addr;
        int in_out_type;
        string car_pictour_addr;
        string gate_id;
        string led_display_info;
        string reason_id;
        int gate_control;
        int gate_way;
        int through_type;
        long scan_time;
        string car_owner_name;
        string car_owner_phone;
        string unusual_open_gate;
        string bussiness_id;
        int lock_car;
        int led_car_type;
        int garage_mode;
        string garage_name;
        string gate_name;
    };

    struct DbView_view_car_through_log_1001 {
        string parking_id;
        string garage_id;
        string garage_name;
        string gate_name;
        string plate_nu;
        string space_id;
        int car_type;
        string camera_ip_addr;
        int in_out_type;
        string car_pictour_addr;
        string gate_id;
        string led_display_info;
        string reason_id;
        int gate_control;
        int gate_way;
        int through_type;
        long scan_time;
        string car_owner_name;
        string car_owner_phone;
        string unusual_open_gate;
        string bussiness_id;
        int lock_car;
        int led_car_type;
        int garage_mode;
    };

    struct DbView_view_space_garage {
        string space_name;
        string space_id;
        string garage_name;
        string garage_id;
        string type;
    };


    typedef struct {
        string parking_id;
        string garage_id;
        string gate_id;
        string gate_name;
        string user_name;
        string user_passwd;
    } ExtraDataFront;

    typedef struct {
        string parking_id;
        string gate_id;
        string garage_id;
        string display_name;
        string sn;
        string ip_addr;
        string camera_name;
        int in_out_type;
        int is_on_watch;
        int camera_factory;
        int display_factory;
        int through_out_type;
    } ExtraDataDevice;

    enum LogicDirection {
        LDIR_IN = 1,
        LDIR_OUT = 2,
    };

    enum Through_out_Type {
        Out_To_Street = 2,
        In_From_Street = 3,
    };

    char *GetInitKeyString(char *title, char *key, const char *filename);

#define LOAD_INI_ATTR(title, key, filename, defaultstr) \
({\
    char exedir[100];\
    getcwd(exedir, 100);\
    char filepath[200];\
    snprintf(filepath, 200, "%s/%s", exedir, filename);\
    char* valuestr = GetIniKeyString(title,key,filepath);\
    strlen(valuestr) > 0 ? valuestr : defaultstr;\
})
#define OneDaySeconds (60*60*24)

#define SqlCmdLen (1024 * 2)

    int convert_timestr_to_stamp(string value);

    long convert_datetime_to_stamp(string value, string fmt);

    string convert_stamp_to_str(long timestamp, string fmt = "%Y-%m-%d %H:%M:%S");

    string convert_interval_to_str(long start, long end);

    string convert_interval_to_daystr(long start, long end);

    string convert_integer_to_voice(int val);

    string convert_double_to_voice(double val);

    int convert_interval_to_days(long start, long end);

    bool is_debug();


    /*
     * 将数据库操作函数进行分类整理，除了可用让代码整洁干净外，
     * 还有一个特殊的作用，就是可以看看那些表缺失操作，比如缺失删除操作的，一般肯定有历史数据存在，产生数据积压
     * */
    //S=select U=update D=delete I=insert
    //F=from W=where SET=set

    string S_parking_id(DataBase *dataBase);

    //DbTable_business_charge_child_order
    //select
    int S_business_charge_child_order_all_W_total_order_type(DataBase *dataBase,
                                                             vector<DbTable_business_charge_child_order> &vector_child_order,
                                                             string total_order, int type);

    //insert
    int I_business_charge_child_order(DataBase *dataBase,
                                      DbTable_business_charge_child_order childOrder);

    //update
    int U_business_charge_child_order_W_child_order(DataBase *dataBase,
                                                    DbTable_business_charge_child_order childOrder,
                                                    string child_order);

    //update
    int U_business_charge_child_order_SET_del_flag_W_plate_nu(DataBase *dataBase,
                                                              string plate_nu, int del_flag);

    //update
    int U_business_charge_child_order_SET_charge_times_charge_nu_W_child_order(DataBase *dataBase,
                                                                               string child_order,
                                                                               int charge_times,
                                                                               double charge_nu);

    //DbTable_business_charge_total_order
    //select
    int S_business_charge_total_order_W_total_order(DataBase *dataBase,
                                                    DbTable_business_charge_total_order &totalOrder,
                                                    string total_order);

    //select
    int S_total_order_F_business_charge_total_order_W_plate_nu(DataBase *dataBase,
                                                               string &total_order,
                                                               string plate_nu);

    //select
    int S_start_time_end_time_F_business_charge_total_order_W_total_order(DataBase *dataBase,
                                                                          long &start_time, long &end_time,
                                                                          string total_order);

    //select
    int S_SUM_charge_nu_F_business_charge_total_order(DataBase *dataBase,
                                                      double &charge_nu_sum,
                                                      string plate_nu, long start_time, long end_time);

    //insert
    int I_business_charge_total_order(DataBase *dataBase,
                                      DbTable_business_charge_total_order totalOrder);

    //update
    int U_business_charge_total_order_SET_del_flag_W_plate_nu(DataBase *dataBase,
                                                              string plate_nu, int del_flag);

    //update
    int U_business_charge_total_order_SET_end_time_W_plate_nu(DataBase *dataBase,
                                                              string plate_nu, long end_time);

    //update
    int U_business_charge_total_order_SET_is_open_W_total_order(DataBase *dataBase,
                                                                string total_order,
                                                                int is_open);

    //update
    int U_business_charge_total_order_SET_prepay_time_W_total_order(DataBase *dataBase,
                                                                    string total_order,
                                                                    long prepay_time);

    //update
    int U_business_charge_total_order_SET_guard_name_W_total_order(DataBase *dataBase,
                                                                   string guard_name, string total_order);

    //update
    int U_business_charge_total_order_fee(DataBase *dataBase,
                                          string total_order, double cost, int charge_type, string reason,
                                          bool is_exception = false,
                                          string cloud_order_no = "");

    //DbTable_car_in_parking
    //select
    int S_is_in_parking_F_car_in_parking_W_plate_nu(DataBase *dataBase,
                                                    int &is_in_parking,
                                                    string plate_nu);

    //select
    int S_is_in_parking_state_plate_nu_F_car_in_parking_W_space_id(DataBase *dataBase,
                                                                   int &is_in_parking, int &state, string &plate_nu,
                                                                   string space_id);

    //select
    int S_update_time_F_car_in_parking_W_plate_nu(DataBase *dataBase,
                                                  long &update_time,
                                                  string plate_nu);

    //select
    int S_car_type_F_car_in_parking_W_plate_nu(DataBase *dataBase,
                                               int &car_type,
                                               string plate_nu);

    //select
    int S_space_id_F_car_in_parking_W_plate_nu(DataBase *dataBase,
                                               string &space_id,
                                               string plate_nu);

    //select
    int S_plate_nu_F_car_in_parking_W_space_id(DataBase *dataBase,
                                               string &plate_nu,
                                               string space_id);

    //insert
    int I_car_in_parking(DataBase *dataBase,
                         DbTable_car_in_parking carInParking);

    //update
    int U_car_in_parking_SET_state_W_space_id_plate_nu(DataBase *dataBase,
                                                       string plate_nu, string space_id, int status);

    //update
    int U_car_in_parking_SET_is_in_parking_W_garage_id_plate_nu(DataBase *dataBase,
                                                                string plate_nu, string garage_id, int is_out,
                                                                int space_state, long timestamp);


    //DbTable_car_through_log
    //select
    int S_car_through_log_W_gate_id_through_type(DataBase *dataBase,
                                                 DbTable_car_through_log &carThroughLog,
                                                 string gate_id, int through_type);

    //select
    int S_car_through_log_W_plate_nu_gate_way(DataBase *dataBase,
                                              DbTable_car_through_log &carThroughLog,
                                              string plate_nu, int gate_way);

    //select
    int S_led_display_info_unusual_open_gate_F_car_through_log_W_plate_nu(DataBase *dataBase,
                                                                          string &led_display_info,
                                                                          string &unusual_open_gate,
                                                                          string plate_nu);

    //insert
    int I_car_through_log(DataBase *dataBase,
                          DbTable_car_through_log carThroughLog);

    //delete
    int D_car_through_log_W_plate_nu(DataBase *dataBase,
                                     string plate_nu);


    //DbTable_card_used_record
    //insert
    int I_card_used_record(DataBase *dataBase, DbTable_card_used_record cardUsedRecord);

    //DbTable_feescale_info
    //select
    int S_type_F_feescale_info_W_garage_id_res_type(DataBase *dataBase,
                                                    int &type,
                                                    string garage_id, int res_type);

    //select
    int S_charge_id_F_feescale_info_W_garage_id_res_type(DataBase *dataBase,
                                                         string &charge_id,
                                                         string garage_id, int res_type);


    //DbTable_front_charge_record
    //select
    int S_front_charge_record_allless_W_gate_id_time_interval(DataBase *dataBase,
                                                              vector<DbTable_front_charge_record> &vector_front_charge_record,
                                                              int &total, string gate_id, string gate_ids_str,
                                                              long start_time, long end_time, int start_line,
                                                              int lines);

    //select
    int S_real_charge_F_front_charge_record_W_gate_id_time_interval(DataBase *dataBase,
                                                                    double &money,
                                                                    string gate_id, string gate_ids_str,
                                                                    long start_time, long end_time);

    //insert
    int I_front_charge_record_append(DataBase *dataBase,
                                     DbTable_front_charge_record frontChargeRecord);

    //DbTable_front_record
    //select
    int S_front_record_all_W_record_no(DataBase *dataBase,
                                       vector<DbTable_front_record> &vector_frontRecord,
                                       string record_no);

    //select
    int S_front_record_in_W_plate_nu(DataBase *dataBase,
                                     DbTable_front_record &frontRecord,
                                     string plate_nu);

    //insert
    int I_front_record_in(DataBase *dataBase,
                          DbTable_front_record frontRecord);

    //insert
    int I_front_record_out(DataBase *dataBase,
                           DbTable_front_record frontRecord);

    //DbTable_garage_info
    //select
    int S_garage_info_all(DataBase *dataBase,
                          vector<DbTable_garage_info> &vector_garageInfo);

    //select
    int S_garage_mode_F_garage_info_W_garage_id(DataBase *dataBase,
                                                int &garage_mode,
                                                string garage_id);

    //DbTable_gate_record
    //select
    int S_user_name_F_gate_record_W_gate_id(DataBase *dataBase,
                                            string &user_name,
                                            string gate_id);

    //select
    int S_sign_time_F_gate_record_W_gate_id_user_name(DataBase *dataBase,
                                                      long &sign_time,
                                                      string gate_id, string user_name);

    //insert
    int I_gate_record(DataBase *dataBase,
                      DbTable_gate_record gateRecord);

    //update
    int U_gate_record_SET_sign_state_W_user_name_gate_id(DataBase *dataBase,
                                                         int sign_state, string user_name, string gate_id);

    //DbTable_guard_info
    //select
    bool S_user_name_F_guard_info_exist(DataBase *dataBase,
                                        string user_name);

    //select
    int S_user_key_F_guard_info_W_user_name(DataBase *dataBase, string &user_key, string user_name);

    //select
    int U_guard_info_SET_user_key_W_user_name(DataBase *dataBase,
                                              string user_name, string user_key);

    //select
    int S_full_name_F_guard_info_W_user_name(DataBase *dataBase,
                                             string &full_name,
                                             string user_name);

    //DbTable_iot_owe_info
    //select
    int S_iot_owe_info_W_plate_nu_control_type(DataBase *dataBase,
                                               DbTable_iot_owe_info &iotOweInfo,
                                               string plate_nu, int control_type = 1);

    //select
    int S_owe_days_relate_name_F_iot_owe_info_W_plate_nu_control_type(DataBase *dataBase,
                                                                      int &owe_days,
                                                                      string &relate_name,
                                                                      string plate_nu,
                                                                      int control_type = 2);

    //DbTable_lock_info
    //select
    int S_lock_info_all_W_plate_nu_lock_type_lock_enable(DataBase *dataBase,
                                                         DbTable_lock_info &lockInfo,
                                                         string plate_nu,
                                                         int lock_type = 1, int lock_enable = 1);


    //DbTable_parking_config
    //select
    int S_community_id_F_parking_config(DataBase *dataBase,
                                        string &community_id);

    //select
    int S_value_F_parking_config_W_config(DataBase *dataBase,
                                          string &value,
                                          string config);

    //DbTable_reason_open_discounts
    //select
    int S_reason_open_discounts_all(DataBase *dataBase,
                                    vector<DbTable_reason_open_discounts> &vector_reasonOpenDiscounts,
                                    string parking_id = "100000");

    //DbTable_space_number
    //update
    int U_space_number_SET_flexible_number_W_garage_id(DataBase *dataBase,
                                                       string garage_id, int num);

    //update
    int U_space_number_SET_temporary_number_W_garage_id(DataBase *dataBase,
                                                        string garage_id, int num);

    //update
    int S_flexible_number_F_space_number_W_garage_id(DataBase *dataBase,
                                                     int &flexible_number,
                                                     string garage_id);

    //update
    int S_temporary_number_F_space_number_W_garage_id(DataBase *dataBase,
                                                      int &temporary_number,
                                                      string garage_id);

    //DbTable_temp_charge_count_rules
    //select
    int S_temp_charge_count_rules_W_charge_id(DataBase *dataBase,
                                              DbTable_temp_charge_count_rules &tempChargeCountRules,
                                              string charge_id);

    //DbTable_temp_charge_hybrid_basic
    //select
    int S_temp_charge_hybrid_basic_W_charge_id(DataBase *dataBase,
                                               DbTable_temp_charge_hybrid_basic &tempChargeHybridBasic,
                                               string charge_id);

    //DbTable_temp_charge_hybrid_rules
    //select
    int S_temp_charge_hybrid_rules_W_charge_id_type(DataBase *dataBase,
                                                    vector<DbTable_temp_charge_hybrid_rules> &vector_temp_charge_hybrid_rules,
                                                    string charge_id, int type);

    //DbTable_temp_charge_hybrid_rules_detail
    //select
    int S_temp_charge_hybrid_rules_detail_W_mix_id(DataBase *dataBase,
                                                   vector<DbTable_temp_charge_hybrid_rules_detail> &vector_temp_charge_hybrid_rules_detail,
                                                   int mix_id);

    //DbTable_temp_charge_tcard_rules
    //select
    int S_exception_price_F_temp_charge_tcard_rules(DataBase *dataBase,
                                                    double &exception_price);

    //select
    int S_temp_charge_tcard_rules(DataBase *dataBase,
                                  DbTable_temp_charge_tcard_rules &tempChargeTcardRules);


    //DbTable_temp_charge_time_rules
    //select
    int S_temp_charge_time_rules_W_charge_id(DataBase *dataBase,
                                             DbTable_temp_charge_time_rules &tempChargeTimeRules,
                                             string charge_id);

    //DbTable_temp_charge_time_rules_detail
    //select
    int S_hour_price_F_temp_charge_time_rules_detail_W_charge_id(DataBase *dataBase,
                                                                 vector<hour_price> &vector_hour_price,
                                                                 string charge_id);

    //DbTable_terminal_info
    //select
    int S_terminal_info(DataBase *dataBase,
                        DbTable_terminal_info &terminalInfo);

    //DbTable_terminal_passing_record
    //select
    bool S_plate_nu_F_terminal_passing_record_exist(DataBase *dataBase,
                                                    string &plate_nu_find,
                                                    string plate_nu, bool use_like);

    //select
    int S_terminal_passing_record_recent_W_plate_nu(DataBase *dataBase,
                                                    DbTable_terminal_passing_record &terminalPassingRecord,
                                                    string plate_nu);

    //insert
    int I_terminal_passing_record(DataBase *dataBase,
                                  DbTable_terminal_passing_record terminalPassingRecord);

    //delete
    int D_terminal_passing_record_W_plate_nu(DataBase *dataBase,
                                             string plate_nu);

    //delete
    int D_terminal_passing_record_W_garage_id_plate_nu(DataBase *dataBase,
                                                       string garage_id, string plate_nu);

    //DbView_bussiness_space
    //select
    int S_last_number_F_bussiness_space_W_garage_id_bussiness_id(DataBase *dataBase,
                                                                 int &last_number,
                                                                 string garage_id, string business_id);

    //select
    int S_serviceTime_F_bussiness_space_W_garage_id_bussiness_id(DataBase *dataBase,
                                                                 long &start_time, long &end_time,
                                                                 string garage_id, string plate_nu, string business_id);

    //update
    int U_bussiness_space_SET_last_number_W_garage_id_business_id(DataBase *dataBase,
                                                                  string garage_id, string business_id, int num);

    //DbView_car_type
    //select
    bool S_plate_nu_F_car_type_exsit(DataBase *dataBase,
                                     string &plate_nu_find,
                                     string plate_nu, bool use_like);

    //select
    int S_car_type_all_W_plate_nu(DataBase *dataBase,
                                  vector<DbView_car_type> &vector_carType,
                                  string plate_nu);

    //select
    bool S_plate_nu_space_id_garage_id_F_car_type_exist(DataBase *dataBase,
                                                        string plate_nu, string space_id, string garage_id);

    //select
    int S_serviceTime_F_car_type_W_garage_id_plate_nu_car_type(DataBase *dataBase,
                                                               long &start_time, long &end_time,
                                                               string garage_id, string plate_nu, int car_type);

    //select
    int S_serviceTime_F_car_type_W_space_id(DataBase *dataBase,
                                            long &start_time, long &end_time,
                                            string garage_id, string plate_nu, string space_id);

    //select
    int S_serviceTime_F_car_type_W_garage_id_plate_nu_Elastic(DataBase *dataBase,
                                                              long &start_time, long &end_time,
                                                              string garage_id, string plate_nu);

    //DbView_card_info
    //select
    int S_card_info_all_W_plate_nu(DataBase *dataBase,
                                   vector<DbView_card_info> &vector_card_info,
                                   string plate_nu);

    //select
    int S_times_last_F_card_info_W_card_id(DataBase *dataBase,
                                           int &times_last,
                                           string card_id);

    //select
    int S_money_last_F_card_info_W_card_id(DataBase *dataBase,
                                           double &money_last,
                                           string card_id);

    //update
    int U_card_info_SET_times_last_W_card_id(DataBase *dataBase,
                                             string card_id, int value);

    //update
    int U_card_info_SET_money_last_W_card_id(DataBase *dataBase,
                                             string card_id, double value);

    //DbView_device_info
    //select
    int S_device_info_all(DataBase *dataBase,
                          vector<DbView_device_info> &vector_deviceInfo);

    //select
    int S_in_out_type_F_device_info_W_camera_ip_addr(DataBase *dataBase,
                                                     int &in_out_type,
                                                     string camera_ip_addr);

    //DbView_fixed_space
    //select
    int S_activation_state_F_fixed_space_W_garage_id_space_id(DataBase *dataBase,
                                                              int &activation_state,
                                                              string garage_id, string space_id);

    //select
    int S_user_name_user_phone_F_fixed_space_W_space_id(DataBase *dataBase,
                                                        string &user_name, string &user_phone,
                                                        string space_id);

    //select
    int S_space_name_F_fixed_space_W_space_id(DataBase *dataBase,
                                              string &space_name,
                                              string space_id);

    //DbView_gate_info
    //select
    int S_gate_info_all(DataBase *dataBase,
                        vector<DbView_gate_info> &vector_gateInfo);

    //select
    bool S_gate_id_garage_id_F_gate_info_exist(DataBase *dataBase,
                                               string gate_id, string garage_id);

    //select
    int S_garage_id_F_gate_info_W_gate_id(DataBase *dataBase,
                                          string &garage_id,
                                          string gate_id);

}

#endif //MY_RPI_DOOR_COMMON_DATABASE_H
