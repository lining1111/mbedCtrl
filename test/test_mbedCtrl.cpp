//
// Created by lining on 5/28/21.
//
#include "MbedCtrl.h"

int main(int argc, char **argv) {
    string configFile = "./config";
    MbedCtrl *mbedCtrl = new MbedCtrl(configFile);
    mbedCtrl->SetConfig(configFile);

    delete mbedCtrl;
}