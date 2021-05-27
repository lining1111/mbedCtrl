//
// Created by root on 2021/3/31.
//

#include <cstring>
#include <unistd.h>
#include "common/common_database.h"

namespace common_database {


    char *GetInitKeyString(char *title, char *key, const char *filename) {
        if (strlen(key) <= 0) {
            return (char *) "";
        }

        FILE *fp;
        char szLine[1024];
        char tmpstr[1024];
        int rtnval;
        int i = 0;
        int flag = 0;
        char *tmp;

        if ((fp = fopen(filename, "r")) == nullptr) {
            printf("Have  no  such  file:%s\n", filename);
            return (char *) "";
        }
        while (!feof(fp)) {
            rtnval = fgetc(fp);
            if (rtnval == EOF)
                break;
            else
                szLine[i++] = rtnval;
            if (rtnval == '\n') {
                i--;
                szLine[i] = '\0';
                i = 0;
                tmp = strchr(szLine, '=');
                if ((tmp != nullptr) && (flag == 1)) {
                    strcpy(tmpstr, key);
                    strcat(tmpstr, "=");
                    if (strstr(szLine, tmpstr) != nullptr) {
                        //注释行
                        if ('#' == szLine[0] || ('/' == szLine[0] && '/' == szLine[1])) {
                        } else {
                            //key对应变量
                            strcpy(tmpstr, tmp + 1);
                            fclose(fp);
                            return tmpstr;
                        }
                    }
                } else if (strlen(title) <= 0) {
                    flag = 1;
                } else {
                    strcpy(tmpstr, "[");
                    strcat(tmpstr, title);
                    strcat(tmpstr, "]");
                    if (strncmp(tmpstr, szLine, strlen(tmpstr)) == 0) //找到title
                        flag = 1;
                }
            }
        }
        fclose(fp);
        return (char *) "";
    }

    int convert_timestr_to_stamp(string value) {
        int len = value.length();
        if (len <= 0) {
            return 0;
        }
        char value_str[64];
        bzero(value_str, sizeof(value_str));
        memcpy(value_str, value.c_str(), value.size());
        char cache[20];
        char *laststr = cache;
        char *token = strtok_r(value_str, ":", &laststr);
        int cnt = 2;
        int result = 0;
        while (token != nullptr) {
            if (cnt == 2) {
                result += atoi(token) * 60 * 60;
            } else if (cnt == 1) {
                result += atoi(token) * 60;
            } else {
                result += atoi(token);
            }

            token = strtok_r(nullptr, ":", &laststr);
            cnt--;
        }
        return result;
    }

    long convert_datetime_to_stamp(string value, string fmt) {
        int len = value.length();
        if (len <= 0) {
            return 0;
        }
        struct tm _tm;
        strptime(value.c_str(), fmt.c_str(), &_tm);
        time_t t = mktime(&_tm);
        return t;
    }

    string convert_stamp_to_str(long timestamp, string fmt) {
        string timestr;
        if (timestamp == 0) {
            timestr = "";
            return timestr;
        }
        struct tm *p = localtime(&timestamp);
        char str[100];
        bzero(str, sizeof(str));
        strftime(str, 100, fmt.c_str(), p);
        timestr = str;
        return timestr;
    }

    string convert_interval_to_str(long start, long end) {
        string timestr;
        if (start <= 0 || end <= 0 || end < start) {
            timestr = "";
            return timestr;
        }
        int dis = end - start;
        int _24hours = 24 * 3600;
        int days = dis / _24hours;
        int hours = (dis - days * _24hours) / 3600;
        int minutes = (dis - days * _24hours - hours * 3600) / 60;
        int seconds = dis - days * _24hours - hours * 3600 - minutes * 60;

        char str[100];
        bzero(str, sizeof(str));
        if (days != 0) {
            snprintf(str, 100, "%d天%d时%d分%d秒", days, hours, minutes, seconds);
        } else if (days == 0 && hours != 0) {
            snprintf(str, 100, "%d时%d分%d秒", hours, minutes, seconds);
        } else if (days == 0 && hours == 0 && minutes != 0) {
            snprintf(str, 100, "%d分%d秒", minutes, seconds);
        } else {
            snprintf(str, 100, "%d秒", seconds);
        }
        timestr = str;
        return timestr;
    }

    string convert_interval_to_daystr(long start, long end) {
        string daystr;
        if (start <= 0 || end <= 0 || end < start) {
            daystr = "";
            return daystr;
        }
        int dis = end - start;
        int _24hours = 24 * 3600;
        int days = dis / _24hours;
        days = dis % _24hours == 0 ? days : days + 1;
        char str[100];
        bzero(str, sizeof(str));
        snprintf(str, 100, "%d天", days);
        daystr = str;
        return daystr;
    }

    string convert_integer_to_voice(int val) {
        string val_str;
        char valstr[100];
        bzero(valstr, sizeof(valstr));
        const char *unit[5] = {"万", "千", "百", "十", ""};
        int iunit[5] = {10000, 1000, 100, 10, 1};
        int num[5] = {0, 0, 0, 0, 0};
        int curr = val;
        int pointer = 0;
        bool zero = false;
        for (int i = 0; i < 5; i++) {
            num[i] = curr / iunit[i];
            curr -= num[i] * iunit[i];
            if (num[i] != 0 || i == 4) {
                if (pointer == 0 && strcmp(unit[i], "十") == 0 && num[i] == 1) {
                    snprintf(valstr + pointer, 100 - pointer, "%s", unit[i]);
                } else if (i == 4 && num[i] == 0) {
                    continue;
                } else {
                    snprintf(valstr + pointer, 100 - pointer, "%d%s", num[i], unit[i]);
                }
                pointer = strlen(valstr);
                zero = false;
            } else {
                if (pointer > 0 && !zero) {
                    snprintf(valstr + pointer, 100 - pointer, "%s", "零");
                    pointer = strlen(valstr);
                    zero = true;
                }
            }
            if (curr <= 0) {
                break;
            }
        }
        val_str = valstr;

        return val_str;
    }

    string convert_double_to_voice(double val) {
        string val_str;
        int mod = ((int) (val * 10)) % 10;
        int interger = (int) val;
        string interger_str = convert_integer_to_voice(interger);
        if (mod == 0) {
            val_str = interger_str;
        } else {
            int dot_val = (val - interger) * 10;
            val_str = interger_str + "点" + to_string(dot_val);
        }
        return val_str;
    }


    int convert_interval_to_days(long start, long end) {
        if (start <= 0 || end <= 0 || end < start) {
            return 0;
        }
        int dis = end - start;
        int _24hours = 24 * 3600;
        int days = dis / _24hours;
        days = dis % _24hours == 0 ? days : days + 1;
        return days;
    }


#define IO_LIMIT

    bool is_debug() {
#ifdef IO_LIMIT
        return false;
#endif
        char exedir[100];
        getcwd(exedir, 100);
        char filepath[200];
        snprintf(filepath, 200, "%s/tmp_test.txt", exedir);
        if ((access(filepath, F_OK)) != -1) {
            printf("文件 tmp_test.txt 存在, 使用调试模式，不写订单结束时间.\n");
            return true;
        }
        printf("文件 tmp_test.txt不存在, 正常落库.\n");
        return false;
    }

    string S_parking_id(DataBase *dataBase) {
        return "100000";
    }

    int S_business_charge_child_order_all_W_total_order_type(DataBase *dataBase,
                                                             vector<DbTable_business_charge_child_order> &vector_child_order,
                                                             string total_order, int type) {
        if (total_order.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        switch (type) {
            case 0:
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "select unix_timestamp(start_time),unix_timestamp(end_time),charge_nu,charge_times,\
                         car_type,source_nu,gate_id,child_order,garage_id,plate_nu,total_order,is_open,\
                         res_garage_id from business_charge_child_order where total_order = '%s' and is_open=0 \
                         and del_flag=0 order by start_time asc",
                         total_order.c_str());
                break;
            case 1:
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "select unix_timestamp(start_time),unix_timestamp(end_time),charge_nu,charge_times,\
                         car_type,source_nu,gate_id,child_order,garage_id,plate_nu,total_order,is_open,\
                         res_garage_id from business_charge_child_order where total_order = '%s' and is_open=1 \
                         and del_flag=0 order by start_time asc",
                         total_order.c_str());
                break;
            default:
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "select unix_timestamp(start_time),unix_timestamp(end_time),charge_nu,charge_times,\
                         car_type,source_nu,gate_id,child_order,garage_id,plate_nu,total_order,is_open,\
                         res_garage_id from business_charge_child_order where total_order = '%s' and del_flag=0 \
                         order by start_time asc",
                         total_order.c_str());
                break;
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbTable_business_charge_child_order child_order;
                    //start_time
                    child_order.start_time = atol(STR(row[index]));
                    index++;
                    //end_time
                    child_order.end_time = atol(STR(row[index]));
                    index++;
                    //charge_nu
                    child_order.charge_nu = atof(STR(row[index]));
                    index++;
                    //charge_times
                    child_order.charge_times = atoi(STR(row[index]));
                    index++;
                    //car_type
                    child_order.car_type = atoi(STR(row[index]));
                    index++;
                    //source_nu
                    child_order.source_nu = atoi(STR(row[index]));
                    index++;
                    //gate_id
                    child_order.gate_id = STR(row[index]);
                    index++;
                    //child_order
                    child_order.child_order = STR(row[index]);
                    index++;
                    //garage_id
                    child_order.garage_id = STR(row[index]);
                    index++;
                    //plate_nu
                    child_order.plate_nu = STR(row[index]);
                    index++;
                    //total_order
                    child_order.total_order = STR(row[index]);
                    index++;
                    //is_open
                    child_order.is_open = atoi(STR(row[index]));
                    index++;
                    //res_garage_id
                    child_order.res_garage_id = STR(row[index]);
                    index++;

                    vector_child_order.push_back(child_order);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }


    int I_business_charge_child_order(DataBase *dataBase,
                                      DbTable_business_charge_child_order childOrder) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into business_charge_child_order(child_order,garage_id,gate_id,\
                 plate_nu,start_time,is_open,del_flag,car_type,source_nu,total_order,res_garage_id) \
                 values(uuid(),'%s','%s','%s',from_unixtime('%ld'),1,0,%d,%d,'%s','%s')",
                 childOrder.garage_id.c_str(),
                 childOrder.gate_id.c_str(),
                 childOrder.plate_nu.c_str(),
                 childOrder.start_time,
                 childOrder.car_type,
                 childOrder.source_nu,
                 childOrder.total_order.c_str(),
                 childOrder.res_garage_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }


    int U_business_charge_child_order_W_child_order(DataBase *dataBase,
                                                    DbTable_business_charge_child_order childOrder,
                                                    string child_order) {
        if (child_order.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update business_charge_child_order set start_time = from_unixtime(%ld),end_time = from_unixtime(%ld),\
                 charge_nu = %.1f,charge_times = %d ,car_type = %d,source_nu = %d,is_open = %d where child_order='%s'",
                 childOrder.start_time,
                 childOrder.end_time,
                 childOrder.charge_nu,
                 childOrder.charge_times,
                 childOrder.car_type,
                 childOrder.source_nu,
                 childOrder.is_open,
                 childOrder.child_order.c_str());
        if (is_debug()) {
            bzero(sql_cmd, sizeof(sql_cmd));
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update business_charge_child_order set charge_nu = %.1f,charge_times = %d ,\
                     car_type = %d,source_nu = %d,is_open = %d where plate_nu = '%s' and garage_id = '%s' and child_order='%s'",
                     childOrder.charge_nu,
                     childOrder.charge_times,
                     childOrder.car_type,
                     childOrder.source_nu,
                     childOrder.is_open,
                     childOrder.plate_nu.c_str(),
                     childOrder.garage_id.c_str(),
                     childOrder.child_order.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }


    int U_business_charge_child_order_SET_del_flag_W_plate_nu(DataBase *dataBase,
                                                              string plate_nu, int del_flag) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update business_charge_child_order set del_flag=%d where plate_nu = '%s'",
                 del_flag, plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_child_order_SET_charge_times_charge_nu_W_child_order(DataBase *dataBase,
                                                                               string child_order, int charge_times,
                                                                               double charge_nu) {
        if (child_order.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update business_charge_child_order set charge_times=%d,charge_nu=%.1f  where child_order='%s'",
                 charge_times, charge_nu, child_order.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_business_charge_total_order_W_total_order(DataBase *dataBase,
                                                    DbTable_business_charge_total_order &totalOrder,
                                                    string total_order) {
        if (total_order.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select total_order,parking_id,plate_nu,gate_id,is_open,unix_timestamp(start_time),unix_timestamp(end_time),\n"
                 "       charge_nu,charge_times,sell_reason,front_charge_nu,card_charge_nu,cloud_charge_nu,sell_nu,del_flag,\n"
                 "       is_exception,unix_timestamp(auto_insert_time),unix_timestamp(auto_update_time),guard_name,\n"
                 "       cloud_order_no,paid_times,unix_timestamp(prepay_time) from business_charge_total_order where total_order='%s'",
                 total_order.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //total_order
                totalOrder.total_order = STR(row[index]);
                index++;
                //parking_id
                totalOrder.parking_id = STR(row[index]);
                index++;
                //plate_nu
                totalOrder.plate_nu = STR(row[index]);
                index++;
                //gate_id
                totalOrder.gate_id = STR(row[index]);
                index++;
                //is_open
                totalOrder.is_open = atoi(STR(row[index]));
                index++;
                //start_time
                totalOrder.start_time = atol(STR(row[index]));
                index++;
                //end_time
                totalOrder.end_time = atol(STR(row[index]));
                index++;
                //charge_nu
                totalOrder.charge_nu = atof(STR(row[index]));
                index++;
                //charge_times
                totalOrder.charge_times = atoi(STR(row[index]));
                index++;
                //sell_reason
                totalOrder.sell_reason = STR(row[index]);
                index++;
                //front_charge_nu
                totalOrder.front_charge_nu = atof(STR(row[index]));
                index++;
                //card_charge_nu
                totalOrder.card_charge_nu = atof(STR(row[index]));
                index++;
                //cloud_charge_nu
                totalOrder.cloud_charge_nu = atof(STR(row[index]));
                index++;
                //sell_nu
                totalOrder.sell_nu = atof(STR(row[index]));
                index++;
                //del_flag
                totalOrder.del_flag = atoi(STR(row[index]));
                index++;
                //is_exception
                totalOrder.is_exception = atoi(STR(row[index]));
                index++;
                //auto_insert_time
                totalOrder.auto_insert_time = atol(STR(row[index]));
                index++;
                //auto_update_time
                totalOrder.auto_update_time = atol(STR(row[index]));
                index++;
                //guard_name
                totalOrder.guard_name = STR(row[index]);
                index++;
                //cloud_order_no
                totalOrder.cloud_order_no = STR(row[index]);
                index++;
                //paid_times
                totalOrder.paid_times = atoi(STR(row[index]));
                index++;
                //prepay_time
                totalOrder.prepay_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_total_order_F_business_charge_total_order_W_plate_nu(DataBase *dataBase,
                                                               string &total_order, string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select total_order from  business_charge_total_order where plate_nu = '%s' \
                 and del_flag!=1 order by start_time desc limit 1",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //total_order
                total_order = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_start_time_end_time_F_business_charge_total_order_W_total_order(DataBase *dataBase,
                                                                          long &start_time, long &end_time,
                                                                          string total_order) {
        if (total_order.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select unix_timestamp(start_time), unix_timestamp(end_time) from business_charge_total_order where total_order = '%s' order by major_id desc limit 1",
                 total_order.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;
                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //start_time
                start_time = atol(STR(row[index]));
                index++;
                //end_time
                end_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_SUM_charge_nu_F_business_charge_total_order(DataBase *dataBase,
                                                      double &charge_nu_sum,
                                                      string plate_nu, long start_time, long end_time) {
        if (plate_nu.empty()) {
            return -1;
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select sum(charge_nu) from  business_charge_total_order where \
                 plate_nu='%s' and end_time >= from_unixtime(%ld) and end_time < from_unixtime(%ld)",
                 plate_nu.c_str(), start_time, end_time);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;

            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned int index = 0;
            //charge_nu_sum
            charge_nu_sum = atof(STR(row[index]));
            index++;
        }

        mysql_free_result(res);
        return ret;
    }

    int I_business_charge_total_order(DataBase *dataBase,
                                      DbTable_business_charge_total_order totalOrder) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into business_charge_total_order(total_order,parking_id, gate_id,plate_nu,start_time,is_open,del_flag,is_exception) values(uuid(),'%s','%s','%s',from_unixtime('%ld'),1,0,%d)",
                 totalOrder.parking_id.c_str(),
                 totalOrder.gate_id.c_str(),
                 totalOrder.plate_nu.c_str(),
                 totalOrder.start_time,
                 totalOrder.is_exception);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_total_order_SET_del_flag_W_plate_nu(DataBase *dataBase, string plate_nu, int del_flag) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update business_charge_total_order set del_flag=%d where plate_nu = '%s'",
                 del_flag, plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_total_order_SET_end_time_W_plate_nu(DataBase *dataBase, string plate_nu, long end_time) {
        if (plate_nu.empty()) {
            return -1;
        }
        if (is_debug()) {
            return 0;
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update business_charge_total_order set end_time=from_unixtime(%ld) where plate_nu = '%s' and del_flag=0",
                 end_time, plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_total_order_SET_is_open_W_total_order(DataBase *dataBase, string total_order, int is_open) {
        if (total_order.empty()) {
            return -1;
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update  business_charge_total_order set is_open=%d where total_order = '%s'",
                 is_open, total_order.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_total_order_SET_prepay_time_W_total_order(DataBase *dataBase,
                                                                    string total_order,
                                                                    long prepay_time) {
        if (total_order.empty()) {
            return -1;
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update  business_charge_total_order set prepay_time=from_unixtime(%ld) where total_order = '%s'",
                 prepay_time, total_order.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_total_order_SET_guard_name_W_total_order(DataBase *dataBase,
                                                                   string guard_name, string total_order) {
        if (total_order.empty() || guard_name.empty()) {
            return -1;
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update  business_charge_total_order set guard_name='%s' where total_order = '%s'",
                 guard_name.c_str(), total_order.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_business_charge_total_order_fee(DataBase *dataBase,
                                          string total_order, double cost, int charge_type,
                                          string reason, bool is_exception, string cloud) {
        if (total_order.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        switch (charge_type) {
            case 1://count
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set charge_times=%d where total_order = '%s'",
                         (int) cost, total_order.c_str());
                break;
            case 2://total money
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set charge_nu=%.1f where total_order = '%s'",
                         cost, total_order.c_str());
                break;
            case 3://front charge money
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set front_charge_nu=%.1f where total_order = '%s'",
                         cost, total_order.c_str());
                break;
            case 4://money card
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set card_charge_nu=%.1f where total_order = '%s'",
                         cost, total_order.c_str());
                break;
            case 5://cloud charge money
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set cloud_charge_nu=%.1f where total_order = '%s'",
                         cost, total_order.c_str());
                break;
            case 6://sell money
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set sell_nu=%.1f, sell_reason='%s' where total_order = '%s'",
                         cost, reason.c_str(), total_order.c_str());
                break;
            case 7://count
                snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                         "update  business_charge_total_order set  paid_times=%d where total_order = '%s'",
                         (int) cost, total_order.c_str());
                break;
            default:
                break;
        }


        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_is_in_parking_F_car_in_parking_W_plate_nu(DataBase *dataBase,
                                                    int &is_in_parking, string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select is_in_parking from car_in_parking where plate_nu = '%s'",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
            is_in_parking = 0;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
                is_in_parking = 0;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                is_in_parking = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_is_in_parking_state_plate_nu_F_car_in_parking_W_space_id(DataBase *dataBase,
                                                                   int &is_in_parking, int &state, string &plate_nu,
                                                                   string space_id) {
        if (space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select  is_in_parking, state,plate_nu from car_in_parking where space_id = '%s'",
                 space_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //is_in_parking
                is_in_parking = atoi(STR(row[0]));
                //state
                state = atoi(STR(row[1]));
                //plate_nu
                plate_nu = STR(row[2]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_update_time_F_car_in_parking_W_plate_nu(DataBase *dataBase, long &update_time, string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select  unix_timestamp(update_time) from car_in_parking where  plate_nu = '%s' ",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //update_time
                update_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_car_type_F_car_in_parking_W_plate_nu(DataBase *dataBase, int &car_type, string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select car_type from car_in_parking where plate_nu = '%s'",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //car_type
                car_type = atoi(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_space_id_F_car_in_parking_W_plate_nu(DataBase *dataBase, string &space_id, string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select space_id from car_in_parking where plate_nu = '%s' ",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //space_id
                space_id = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_plate_nu_F_car_in_parking_W_space_id(DataBase *dataBase, string &plate_nu, string space_id) {
        if (space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select plate_nu from car_in_parking where space_id = '%s' and state !=1 and is_in_parking = 1",
                 space_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //plate_nu
                plate_nu = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int I_car_in_parking(DataBase *dataBase, DbTable_car_in_parking carInParking) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into car_in_parking(plate_nu,parking_id,garage_id,space_id,\
                 car_type,gate_id,is_in_parking,create_time,update_time,state) \
                 values('%s','%s','%s','%s',%d,'%s',%d,from_unixtime(%ld),from_unixtime(%ld),%d) \
                 ON DUPLICATE KEY UPDATE garage_id='%s',space_id='%s', car_type=%d, gate_id='%s',\
                 is_in_parking=%d,state=%d,update_time=from_unixtime(%ld)",
                 carInParking.plate_nu.c_str(),
                 carInParking.parking_id.c_str(),
                 carInParking.garage_id.c_str(),
                 carInParking.space_id.c_str(),
                 carInParking.car_type,
                 carInParking.gate_id.c_str(),
                 carInParking.is_in_parking,
                 carInParking.create_time,
                 carInParking.update_time,
                 carInParking.state,
                 carInParking.garage_id.c_str(),
                 carInParking.space_id.c_str(),
                 carInParking.car_type,
                 carInParking.gate_id.c_str(),
                 carInParking.is_in_parking,
                 carInParking.state,
                 carInParking.update_time);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_car_in_parking_SET_state_W_space_id_plate_nu(DataBase *dataBase, string plate_nu,
                                                       string space_id, int status) {
        if (plate_nu.empty() || space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update car_in_parking set state = %d where space_id = '%s' and plate_nu = '%s'",
                 status, space_id.c_str(), plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_car_in_parking_SET_is_in_parking_W_garage_id_plate_nu(DataBase *dataBase,
                                                                string plate_nu, string garage_id,
                                                                int is_out, int space_state, long timestamp) {
        if (garage_id.empty() || plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (space_state == -1) {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update car_in_parking set is_in_parking = %d, garage_id = '%s',"
                     "update_time=from_unixtime(%ld) where plate_nu = '%s'",
                     is_out, garage_id.c_str(), timestamp, plate_nu.c_str());
        } else {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update car_in_parking set is_in_parking = %d, state=%d, garage_id = '%s',"
                     "update_time=from_unixtime(%ld) where plate_nu = '%s'",
                     is_out, space_state, garage_id.c_str(), timestamp, plate_nu.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_car_through_log_W_gate_id_through_type(DataBase *dataBase,
                                                 DbTable_car_through_log &carThroughLog,
                                                 string gate_id, int through_type) {
        if (gate_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select parking_id,garage_id,plate_nu,space_id,car_type,camera_ip_addr,\n"
                 "       in_out_type,car_picture_addr,gate_id,led_display_info,reason_id,\n"
                 "       gate_control,gate_way,through_type,unix_timestamp(scan_time),car_owner_name,car_owner_phone,\n"
                 "       unusual_open_gate,bussiness_id,lock_car,led_car_type,garage_mode from car_through_log\n"
                 "where gate_id='%s' and through_type=%d and gate_way <50 order by major_id desc limit 1",
                 gate_id.c_str(), through_type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    //parking_id
                    carThroughLog.parking_id = STR(row[index]);
                    index++;
                    //garage_id
                    carThroughLog.garage_id = STR(row[index]);
                    index++;
                    //plate_nu
                    carThroughLog.plate_nu = STR(row[index]);
                    index++;
                    //space_id
                    carThroughLog.space_id = STR(row[index]);
                    index++;
                    //car_type
                    carThroughLog.car_type = atoi(STR(row[index]));
                    index++;
                    //camera_ip_addr
                    carThroughLog.camera_ip_addr = STR(row[index]);
                    index++;
                    //in_out_type
                    carThroughLog.in_out_type = atoi(STR(row[index]));
                    index++;
                    //car_picture_addr
                    carThroughLog.car_pictour_addr = STR(row[index]);
                    index++;
                    //led_display_info
                    carThroughLog.led_display_info = STR(row[index]);
                    index++;
                    //reason_id
                    carThroughLog.reason_id = STR(row[index]);
                    index++;
                    //gate_control
                    carThroughLog.gate_control = atoi(STR(row[index]));
                    index++;
                    //gate_way
                    carThroughLog.gate_way = atoi(STR(row[index]));
                    index++;
                    //through_type
                    carThroughLog.through_type = atoi(STR(row[index]));
                    index++;
                    //scan_time
                    carThroughLog.scan_time = atol(STR(row[index]));
                    index++;
                    //car_owner_name
                    carThroughLog.car_owner_name = STR(row[index]);
                    index++;
                    //car_owner_phone
                    carThroughLog.car_owner_phone = STR(row[index]);
                    index++;
                    //unusual_open_gate
                    carThroughLog.unusual_open_gate = STR(row[index]);
                    index++;
                    //bussiness_id
                    carThroughLog.bussiness_id = STR(row[index]);
                    index++;
                    //lock_car
                    carThroughLog.lock_car = atoi(STR(row[index]));
                    index++;
                    //led_car_type
                    carThroughLog.led_car_type = atoi(STR(row[index]));
                    index++;
                    //garage_mode
                    carThroughLog.garage_mode = atoi(STR(row[index]));
                    index++;
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_car_through_log_W_plate_nu_gate_way(DataBase *dataBase, DbTable_car_through_log &carThroughLog,
                                              string plate_nu, int gate_way) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select parking_id,garage_id,plate_nu,space_id,car_type,camera_ip_addr,\n"
                 "       in_out_type,car_picture_addr,gate_id,led_display_info,reason_id,\n"
                 "       gate_control,gate_way,through_type,unix_timestamp(scan_time),car_owner_name,car_owner_phone,\n"
                 "       unusual_open_gate,bussiness_id,lock_car,led_car_type,garage_mode from car_through_log\n"
                 "where plate_nu='%s' and gate_way=%d order by major_id desc limit 1",
                 plate_nu.c_str(), gate_way);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    //parking_id
                    carThroughLog.parking_id = STR(row[index]);
                    index++;
                    //garage_id
                    carThroughLog.garage_id = STR(row[index]);
                    index++;
                    //plate_nu
                    carThroughLog.plate_nu = STR(row[index]);
                    index++;
                    //space_id
                    carThroughLog.space_id = STR(row[index]);
                    index++;
                    //car_type
                    carThroughLog.car_type = atoi(STR(row[index]));
                    index++;
                    //camera_ip_addr
                    carThroughLog.camera_ip_addr = STR(row[index]);
                    index++;
                    //in_out_type
                    carThroughLog.in_out_type = atoi(STR(row[index]));
                    index++;
                    //car_picture_addr
                    carThroughLog.car_pictour_addr = STR(row[index]);
                    index++;
                    //led_display_info
                    carThroughLog.led_display_info = STR(row[index]);
                    index++;
                    //reason_id
                    carThroughLog.reason_id = STR(row[index]);
                    index++;
                    //gate_control
                    carThroughLog.gate_control = atoi(STR(row[index]));
                    index++;
                    //gate_way
                    carThroughLog.gate_way = atoi(STR(row[index]));
                    index++;
                    //through_type
                    carThroughLog.through_type = atoi(STR(row[index]));
                    index++;
                    //scan_time
                    carThroughLog.scan_time = atol(STR(row[index]));
                    index++;
                    //car_owner_name
                    carThroughLog.car_owner_name = STR(row[index]);
                    index++;
                    //car_owner_phone
                    carThroughLog.car_owner_phone = STR(row[index]);
                    index++;
                    //unusual_open_gate
                    carThroughLog.unusual_open_gate = STR(row[index]);
                    index++;
                    //bussiness_id
                    carThroughLog.bussiness_id = STR(row[index]);
                    index++;
                    //lock_car
                    carThroughLog.lock_car = atoi(STR(row[index]));
                    index++;
                    //led_car_type
                    carThroughLog.led_car_type = atoi(STR(row[index]));
                    index++;
                    //garage_mode
                    carThroughLog.garage_mode = atoi(STR(row[index]));
                    index++;
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_led_display_info_unusual_open_gate_F_car_through_log_W_plate_nu(DataBase *dataBase, string &led_display_info,
                                                                          string &unusual_open_gate, string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select led_display_info,unusual_open_gate from car_through_log where plate_nu='%s' and gate_way=1 order by major_id desc LIMIT 1",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //led_display_info
                led_display_info = STR(row[index]);
                index++;
                //unusual_open_gate
                unusual_open_gate = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int I_car_through_log(DataBase *dataBase, DbTable_car_through_log carThroughLog) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into car_through_log(parking_id,garage_id,gate_id,plate_nu,car_type,\
                 space_id,bussiness_id,gate_way,reason_id,led_display_info,through_type,\
                 camera_ip_addr,car_picture_addr,gate_control,scan_time,car_owner_name,\
                 car_owner_phone,unusual_open_gate, lock_car, led_car_type,garage_mode) \
                 values('%s','%s','%s','%s',%d,'%s','%s',%d,'%s','%s',%d,'%s','%s',%d,\
                 from_unixtime(%ld),'%s','%s','%s',%d,%d,%d)",
                 carThroughLog.parking_id.c_str(),
                 carThroughLog.garage_id.c_str(),
                 carThroughLog.gate_id.c_str(),
                 carThroughLog.plate_nu.c_str(),
                 carThroughLog.car_type,
                 carThroughLog.space_id.c_str(),
                 carThroughLog.bussiness_id.c_str(),
                 carThroughLog.gate_way,
                 carThroughLog.reason_id.c_str(),
                 carThroughLog.led_display_info.c_str(),
                 carThroughLog.through_type,
                 carThroughLog.camera_ip_addr.c_str(),
                 carThroughLog.car_pictour_addr.c_str(),
                 carThroughLog.gate_control,
                 carThroughLog.scan_time,
                 carThroughLog.car_owner_name.c_str(),
                 carThroughLog.car_owner_phone.c_str(),
                 carThroughLog.unusual_open_gate.c_str(),
                 carThroughLog.lock_car,
                 carThroughLog.led_car_type,
                 carThroughLog.garage_mode);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }


    int D_car_through_log_W_plate_nu(DataBase *dataBase, string plate_nu) {

        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "delete from car_through_log where plate_nu = '%s'", plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int I_card_used_record(DataBase *dataBase, DbTable_card_used_record cardUsedRecord) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into card_used_record(garage_id,plate_nu,card_id,total_order,card_type,use_times, use_money,\
                 start_time,end_time)  values('%s','%s','%s','%s',%d,%d,%.1f,from_unixtime(%ld),from_unixtime(%ld))",
                 cardUsedRecord.garage_id.c_str(),
                 cardUsedRecord.plate_nu.c_str(),
                 cardUsedRecord.card_id.c_str(),
                 cardUsedRecord.total_order.c_str(),
                 cardUsedRecord.card_type,
                 cardUsedRecord.use_times,
                 cardUsedRecord.use_money,
                 cardUsedRecord.start_time,
                 cardUsedRecord.end_time);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_type_F_feescale_info_W_garage_id_res_type(DataBase *dataBase, int &type, string garage_id, int res_type) {
        if (garage_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select type from feescale_info where garage_id = '%s' and res_type = %d",
                 garage_id.c_str(), res_type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
            type = 0;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
                type = 0;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                type = atoi(STR(row[0]));
                type += 1;//0=null
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_charge_id_F_feescale_info_W_garage_id_res_type(DataBase *dataBase,
                                                         string &charge_id,
                                                         string garage_id, int res_type) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select charge_id from feescale_info where  garage_id = '%s' and res_type=%d",
                 garage_id.c_str(), res_type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;

            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned int index = 0;
            //charge_id
            charge_id = STR(row[index]);
            index++;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_front_charge_record_allless_W_gate_id_time_interval(DataBase *dataBase,
                                                              vector<DbTable_front_charge_record> &vector_front_charge_record,
                                                              int &total, string gate_id, string gate_ids_str,
                                                              long start_time, long end_time, int start_line,
                                                              int lines) {
        if (gate_id.empty() || gate_ids_str.empty()) {
            return -1;
        }

        int ret = 0;
        char idslist[1024];
        strcpy(idslist, "");
        char ids_str[1024 * 8];
        bzero(ids_str, sizeof(ids_str));
        memcpy(ids_str, gate_ids_str.c_str(), gate_ids_str.size());

        char *p = strtok(ids_str, ",");
        while (p) {
            int curr_len = strlen(idslist);
            if (1024 - curr_len <= 0)
                break;
            printf("%s\n", p);
            snprintf(idslist + curr_len, 1024 - curr_len, (curr_len == 0 ? "'%s'" : ",'%s'"), p);
            p = strtok(nullptr, ",");
        }

        string community_id;
        S_community_id_F_parking_config(dataBase, community_id);

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select plate_nu,charge_nu,sell_reason,real_charge,self_paid,sell_nu "
                 "from front_charge_record where gate_id in (%s) and from_unixtime(%ld)  < create_time  order by major_id desc", \
                 idslist, start_time);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbTable_front_charge_record dbTableFrontChargeRecord;
                    //plate_nu
                    dbTableFrontChargeRecord.plate_nu = STR(row[index]);
                    index++;
                    //charge_nu
                    dbTableFrontChargeRecord.charge_nu = atof(STR(row[index]));
                    index++;
                    //sell_reason
                    dbTableFrontChargeRecord.sell_reason = STR(row[index]);
                    index++;
                    //real_charge
                    dbTableFrontChargeRecord.real_charge = atof(STR(row[index]));
                    index++;
                    //self_paid
                    dbTableFrontChargeRecord.self_paid = atof(STR(row[index]));
                    index++;
                    //sell_nu
                    dbTableFrontChargeRecord.sell_nu = atof(STR(row[index]));
                    index++;
                    vector_front_charge_record.push_back(dbTableFrontChargeRecord);
                }
            }
        }

        total = lines;
        mysql_free_result(res);
        return ret;
    }

    int S_real_charge_F_front_charge_record_W_gate_id_time_interval(DataBase *dataBase,
                                                                    double &money,
                                                                    string gate_id, string gate_ids_str,
                                                                    long start_time, long end_time) {
        if (gate_id.empty() || gate_ids_str.empty()) {
            return -1;
        }

        char idslist[1024];
        strcpy(idslist, "");
        char ids_str[1024 * 8];
        bzero(ids_str, sizeof(ids_str));
        memcpy(ids_str, gate_ids_str.c_str(), gate_ids_str.size());

        char *p = strtok(ids_str, ",");
        while (p) {
            int curr_len = strlen(idslist);
            if (1024 - curr_len <= 0)
                break;
            printf("%s\n", p);
            snprintf(idslist + curr_len, 1024 - curr_len, (curr_len == 0 ? "'%s'" : ",'%s'"), p);
            p = strtok(nullptr, ",");
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select real_charge from front_charge_record where gate_id in (%s) and \
                 from_unixtime(%ld) < create_time and real_charge>0",
                 idslist, start_time);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                money = atof(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int I_front_charge_record_append(DataBase *dataBase,
                                     DbTable_front_charge_record frontChargeRecord) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into front_charge_record(gate_id,real_charge,charge_nu, \
                 sell_nu,sell_reason,create_time, self_paid,plate_nu) values('%s',%.1f,%.1f,%.1f,'%s', from_unixtime(%ld),%.1f ,'%s')",
                 frontChargeRecord.gate_id.c_str(),
                 frontChargeRecord.real_charge,
                 frontChargeRecord.charge_nu,
                 frontChargeRecord.sell_nu,
                 frontChargeRecord.sell_reason.c_str(),
                 frontChargeRecord.create_time,
                 frontChargeRecord.self_paid,
                 frontChargeRecord.plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_front_record_all_W_record_no(DataBase *dataBase, vector<DbTable_front_record> &vector_frontRecord,
                                       string record_no) {
        if (record_no.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select record_no, in_out_type,plate_nu,camera_ip,car_type,gate_id,bussiness_id,"
                 "space_id,res_garage_id,target_garage_id,use_res_num,picture,owner_name,owner_phone,"
                 "checkin_str,led_car_type,unix_timestamp(visitor_time),led, voice, exception_price,"
                 "unix_timestamp(in_time),unix_timestamp(out_time), stay_time,car_type_name,car_state,"
                 "amount_due, amount_paid, amount_to_paid, abnormal_amount, note from front_record where record_no='%s'",
                 record_no.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    DbTable_front_record frontRecord;
                    unsigned int index = 0;
                    //record_no
                    frontRecord.record_no = STR(row[index]);
                    index++;
                    //in_out_type
                    frontRecord.in_out_type = atoi(STR(row[index]));
                    index++;
                    //plate_nu
                    frontRecord.plate_nu = STR(row[index]);
                    index++;
                    //camera_ip
                    frontRecord.camera_ip = STR(row[index]);
                    index++;
                    //car_type
                    frontRecord.car_type = atoi(STR(row[index]));
                    index++;
                    //gate_id
                    frontRecord.gate_id = STR(row[index]);
                    index++;
                    //bussiness_id
                    frontRecord.bussiness_id = STR(row[index]);
                    index++;
                    //space_id
                    frontRecord.space_id = STR(row[index]);
                    index++;
                    //res_garage_id
                    frontRecord.res_garage_id = STR(row[index]);
                    index++;
                    //target_garage_id
                    frontRecord.target_garage_id = STR(row[index]);
                    index++;
                    //use_res_num
                    frontRecord.use_res_num = atoi(STR(row[index]));
                    index++;
                    //picture
                    frontRecord.picture = STR(row[index]);
                    index++;
                    //owner_name
                    frontRecord.owner_name = STR(row[index]);
                    index++;
                    //owner_phone
                    frontRecord.owner_phone = STR(row[index]);
                    index++;
                    //checkin_str
                    frontRecord.checkin_str = STR(row[index]);
                    index++;
                    //led_car_type
                    frontRecord.led_car_type = atoi(STR(row[index]));
                    index++;
                    //visitor_time
                    frontRecord.visitor_time = atoi(STR(row[index]));
                    index++;
                    //led
                    frontRecord.led = STR(row[index]);
                    index++;
                    //voice
                    frontRecord.voice = STR(row[index]);
                    index++;
                    //exception_price
                    frontRecord.exception_price = atof(STR(row[index]));
                    index++;
                    //in_time
                    frontRecord.in_time = atol(STR(row[index]));
                    index++;
                    //out_time
                    frontRecord.out_time = atol(STR(row[index]));
                    index++;
                    //stay_time
                    frontRecord.stay_time = STR(row[index]);
                    index++;
                    //car_type_name
                    frontRecord.car_type_name = STR(row[index]);
                    index++;
                    //car_state
                    frontRecord.car_state = STR(row[index]);
                    index++;
                    //amount_due
                    frontRecord.amount_due = atof(STR(row[index]));
                    index++;
                    //amount_paid
                    frontRecord.amount_paid = atof(STR(row[index]));
                    index++;
                    //amount_to_paid
                    frontRecord.amount_to_paid = atof(STR(row[index]));
                    index++;
                    //abnormal_amount
                    frontRecord.abnormal_amount = atof(STR(row[index]));
                    index++;
                    //note
                    frontRecord.note = STR(row[index]);
                    index++;

                    vector_frontRecord.push_back(frontRecord);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_front_record_in_W_plate_nu(DataBase *dataBase,
                                     DbTable_front_record &frontRecord,
                                     string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select record_no,in_out_type,plate_nu,camera_ip,car_type,gate_id,bussiness_id,space_id,\n"
                 "       res_garage_id,target_garage_id,use_res_num,picture,owner_name,owner_phone,checkin_str,\n"
                 "       led_car_type,visitor_time,led,voice,exception_price,unix_timestamp(in_time),unix_timestamp(out_time), stay_time,car_type_name,\n"
                 "       car_state,amount_due,amount_paid,amount_to_paid,abnormal_amount,note from front_record where plate_nu ='%s'\n"
                 "order by  in_time desc limit 1;",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //record_no
                frontRecord.record_no = STR(row[index]);
                index++;
                //in_out_type
                frontRecord.in_out_type = atoi(STR(row[index]));
                index++;
                //plate_nu
                frontRecord.plate_nu = STR(row[index]);
                index++;
                //camera_ip
                frontRecord.camera_ip = STR(row[index]);
                index++;
                //car_type
                frontRecord.car_type = atoi(STR(row[index]));
                index++;
                //gate_id
                frontRecord.gate_id = STR(row[index]);
                index++;
                //bussiness_id
                frontRecord.bussiness_id = STR(row[index]);
                index++;
                //space_id
                frontRecord.space_id = STR(row[index]);
                index++;
                //res_garage_id
                frontRecord.res_garage_id = STR(row[index]);
                index++;
                //target_garage_id
                frontRecord.target_garage_id = STR(row[index]);
                index++;
                //use_res_num
                frontRecord.use_res_num = atoi(STR(row[index]));
                index++;
                //picture
                frontRecord.picture = STR(row[index]);
                index++;
                //owner_name
                frontRecord.owner_name = STR(row[index]);
                index++;
                //owner_phone
                frontRecord.owner_phone = STR(row[index]);
                index++;
                //checkin_str
                frontRecord.checkin_str = STR(row[index]);
                index++;
                //led_car_type
                frontRecord.led_car_type = atoi(STR(row[index]));
                index++;
                //visitor_time
                frontRecord.visitor_time = atol(STR(row[index]));
                index++;
                //led
                frontRecord.led = STR(row[index]);
                index++;
                //voice
                frontRecord.voice = STR(row[index]);
                index++;
                //exception_price
                frontRecord.exception_price = atof(STR(row[index]));
                index++;
                //in_time
                frontRecord.in_time = atol(STR(row[index]));
                index++;
                //out_time
                frontRecord.out_time = atol(STR(row[index]));
                index++;
                //stay_time
                frontRecord.stay_time = STR(row[index]);
                index++;
                //car_type_name
                frontRecord.car_type_name = STR(row[index]);
                index++;
                //car_state
                frontRecord.car_state = STR(row[index]);
                index++;
                //amount_due
                frontRecord.amount_due = atof(STR(row[index]));
                index++;
                //amount_paid
                frontRecord.amount_paid = atof(STR(row[index]));
                index++;
                //amount_to_paid
                frontRecord.amount_to_paid = atof(STR(row[index]));
                index++;
                //abnormal_amount
                frontRecord.abnormal_amount = atof(STR(row[index]));
                index++;
                //note
                frontRecord.note = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int I_front_record_in(DataBase *dataBase, DbTable_front_record frontRecord) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (frontRecord.visitor_time > 0) {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "insert into  front_record(record_no, in_out_type,plate_nu,camera_ip,car_type,\
                     gate_id,bussiness_id,space_id,res_garage_id,target_garage_id,use_res_num,picture,\
		             owner_name,owner_phone,checkin_str,led_car_type,visitor_time,car_type_name,car_state)\
		             value(uuid(),%d,'%s','%s',%d,'%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s',%d,from_unixtime(%ld),'%s','%s')",
                     frontRecord.in_out_type,
                     frontRecord.plate_nu.c_str(),
                     frontRecord.camera_ip.c_str(),
                     frontRecord.car_type,
                     frontRecord.gate_id.c_str(),
                     frontRecord.bussiness_id.c_str(),
                     frontRecord.space_id.c_str(),
                     frontRecord.res_garage_id.c_str(),
                     frontRecord.target_garage_id.c_str(),
                     frontRecord.use_res_num,
                     frontRecord.picture.c_str(),
                     frontRecord.owner_name.c_str(),
                     frontRecord.owner_phone.c_str(),
                     frontRecord.checkin_str.c_str(),
                     frontRecord.led_car_type,
                     frontRecord.visitor_time,
                     frontRecord.car_type_name.c_str(),
                     frontRecord.car_state.c_str());
        } else {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "insert into  front_record(record_no, in_out_type,plate_nu,camera_ip,car_type,\
                     gate_id,bussiness_id,space_id,res_garage_id,target_garage_id,use_res_num,picture,\
                     owner_name,owner_phone,checkin_str,led_car_type,visitor_time,car_type_name,car_state)\
                     value(uuid(),%d,'%s','%s',%d,'%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s',%d,null,'%s','%s')",
                     frontRecord.in_out_type,
                     frontRecord.plate_nu.c_str(),
                     frontRecord.camera_ip.c_str(),
                     frontRecord.car_type,
                     frontRecord.gate_id.c_str(),
                     frontRecord.bussiness_id.c_str(),
                     frontRecord.space_id.c_str(),
                     frontRecord.res_garage_id.c_str(),
                     frontRecord.target_garage_id.c_str(),
                     frontRecord.use_res_num,
                     frontRecord.picture.c_str(),
                     frontRecord.owner_name.c_str(),
                     frontRecord.owner_phone.c_str(),
                     frontRecord.checkin_str.c_str(),
                     frontRecord.led_car_type,
                     frontRecord.car_type_name.c_str(),
                     frontRecord.car_state.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int I_front_record_out(DataBase *dataBase, DbTable_front_record frontRecord) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (frontRecord.visitor_time > 0) {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "insert into  front_record(record_no, in_out_type,plate_nu,camera_ip,car_type,\
                     gate_id,bussiness_id,space_id,res_garage_id,target_garage_id,use_res_num,picture,\
                     owner_name,owner_phone,checkin_str,led_car_type,visitor_time,\
                     led,voice,exception_price,in_time,out_time,stay_time,car_type_name,car_state,\
                     amount_due, amount_paid, amount_to_paid, abnormal_amount, note)\
                     value(uuid(),%d,'%s','%s',%d,'%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s',%d,from_unixtime(%ld),\
                     '%s','%s',%.1f,from_unixtime(%ld),from_unixtime(%ld),'%s','%s','%s',%.1f,%.1f,%.1f,%.1f,'%s')",
                     frontRecord.in_out_type,
                     frontRecord.plate_nu.c_str(),
                     frontRecord.camera_ip.c_str(),
                     frontRecord.car_type,
                     frontRecord.gate_id.c_str(),
                     frontRecord.bussiness_id.c_str(),
                     frontRecord.space_id.c_str(),
                     frontRecord.res_garage_id.c_str(),
                     frontRecord.target_garage_id.c_str(),
                     frontRecord.use_res_num,
                     frontRecord.picture.c_str(),
                     frontRecord.owner_name.c_str(),
                     frontRecord.owner_phone.c_str(),
                     frontRecord.checkin_str.c_str(),
                     frontRecord.led_car_type,
                     frontRecord.visitor_time,
                     frontRecord.led.c_str(),
                     frontRecord.voice.c_str(),
                     frontRecord.exception_price,
                     frontRecord.in_time,
                     frontRecord.out_time,
                     frontRecord.stay_time.c_str(),
                     frontRecord.car_type_name.c_str(),
                     frontRecord.car_state.c_str(),
                     frontRecord.amount_due,
                     frontRecord.amount_paid,
                     frontRecord.amount_to_paid,
                     frontRecord.abnormal_amount,
                     frontRecord.note.c_str());
        } else {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "insert into  front_record(record_no, in_out_type,plate_nu,camera_ip,car_type,\
                     gate_id,bussiness_id,space_id,res_garage_id,target_garage_id,use_res_num,picture,\
                     owner_name,owner_phone,checkin_str,led_car_type,visitor_time,\
                     led,voice,exception_price,in_time,out_time,stay_time,car_type_name,car_state,\
                     amount_due, amount_paid, amount_to_paid, abnormal_amount, note)\
                     value(uuid(),%d,'%s','%s',%d,'%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s',%d,null,\
                     '%s','%s',%.1f,from_unixtime(%ld),from_unixtime(%ld),'%s','%s','%s',%.1f,%.1f,%.1f,%.1f,'%s'",
                     frontRecord.in_out_type,
                     frontRecord.plate_nu.c_str(),
                     frontRecord.camera_ip.c_str(),
                     frontRecord.car_type,
                     frontRecord.gate_id.c_str(),
                     frontRecord.bussiness_id.c_str(),
                     frontRecord.space_id.c_str(),
                     frontRecord.res_garage_id.c_str(),
                     frontRecord.target_garage_id.c_str(),
                     frontRecord.use_res_num,
                     frontRecord.picture.c_str(),
                     frontRecord.owner_name.c_str(),
                     frontRecord.owner_phone.c_str(),
                     frontRecord.checkin_str.c_str(),
                     frontRecord.led_car_type,
                     frontRecord.led.c_str(),
                     frontRecord.voice.c_str(),
                     frontRecord.exception_price,
                     frontRecord.in_time,
                     frontRecord.out_time,
                     frontRecord.stay_time.c_str(),
                     frontRecord.car_type_name.c_str(),
                     frontRecord.car_state.c_str(),
                     frontRecord.amount_due,
                     frontRecord.amount_paid,
                     frontRecord.amount_to_paid,
                     frontRecord.abnormal_amount,
                     frontRecord.note.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_garage_info_all(DataBase *dataBase, vector<DbTable_garage_info> &vector_garageInfo) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select garage_id,garage_mode,garage_level,parent_id,garage_name "
                 "from garage_info order by garage_level asc ;");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbTable_garage_info garageInfo;
                    //garage_id
                    garageInfo.garage_id = STR(row[index]);
                    index++;
                    //garage_mode
                    garageInfo.garage_mode = atoi(STR(row[index]));
                    index++;
                    //garage_level
                    garageInfo.garage_level = atoi(STR(row[index]));
                    index++;
                    //parent_id
                    garageInfo.parent_id = STR(row[index]);
                    index++;
                    //garage_name
                    garageInfo.garage_name = STR(row[index]);
                    index++;
                    vector_garageInfo.push_back(garageInfo);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }


    int S_garage_mode_F_garage_info_W_garage_id(DataBase *dataBase, int &garage_mode, string garage_id) {
        if (garage_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select garage_mode from garage_info  where garage_id='%s'",
                 garage_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                garage_mode = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_user_name_F_gate_record_W_gate_id(DataBase *dataBase, string &user_name, string gate_id) {
        int ret;
        if (gate_id.empty()) {
            return -1;
        }

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select user_name from gate_record where gate_id = '%s' order by major_id desc limit 1",
                 gate_id.c_str());
        MYSQL_RES *res = nullptr;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                //get user_name
                MYSQL_ROW row = mysql_fetch_row(res);
                user_name = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_sign_time_F_gate_record_W_gate_id_user_name(DataBase *dataBase, long &sign_time, string gate_id,
                                                      string user_name) {
        if (gate_id.empty() || user_name.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select unix_timestamp(sign_time) from gate_record where gate_id = '%s' "
                 "and user_name = '%s' order by sign_time desc limit 1",
                 gate_id.c_str(), user_name.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                sign_time = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int I_gate_record(DataBase *dataBase, DbTable_gate_record gateRecord) {
        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "insert into gate_record(parking_id,garage_id,gate_id,sign_time,"
                 "user_name,user_key,sign_state) value('%s','%s','%s',from_unixtime(%lu),'%s','%s',%d)",
                 gateRecord.parking_id.c_str(),
                 gateRecord.garage_id.c_str(),
                 gateRecord.gate_id.c_str(),
                 gateRecord.sign_time,
                 gateRecord.user_name.c_str(),
                 gateRecord.user_key.c_str(),
                 gateRecord.sign_state);
        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_gate_record_SET_sign_state_W_user_name_gate_id(DataBase *dataBase,
                                                         int sign_state, string user_name, string gate_id) {
        if (user_name.empty() || gate_id.empty()) {
            return -1;
        }
        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update gate_record set sign_state=%d where user_name='%s' and gate_id = '%s'",
                 sign_state,
                 user_name.c_str(),
                 gate_id.c_str());
        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    bool S_user_name_F_guard_info_exist(DataBase *dataBase, string user_name) {
        bool ret = false;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "select 1 from guard_info where user_name like '%%%s%%'", user_name.c_str());
        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = false;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = false;
            } else {
                ret = true;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_user_key_F_guard_info_W_user_name(DataBase *dataBase, string &user_key, string user_name) {

        if (user_name.empty()) {
            return -1;
        }

        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "select user_key from guard_info where user_name like '%%%s%%' limit 1", user_name.c_str());
        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                user_key = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int U_guard_info_SET_user_key_W_user_name(DataBase *dataBase, string user_name, string user_key) {
        if (user_name.empty() || user_key.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update guard_info set user_key='%s' where user_name like '%%%s%%'",
                 user_key.c_str(), user_name.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_full_name_F_guard_info_W_user_name(DataBase *dataBase,
                                             string &full_name, string user_name) {
        if (user_name.empty()) {
            return -1;
        }

        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select  full_name from guard_info  where user_name like '%%%s%%'",
                 user_name.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                full_name = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_iot_owe_info_W_plate_nu_control_type(DataBase *dataBase, DbTable_iot_owe_info &iotOweInfo, string plate_nu,
                                               int control_type) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select owe_days, relate_name, owe_state  from  iot_owe_info where plate_nu = '%s' and control_type=%d order by owe_state desc limit 1",
                 plate_nu.c_str(), control_type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //owe_days
                iotOweInfo.owe_days = atoi(STR(row[index]));
                index++;
                //relate_name
                iotOweInfo.relate_name = STR(row[index]);
                index++;
                //owe_state
                iotOweInfo.owe_state = atoi(STR(row[index]));
                index++;
                iotOweInfo.control_type = control_type;
                iotOweInfo.plate_nu = plate_nu;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_owe_days_relate_name_F_iot_owe_info_W_plate_nu_control_type(DataBase *dataBase,
                                                                      int &owe_days, string &relate_name,
                                                                      string plate_nu, int control_type) {

        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select owe_days, relate_name  from  iot_owe_info where plate_nu = '%s' and control_type=%d limit 1",
                 plate_nu.c_str(), control_type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //owe_days
                owe_days = atoi(STR(row[index]));
                index++;
                //relate_name
                relate_name = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int
    S_lock_info_all_W_plate_nu_lock_type_lock_enable(DataBase *dataBase, DbTable_lock_info &lockInfo, string plate_nu,
                                                     int lock_type, int lock_enable) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select lock_type,lock_enable,lock_period,lock_start_time,lock_end_time,\
                 unix_timestamp(unlock_finish_time) from lock_info where plate_nu='%s' and \
                 lock_type=%d and lock_enable = %d",
                 plate_nu.c_str(), lock_type, lock_enable);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //lock_type
                lockInfo.lock_type = atoi(STR(row[index]));
                index++;
                //lock_enable
                lockInfo.lock_enable = atoi(STR(row[index]));
                index++;
                //lock_period
                lockInfo.lock_period = STR(row[index]);
                index++;
                //lock_start_time
                lockInfo.lock_start_time = convert_timestr_to_stamp(STR(row[index]));
                index++;
                //lock_end_time
                lockInfo.lock_end_time = convert_timestr_to_stamp(STR(row[index]));
                index++;
                if (lockInfo.lock_end_time < lockInfo.lock_start_time) {
                    lockInfo.lock_end_time = lockInfo.lock_end_time + OneDaySeconds;
                }
                //unlock_finish_time
                lockInfo.unlock_finish_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_community_id_F_parking_config(DataBase *dataBase, string &community_id) {
        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select community_id from parking_config");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);

        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                community_id = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_value_F_parking_config_W_config(DataBase *dataBase, string &value, string config) {
        if (config.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select value from parking_config where config = '%s'",
                 config.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                value = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_reason_open_discounts_all(DataBase *dataBase,
                                    vector<DbTable_reason_open_discounts> &vector_reasonOpenDiscounts,
                                    string parking_id) {
        if (parking_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select reason_type,reason_name,reason_id from  reason_open_discounts where parking_id = '%s'",
                 parking_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbTable_reason_open_discounts item;
                    //reason_type
                    item.reason_type = atoi(STR(row[index]));
                    index++;
                    //reason_name
                    item.reason_name = STR(row[index]);
                    index++;
                    //reason_id
                    item.reason_id = STR(row[index]);
                    index++;

                    vector_reasonOpenDiscounts.push_back(item);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int U_space_number_SET_flexible_number_W_garage_id(DataBase *dataBase,
                                                       string garage_id, int num) {
        if (garage_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (num > 0) {
            //临停车位数增加,如果超过设置的最大值,则设置为最大值
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update space_number "
                     "set flexible_number = (case when (flexible_number + %d) >= flexible_total "
                     "then flexible_total else flexible_number + %d end) where garage_id = '%s'",
                     num, num, garage_id.c_str());
        } else {
            //临停车位数减少,如果小于0,则设置为0
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update space_number "
                     "set flexible_number = (case when (flexible_number + %d) <= 0 "
                     "then 0 else flexible_number + %d end) where garage_id = '%s'",
                     num, num, garage_id.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_space_number_SET_temporary_number_W_garage_id(DataBase *dataBase,
                                                        string garage_id, int num) {
        if (garage_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (num > 0) {
            //临停车位数增加,如果超过设置的最大值,则设置为最大值
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update space_number "
                     "set temporary_number = (case when (temporary_number + %d) >= temporary_total "
                     "then temporary_total else temporary_number + %d end) where garage_id = '%s'",
                     num, num, garage_id.c_str());
        } else {
            //临停车位数减少,如果小于0,则设置为0
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update space_number "
                     "set temporary_number = (case when (temporary_number + %d) <= 0 "
                     "then 0 else temporary_number + %d end) where garage_id = '%s'",
                     num, num, garage_id.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_flexible_number_F_space_number_W_garage_id(DataBase *dataBase, int &flexible_number, string garage_id) {
        if (garage_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select flexible_number from space_number where garage_id = '%s'",
                 garage_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //flexible_number
                flexible_number = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temporary_number_F_space_number_W_garage_id(DataBase *dataBase, int &temporary_number, string garage_id) {
        if (garage_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select temporary_number from space_number where garage_id = '%s'",
                 garage_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //temporary_number
                temporary_number = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temp_charge_count_rules_W_charge_id(DataBase *dataBase,
                                              DbTable_temp_charge_count_rules &tempChargeCountRules,
                                              string charge_id) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select charge_id, charge_type, free_time,is_add_free_time,price,is_repeat_across_day,across_day_price,day_type,max_price \
                 from temp_charge_count_rules where  charge_id = '%s' limit 1",
                 charge_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;

            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned int index = 0;
            //charge_id
            tempChargeCountRules.charge_id = STR(row[index]);
            index++;
            //charge_type
            tempChargeCountRules.charge_type = atoi(STR(row[index]));
            index++;
            //free_time
            tempChargeCountRules.free_time = atoi(STR(row[index]));
            index++;
            //is_add_free_time
            tempChargeCountRules.is_add_free_time = atoi(STR(row[index]));
            index++;
            //price
            tempChargeCountRules.price = atof(STR(row[index]));
            index++;
            //is_repeat_across_day
            tempChargeCountRules.is_repeat_across_day = atoi(STR(row[index]));
            index++;
            //across_day_price
            tempChargeCountRules.across_day_price = atof(STR(row[index]));
            index++;
            //day_type
            tempChargeCountRules.day_type = atoi(STR(row[index]));
            index++;
            //max_price
            tempChargeCountRules.max_price = atof(STR(row[index]));
            index++;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temp_charge_hybrid_basic_W_charge_id(DataBase *dataBase,
                                               DbTable_temp_charge_hybrid_basic &tempChargeHybridBasic,
                                               string charge_id) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select major_id,charge_type,charge_name,free_time,is_add_free_time,max_price,\
                 is_limit_max_price,price from temp_charge_hybrid_basic where  charge_id = '%s'",
                 charge_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //major_id
                tempChargeHybridBasic.major_id = atoi(STR(row[index]));
                index++;
                //charge_type
                tempChargeHybridBasic.charge_type = atoi(STR(row[index]));
                index++;
                //charge_name
                tempChargeHybridBasic.charge_name = STR(row[index]);
                index++;
                //free_time
                tempChargeHybridBasic.free_time = atoi(STR(row[index]));
                index++;
                //is_add_free_time
                tempChargeHybridBasic.is_add_free_time = atoi(STR(row[index]));
                index++;
                //max_price
                tempChargeHybridBasic.max_price = atof(STR(row[index]));
                index++;
                //is_limit_max_price
                tempChargeHybridBasic.is_limit_max_price = atoi(STR(row[index]));
                index++;
                //price
                tempChargeHybridBasic.price = atof(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temp_charge_hybrid_rules_W_charge_id_type(DataBase *dataBase,
                                                    vector<DbTable_temp_charge_hybrid_rules> &vector_temp_charge_hybrid_rules,
                                                    string charge_id, int type) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select major_id,charge_id,max_price,is_limit_max_price,overflow_minute,type,overflow_price,need_special_pay,\n"
                 "       free_in_or_out,no_full_need_pay_for_time,start_time,end_time,price,cloud_id \n"
                 "from temp_charge_hybrid_rules where charge_id=‘%s’ and type =%d;",
                 charge_id.c_str(), type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbTable_temp_charge_hybrid_rules item;
                    //major_id
                    item.major_id = atoi(STR(row[index]));
                    index++;
                    //charge_id
                    item.charge_id = STR(row[index]);
                    index++;
                    //max_price
                    item.max_price = atof(STR(row[index]));
                    index++;
                    //is_limit_max_price
                    item.is_limit_max_price = atoi(STR(row[index]));
                    index++;
                    //overflow_minute
                    item.overflow_minute = atoi(STR(row[index]));
                    index++;
                    //need_special_pay
                    item.need_special_pay = atoi(STR(row[index]));
                    index++;
                    //free_in_or_out
                    item.free_in_or_out = atoi(STR(row[index]));
                    index++;
                    //no_full_need_pay_for_time
                    item.no_full_need_pay_for_time = atoi(STR(row[index]));
                    index++;
                    //start_time
                    item.start_time = convert_timestr_to_stamp(STR(row[index]));
                    index++;
                    //end_time
                    item.end_time = convert_timestr_to_stamp(STR(row[index]));
                    index++;
                    //price
                    item.price = atof(STR(row[index]));
                    index++;
                    //cloud_id
                    item.cloud_id = STR(row[index]);
                    index++;

                    vector_temp_charge_hybrid_rules.push_back(item);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temp_charge_hybrid_rules_detail_W_mix_id(DataBase *dataBase,
                                                   vector<DbTable_temp_charge_hybrid_rules_detail> &vector_temp_charge_hybrid_rules_detail,
                                                   int mix_id) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select start_minutes, end_minutes, minutes,price \
                from temp_charge_hybrid_rules_detail where  mix_id = %d order by start_minutes asc",
                 mix_id);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbTable_temp_charge_hybrid_rules_detail item;
                    //start_minutes
                    item.start_minutes = atoi(STR(row[index]));
                    index++;
                    //end_minutes
                    item.end_minutes = atoi(STR(row[index]));
                    index++;
                    //minutes
                    item.minutes = atoi(STR(row[index]));
                    index++;
                    //price
                    item.price = atof(STR(row[index]));
                    index++;

                    vector_temp_charge_hybrid_rules_detail.push_back(item);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_exception_price_F_temp_charge_tcard_rules(DataBase *dataBase, double &exception_price) {
        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select exception_price  from  temp_charge_tcard_rules limit 1");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);

        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                //依次获取
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    //parking_id
                    exception_price = atof(STR(row[index]));
                    index++;
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temp_charge_tcard_rules(DataBase *dataBase,
                                  DbTable_temp_charge_tcard_rules &tempChargeTcardRules) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select free_time, is_repeat_across_day, is_repeat_across,day_type,calculation  \
                 from temp_charge_tcard_rules limit 1");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //free_time
                tempChargeTcardRules.free_time = atoi(STR(row[index]));
                index++;
                //is_repeat_across_day
                tempChargeTcardRules.is_repeat_across_day = atoi(STR(row[index]));
                index++;
                //is_repeat_across
                tempChargeTcardRules.is_repeat_across = atoi(STR(row[index]));
                index++;
                //day_type
                tempChargeTcardRules.day_type = atoi(STR(row[index]));
                index++;
                //calculation
                tempChargeTcardRules.calculation = atoi(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_temp_charge_time_rules_W_charge_id(DataBase *dataBase,
                                             DbTable_temp_charge_time_rules &tempChargeTimeRules,
                                             string charge_id) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select charge_id, charge_type, free_time,is_add_free_time,max_price,\
	            overflow_hour,overflow_minute_unit,overflow_minute_price,no_full_need_pay,is_limit_max_price  \
	            from temp_charge_time_rules where  charge_id = '%s' limit 1",
                 charge_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //charge_id
                tempChargeTimeRules.charge_id = STR(row[index]);
                index++;
                //charge_type
                tempChargeTimeRules.charge_type = atoi(STR(row[index]));
                index++;
                //free_time
                tempChargeTimeRules.free_time = atoi(STR(row[index]));
                index++;
                //is_add_free_time
                tempChargeTimeRules.is_add_free_time = atoi(STR(row[index]));
                index++;
                //max_price
                tempChargeTimeRules.max_price = atof(STR(row[index]));
                index++;
                //overflow_hour
                tempChargeTimeRules.overflow_hour = atoi(STR(row[index]));
                index++;
                //overflow_minute_unit
                tempChargeTimeRules.overflow_minute_unit = atoi(STR(row[index]));
                index++;
                //overflow_minute_price
                tempChargeTimeRules.overflow_minute_price = atof(STR(row[index]));
                index++;
                //no_full_need_pay
                tempChargeTimeRules.no_full_need_pay = atoi(STR(row[index]));
                index++;
                //is_limit_max_price
                tempChargeTimeRules.is_limit_max_price = atoi(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_hour_price_F_temp_charge_time_rules_detail_W_charge_id(DataBase *dataBase,
                                                                 vector<hour_price> &vector_hour_price,
                                                                 string charge_id) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select hours, price from temp_charge_time_rules_detail where  charge_id = '%s'",
                 charge_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    hour_price item;
                    //hours
                    item.hours = atoi(STR(row[index]));
                    index++;
                    //price
                    item.price = atof(STR(row[index]));
                    index++;
                    vector_hour_price.push_back(item);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_terminal_info(DataBase *dataBase, DbTable_terminal_info &terminalInfo) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select ticket,camera_number,display_number,terminal_name,community_id "
                 " from terminal_info");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //ticket
                terminalInfo.ticket = STR(row[index]);
                index++;
                //camera_number
                terminalInfo.camera_number = atoi(STR(row[index]));
                index++;
                //display_number
                terminalInfo.display_number = atoi(STR(row[index]));
                index++;
                //terminal_name
                terminalInfo.terminal_name = STR(row[index]);
                index++;
                //community_id
                terminalInfo.community_id = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    bool S_plate_nu_F_terminal_passing_record_exist(DataBase *dataBase, string &plate_nu_find, string plate_nu,
                                                    bool use_like) {
        bool ret;
        int plate_nu_omit_province = 0;
        if (use_like) {
            char *valuestr = GetInitKeyString("database", "plate_nu_omit_province", "main_config.ini");
            printf("==============LOAD_INI_ATTR: plate_nu_omit_province=%s", valuestr);
            plate_nu_omit_province = atoi(valuestr);
        }
        string community_id;
        S_community_id_F_parking_config(dataBase, community_id);

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (plate_nu_omit_province == 0) {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "select plate_nu from  terminal_passing_record where plate_nu='%s' "
        "order by timestamp desc limit 1",
                     plate_nu.c_str());
            printf("sql_command:%s\n", sql_cmd);
        } else {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "select plate_nu from  terminal_passing_record where plate_nu like '%%%s%%' "
        "order by timestamp desc limit 1",
                     plate_nu.c_str() + 3);
            printf("sql_command:%s\n", sql_cmd);
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = false;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = false;
            } else {
                ret = true;

                MYSQL_ROW row = mysql_fetch_row(res);
                plate_nu_find = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_terminal_passing_record_recent_W_plate_nu(DataBase *dataBase,
                                                    DbTable_terminal_passing_record &terminalPassingRecord,
                                                    string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select parking_id,garage_id,gate_id,group_id,space_id,bussiness_id,"
                 "resunum,res_garage_id,timestamp,through_type,situstion_type,visitor_time,"
                 "charge_id,dev_sn,car_pic,is_open,car_owner_name,car_owner_phone,checkin_str,"
                 "led_car_type from terminal_passing_record where plate_nu='%s' "
                 "order by timestamp desc limit 1", plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //parking_id
                terminalPassingRecord.parking_id = STR(row[index]);
                index++;
                //garage_id
                terminalPassingRecord.garage_id = STR(row[index]);
                index++;
                //gate_id
                terminalPassingRecord.gate_id = STR(row[index]);
                index++;
                //group_id
                terminalPassingRecord.group_id = atoi(STR(row[index]));
                index++;
                //space_id
                terminalPassingRecord.space_id = STR(row[index]);
                index++;
                //bussiness_id
                terminalPassingRecord.bussiness_id = STR(row[index]);
                index++;
                //resunum
                terminalPassingRecord.resunum = atoi(STR(row[index]));
                index++;
                //res_garage_id
                terminalPassingRecord.res_garage_id = STR(row[index]);
                index++;
                //timestamp
                terminalPassingRecord.timestamp = atoi(STR(row[index]));
                index++;
                //through_type
                terminalPassingRecord.through_type = atoi(STR(row[index]));
                index++;
                //situstion_type
                terminalPassingRecord.situstion_type = atoi(STR(row[index]));
                index++;
                //visitor_time
                terminalPassingRecord.visitor_time = atoi(STR(row[index]));
                index++;
                //charge_id
                terminalPassingRecord.charge_id = atoi(STR(row[index]));
                index++;
                //dev_sn
                terminalPassingRecord.dev_sn = STR(row[index]);
                index++;
                //car_pic
                terminalPassingRecord.car_pic = STR(row[index]);
                index++;
                //is_open
                terminalPassingRecord.is_open = atoi(STR(row[index]));
                index++;
                //car_owner_name
                terminalPassingRecord.car_owner_name = STR(row[index]);
                index++;
                //car_owner_phone
                terminalPassingRecord.car_owner_phone = STR(row[index]);
                index++;
                //checkin_str
                terminalPassingRecord.checkin_str = STR(row[index]);
                index++;
                //led_car_type
                terminalPassingRecord.led_car_type = atoi(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int I_terminal_passing_record(DataBase *dataBase,
                                  DbTable_terminal_passing_record terminalPassingRecord) {
        int ret = 0;

        char sql_cmd[SqlCmdLen];

        //delete
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "delete from terminal_passing_record where plate_nu='%s' and garage_id='%s'",
                 terminalPassingRecord.plate_nu.c_str(),
                 terminalPassingRecord.garage_id.c_str());
        MYSQL_RES *res;
        int sql_ret;
        sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);

        //insert
        bzero(sql_cmd, sizeof(sql_cmd));
        if (terminalPassingRecord.visitor_time > 0) {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "insert into terminal_passing_record \
		(parking_id,garage_id,gate_id,plate_nu,group_id,space_id,bussiness_id,resunum,res_garage_id,\
		timestamp,through_type,situstion_type,visitor_time,charge_id,dev_sn,car_pic,is_open,car_owner_name,car_owner_phone,checkin_str,led_car_type)\
		value('%s','%s','%s','%s',%d,'%s','%s',%d,'%s',from_unixtime(%ld),%d,%d,from_unixtime(%ld),%d,'%s','%s',%d,'%s','%s','%s',%d)", \
        terminalPassingRecord.parking_id.c_str(),
                     terminalPassingRecord.garage_id.c_str(),
                     terminalPassingRecord.gate_id.c_str(),
                     terminalPassingRecord.plate_nu.c_str(),
                     terminalPassingRecord.group_id,
                     terminalPassingRecord.space_id.c_str(),
                     terminalPassingRecord.bussiness_id.c_str(),
                     terminalPassingRecord.resunum,
                     terminalPassingRecord.res_garage_id.c_str(),
                     terminalPassingRecord.timestamp,
                     terminalPassingRecord.through_type,
                     terminalPassingRecord.situstion_type,
                     terminalPassingRecord.visitor_time,
                     terminalPassingRecord.charge_id,
                     terminalPassingRecord.dev_sn.c_str(),
                     terminalPassingRecord.car_pic.c_str(),
                     terminalPassingRecord.is_open,
                     terminalPassingRecord.car_owner_name.c_str(),
                     terminalPassingRecord.car_owner_phone.c_str(),
                     terminalPassingRecord.checkin_str.c_str(),
                     terminalPassingRecord.led_car_type
            );
        } else {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "insert into terminal_passing_record \
		(parking_id,garage_id,gate_id,plate_nu,group_id,space_id,bussiness_id,resunum,res_garage_id,\
		timestamp,through_type,situstion_type,visitor_time,charge_id,dev_sn,car_pic,is_open,car_owner_name,car_owner_phone,checkin_str,led_car_type)\
		value('%s','%s','%s','%s',%d,'%s','%s',%d,'%s',from_unixtime(%ld),%d,%d,null,%d,'%s','%s',%d,'%s','%s','%s',%d)", \
        terminalPassingRecord.parking_id.c_str(),
                     terminalPassingRecord.garage_id.c_str(),
                     terminalPassingRecord.gate_id.c_str(),
                     terminalPassingRecord.plate_nu.c_str(),
                     terminalPassingRecord.group_id,
                     terminalPassingRecord.space_id.c_str(),
                     terminalPassingRecord.bussiness_id.c_str(),
                     terminalPassingRecord.resunum,
                     terminalPassingRecord.res_garage_id.c_str(),
                     terminalPassingRecord.timestamp,
                     terminalPassingRecord.through_type,
                     terminalPassingRecord.situstion_type,
                     terminalPassingRecord.charge_id,
                     terminalPassingRecord.dev_sn.c_str(),
                     terminalPassingRecord.car_pic.c_str(),
                     terminalPassingRecord.is_open,
                     terminalPassingRecord.car_owner_name.c_str(),
                     terminalPassingRecord.car_owner_phone.c_str(),
                     terminalPassingRecord.checkin_str.c_str(),
                     terminalPassingRecord.led_car_type);
        }

        sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int D_terminal_passing_record_W_plate_nu(DataBase *dataBase, string plate_nu) {

        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "delete from terminal_passing_record where plate_nu = '%s'", plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int D_terminal_passing_record_W_garage_id_plate_nu(DataBase *dataBase, string garage_id, string plate_nu) {
        if (garage_id.empty() || plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "delete from terminal_passing_record where garage_id = '%s' and plate_nu = '%s';",
                 garage_id.c_str(), plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_last_number_F_bussiness_space_W_garage_id_bussiness_id(DataBase *dataBase, int &last_number, string garage_id,
                                                                 string business_id) {
        if (garage_id.empty() || business_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select last_number from bussiness_space where garage_id = '%s' and bussiness_id = '%s'",
                 garage_id.c_str(),
                 business_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //last_number
                last_number = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_serviceTime_F_bussiness_space_W_garage_id_bussiness_id(DataBase *dataBase, long &start_time, long &end_time,
                                                                 string garage_id, string plate_nu,
                                                                 string business_id) {
        if (garage_id.empty() || business_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select unix_timestamp(start_service),unix_timestamp(end_service) "
                 "from bussiness_space where garage_id = '%s' and bussiness_id = '%s'",
                 garage_id.c_str(), business_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                int index = 0;
                //start_service
                start_time = atol(STR(row[index]));
                index++;
                //end_service
                end_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int U_bussiness_space_SET_last_number_W_garage_id_business_id(DataBase *dataBase,
                                                                  string garage_id, string business_id, int num) {
        if (garage_id.empty() || business_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (num > 0) {
            //临停车位数增加,如果超过设置的最大值,则设置为最大值
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update bussiness_space "
                     "set last_number = (case when (last_number + %d) >= all_number "
                     "then all_number else last_number + %d end) "
                     "where garage_id = '%s' and bussiness_id = '%s'",
                     num, num, garage_id.c_str(), business_id.c_str());
        } else {
            //临停车位数减少,如果小于0,则设置为0
            snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                     "update bussiness_space "
                     "set last_number = (case when (last_number + %d) <= 0 "
                     "then 0 else last_number + %d end) "
                     "where garage_id = '%s' and bussiness_id = '%s'",
                     num, num, garage_id.c_str(), business_id.c_str());
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    bool S_plate_nu_F_car_type_exsit(DataBase *dataBase, string &plate_nu_find, string plate_nu, bool use_like) {

        bool ret;
        int plate_nu_omit_province = 0;
        if (use_like) {
            char *valuestr = GetInitKeyString("database", "plate_nu_omit_province", "main_config.ini");
            printf("==============LOAD_INI_ATTR: plate_nu_omit_province=%s", valuestr);
            plate_nu_omit_province = atoi(valuestr);
        }
        string community_id;
        S_community_id_F_parking_config(dataBase, community_id);

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));

        if (plate_nu_omit_province == 0) {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "select plate_nu from  car_type where plate_nu='%s' order by bind_time desc limit 1", plate_nu.c_str());
            printf("sql_command:%s\n", sql_cmd);
        } else {
            snprintf(sql_cmd, sizeof(sql_cmd) - 1, \
        "select plate_nu from  car_type where plate_nu like '%%%s%%' order by bind_time desc limit 1",
                     plate_nu.c_str() + 3);
            printf("sql_command:%s\n", sql_cmd);
        }

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = false;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = false;
            } else {
                ret = true;

                MYSQL_ROW row = mysql_fetch_row(res);
                plate_nu_find = STR(row[0]);
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_car_type_all_W_plate_nu(DataBase *dataBase,
                                  vector<DbView_car_type> &vector_carType,
                                  string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select garage_id,car_type,space_id,bussiness_id,unix_timestamp(start_service),"
                 "unix_timestamp(end_service),unix_timestamp(visitor_time),unix_timestamp(bind_time),"
                 "car_owner_name, car_owner_phone "
                 "from car_type where plate_nu='%s' order by car_type asc,bind_time asc", plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    DbView_car_type carType;
                    unsigned int index = 0;
                    //garage_id
                    carType.garage_id = STR(row[index]);
                    index++;
                    //car_type
                    carType.car_type = atoi(STR(row[index]));
                    index++;
                    //space_id
                    carType.space_id = STR(row[index]);
                    index++;
                    //bussiness_id
                    carType.bussiness_id = STR(row[index]);
                    index++;
                    //start_service
                    carType.start_service = atol(STR(row[index]));
                    index++;
                    //end_service
                    carType.end_service = atol(STR(row[index]));
                    index++;
                    //visitor_time
                    carType.visitor_time = atol(STR(row[index]));
                    index++;
                    //bind_time
                    carType.bind_time = atol(STR(row[index]));
                    index++;
                    //car_owner_name
                    carType.car_owner_name = STR(row[index]);
                    index++;
                    //car_owner_phone
                    carType.car_owner_phone = STR(row[index]);
                    index++;
                    vector_carType.push_back(carType);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    bool S_plate_nu_space_id_garage_id_F_car_type_exist(DataBase *dataBase,
                                                        string plate_nu, string space_id, string garage_id) {
        if (plate_nu.empty() || space_id.empty() || garage_id.empty()) {
            return false;
        }
        bool ret = false;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select 1 from car_type where plate_nu='%s' and space_id='%s' and garage_id='%s' "
                 "and now() >= start_service and now() < end_service",
                 plate_nu.c_str(), space_id.c_str(), garage_id.c_str());
        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = false;
        } else {
            ret = true;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_serviceTime_F_car_type_W_garage_id_plate_nu_car_type(DataBase *dataBase,
                                                               long &start_time, long &end_time,
                                                               string garage_id, string plate_nu, int car_type) {
        if (garage_id.empty() || plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select unix_timestamp(start_service),unix_timestamp(end_service) "
                 "from car_type where plate_nu='%s' and garage_id='%s' and car_type=%d",
                 plate_nu.c_str(), garage_id.c_str(), car_type);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                int index = 0;
                //start_service
                start_time = atol(STR(row[index]));
                index++;
                //end_service
                end_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_serviceTime_F_car_type_W_space_id(DataBase *dataBase,
                                            long &start_time, long &end_time,
                                            string garage_id, string plate_nu, string space_id) {

        if (space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select unix_timestamp(start_service),unix_timestamp(end_service) "
                 "from car_type where space_id='%s'",
                 space_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                int index = 0;
                //start_service
                start_time = atol(STR(row[index]));
                index++;
                //end_service
                end_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_serviceTime_F_car_type_W_garage_id_plate_nu_Elastic(DataBase *dataBase,
                                                              long &start_time, long &end_time,
                                                              string garage_id, string plate_nu) {
        if (garage_id.empty() || plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select unix_timestamp(start_service),unix_timestamp(end_service) "
                 "from car_type where plate_nu='%s' and garage_id='%s' and car_type=%d",
                 plate_nu.c_str(), garage_id.c_str(), CarType_Elastic);

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                int index = 0;
                //start_service
                start_time = atol(STR(row[index]));
                index++;
                //end_service
                end_time = atol(STR(row[index]));
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_card_info_all_W_plate_nu(DataBase *dataBase,
                                   vector<DbView_card_info> &vector_card_info,
                                   string plate_nu) {
        if (plate_nu.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select card_id,card_type,times_last,money_last,unix_timestamp(start_time),unix_timestamp(end_time) \
                 from card_info where plate_nu='%s' order by end_time asc",
                 plate_nu.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    unsigned int index = 0;
                    DbView_card_info cardInfo;
                    //card_id
                    cardInfo.card_id = STR(row[index]);
                    index++;
                    //card_type
                    cardInfo.card_type = atoi(STR(row[index]));
                    index++;
                    //times_last
                    cardInfo.times_last = atoi(STR(row[index]));
                    index++;
                    //money_last
                    cardInfo.money_last = atof(STR(row[index]));
                    index++;
                    //start_time
                    cardInfo.start_time = atol(STR(row[index]));
                    index++;
                    //end_time
                    cardInfo.end_time = atol(STR(row[index]));
                    index++;
                    vector_card_info.push_back(cardInfo);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_times_last_F_card_info_W_card_id(DataBase *dataBase, int &times_last, string card_id) {
        if (card_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select times_last from card_info where card_id='%s'",
                 card_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //times_last
                times_last = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_money_last_F_card_info_W_card_id(DataBase *dataBase, double &money_last, string card_id) {
        if (card_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select money_last from card_info where card_id='%s'",
                 card_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //money_last
                money_last = atof(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int U_card_info_SET_times_last_W_card_id(DataBase *dataBase, string card_id, int value) {
        if (card_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update card_info set times_last=(case when (times_last + %d) <= 0 then 0 else times_last + %d end) where card_id='%s'",
                 value, value, card_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int U_card_info_SET_money_last_W_card_id(DataBase *dataBase, string card_id, double value) {
        if (card_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "update card_info set money_last=(case when (money_last + %.1f) <= 0 then 0 else money_last + %.1f end) where card_id='%s'",
                 value, value, card_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            ret = 0;
        }

        mysql_free_result(res);
        return ret;
    }

    int S_device_info_all(DataBase *dataBase,
                          vector<DbView_device_info> &vector_deviceInfo) {
        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select parking_id,garage_id,gate_id,"
                 "camera_ip_addr,camera_factory_string, camera_factory,"
                 "display_ip_addr,display_factory_string,display_factory,"
                 "sn,in_out_type,is_on_watch,father_garage_id,garage_level,through_out_type,device_id "
                 "from device_info order by camera_ip_addr limit 500");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);

        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                //依次获取
                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    DbView_device_info deviceInfo;
                    unsigned int index = 0;
                    //parking_id
                    deviceInfo.parking_id = atoi(STR(row[index]));
                    index++;
                    //garage_id
                    deviceInfo.garage_id = STR(row[index]);
                    index++;
                    //gate_id
                    deviceInfo.gate_id = STR(row[index]);
                    index++;
                    //camera_ip_addr
                    deviceInfo.camera_ip_addr = STR(row[index]);
                    index++;
                    //camera_factory_string
                    deviceInfo.camera_factory_string = STR(row[index]);
                    index++;
                    //camera_factory
                    deviceInfo.camera_factory = atoi(STR(row[index]));
                    index++;
                    //display_ip_addr
                    deviceInfo.display_ip_addr = STR(row[index]);
                    index++;
                    //display_factory_string
                    deviceInfo.display_factory_string = STR(row[index]);
                    index++;
                    //display_factory
                    deviceInfo.display_factory = atoi(STR(row[index]));
                    index++;
                    //sn
                    deviceInfo.sn = STR(row[index]);
                    index++;
                    //in_out_type
                    deviceInfo.in_out_type = atoi(STR(row[index]));
                    index++;
                    //is_on_watch
                    deviceInfo.is_on_watch = atoi(STR(row[index]));
                    index++;
                    //father_garage_id
                    deviceInfo.father_garage_id = STR(row[index]);
                    index++;
                    //garage_level
                    deviceInfo.garage_level = atoi(STR(row[index]));
                    index++;
                    //through_out_type
                    deviceInfo.through_out_type = atoi(STR(row[index]));
                    index++;
                    //device_id
                    deviceInfo.device_id = STR(row[index]);
                    index++;
                    vector_deviceInfo.push_back(deviceInfo);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_in_out_type_F_device_info_W_camera_ip_addr(DataBase *dataBase,
                                                     int &in_out_type,
                                                     string camera_ip_addr) {
        if (camera_ip_addr.empty()) {
            return -1;
        }

        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select in_out_type from device_info where camera_ip_addr = '%s' limit 1",
                 camera_ip_addr.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                //依次获取
                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //in_out_type
                in_out_type = atoi(STR(row[index]));
                index++;

            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_activation_state_F_fixed_space_W_garage_id_space_id(DataBase *dataBase,
                                                              int &activation_state,
                                                              string garage_id, string space_id) {
        if (garage_id.empty() || space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select activation_state from fixed_space where garage_id = '%s' and space_id = '%s'",
                 garage_id.c_str(), space_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                //activation_state
                activation_state = atoi(STR(row[0]));
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_user_name_user_phone_F_fixed_space_W_space_id(DataBase *dataBase,
                                                        string &user_name, string &user_phone,
                                                        string space_id) {
        if (space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select  user_name, user_phone_number from fixed_space where space_id = '%s' ",
                 space_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //user_name
                user_name = STR(row[index]);
                index++;
                //user_phone_number
                user_phone = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_space_name_F_fixed_space_W_space_id(DataBase *dataBase,
                                              string &space_name,
                                              string space_id) {
        if (space_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select space_name from fixed_space where space_id = '%s'",
                 space_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                unsigned int index = 0;
                //space_name
                space_name = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }

    int S_gate_info_all(DataBase *dataBase,
                        vector<DbView_gate_info> &vector_gateInfo) {
        int ret = 0;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select garage_id,gate_id,gate_name,is_on_watch,user_name,"
                 "user_key,sign_in  from gate_info order by garage_id asc");

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);

        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                //依次获取
                MYSQL_ROW row;
                for (int i = 0; i < line; i++) {
                    row = mysql_fetch_row(res);
                    DbView_gate_info gateInfo;
                    unsigned int index = 0;
                    //garage_id
                    gateInfo.garage_id = STR(row[index]);
                    index++;
                    //gate_id
                    gateInfo.gate_id = STR(row[index]);
                    index++;
                    //gate_name
                    gateInfo.gate_name = STR(row[index]);
                    index++;
                    //is_on_watch
                    gateInfo.is_on_watch = atoi(STR(row[index]));
                    index++;
                    //user_name
                    gateInfo.user_name = STR(row[index]);
                    index++;
                    //user_key
                    gateInfo.user_key = STR(row[index]);
                    index++;
                    //sign_in
                    gateInfo.sign_in = atoi(STR(row[index]));
                    index++;

                    vector_gateInfo.push_back(gateInfo);
                }
            }
        }

        mysql_free_result(res);
        return ret;
    }

    bool S_gate_id_garage_id_F_gate_info_exist(DataBase *dataBase, string gate_id, string garage_id) {
        bool ret = false;
        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select gate_id,garage_id from gate_info where gate_id='%s' and garage_id='%s' limit 1",
                 gate_id.c_str(), garage_id.c_str());
        MYSQL_RES *res = nullptr;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = false;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line > 0) {
                ret = true;
            } else {
                ret = false;
            }
        }
        mysql_free_result(res);
        return ret;
    }

    int S_garage_id_F_gate_info_W_gate_id(DataBase *dataBase, string &garage_id, string gate_id) {
        if (gate_id.empty()) {
            return -1;
        }
        int ret = 0;

        char sql_cmd[SqlCmdLen];
        bzero(sql_cmd, sizeof(sql_cmd));
        snprintf(sql_cmd, sizeof(sql_cmd) - 1,
                 "select garage_id from  gate_info where gate_id = '%s'",
                 gate_id.c_str());

        MYSQL_RES *res;
        int sql_ret = dataBase->Database_Query(sql_cmd, &res);
        if (sql_ret != 0) {
            ret = -1;
        } else {
            int line = dataBase->CheckResult(res, true);
            if (line <= 0) {
                ret = -1;
            } else {
                ret = 0;

                MYSQL_ROW row = mysql_fetch_row(res);
                int index = 0;
                //garage_id
                garage_id = STR(row[index]);
                index++;
            }
        }

        mysql_free_result(res);
        return ret;
    }
}