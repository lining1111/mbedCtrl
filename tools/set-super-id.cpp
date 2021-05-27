#include <iostream>
#include <string.h>
#include <stdio.h>
#include <nfc/nfc.h>

#ifdef arm64

#include <freefare/freefare.h>

#else
#include <freefare.h>
#endif

#ifdef arm
#include <json/json.h>
#endif

#ifdef arm64

#include <json/json.h>

#endif

#ifdef x86

#include <jsoncpp/json/json.h>

#endif

#include <fstream>

using namespace std;

#ifndef arm
typedef MifareTag FreefareTag;
#endif


string get_cid() {
    string tag_uid = "";
    int error = EXIT_SUCCESS;
    nfc_device *device = nullptr;
    FreefareTag *tags = nullptr;

    nfc_connstring devices[8];
    size_t device_count;

    nfc_context *context;
    nfc_init(&context);
    if (context == NULL) {
        printf("Unable to init libnfc (malloc) \n");
    }

    device_count = nfc_list_devices(context, devices, 8);
    if (device_count <= 0) {
        printf("No NFC device found. \n");
    }

    for (size_t d = 0; d < device_count; d++) {
        device = nfc_open(context, devices[d]);
        if (!device) {
            printf("nfc_open() failed.");
            error = EXIT_FAILURE;
            continue;
        }

        tags = freefare_get_tags(device);
        if (!tags) {
            nfc_close(device);
            printf("Error listing MIFARE classic tag. \n");
        }

        for (int i = 0; (!error) && tags[i]; i++) {
            switch (freefare_get_tag_type(tags[i])) {
#ifdef arm
                case MIFARE_CLASSIC_1K:
                case MIFARE_CLASSIC_4K:
#endif
#ifdef arm64
                case MIFARE_CLASSIC_1K:
                case MIFARE_CLASSIC_4K:
#endif
#ifdef x86
                case CLASSIC_1K:
                case CLASSIC_4K:
#endif
                    break;
                default:
                    continue;
            }
            tag_uid = freefare_get_tag_uid(tags[i]);
        }
        freefare_free_tags(tags);
        nfc_close(device);
    }
    nfc_exit(context);
    return tag_uid;
}


int main() {

    string sid;

    int inorder;

    Json::Value root;
    Json::FastWriter writer;

    while (1) {
        cout << "输入１添加超级卡，输入２退出　" << endl;

        cin >> inorder;

        if (inorder == 1) {
            cout << "请刷一张卡，设置为超级卡 :" << endl;


            sid = get_cid();

            cout << "sid = " << sid << endl;
            root["superID"].append(sid);
        }
        if (inorder == 2)
            break;

    }

    Json::FastWriter fw;

    ofstream ofs;
    ofs.open("./superID.json");
    ofs << fw.write(root);
    ofs.close();

    printf("写入完成 \n");
    return 0;

}
