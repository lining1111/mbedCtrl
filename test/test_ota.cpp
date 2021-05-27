//
// Created by root on 2021/5/21.
//
#include "ota/Ota.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
    Ota *ota = new Ota();
    ota->usr = "lining";
    ota->passwd = "001314";
//    ota->timeout = 3;

//    std::cout << "==== upload test ====" << std::endl;
//
//    ota->Upload("/home/lining/1.jpg", "ftp://127.0.0.1/ftp/1.jpg");
//    std::cout << "==== download test ====" << std::endl;
//    ota->Download("/home/lining/1_get.jpg", "ftp://127.0.0.1/ftp/1.jpg");
//
//    ota->FileVerifySHA1("/home/lining/1.txt", "3f58e25750175e15f0575209364a8e2abed380af");

    ota->BackupFileByDate("/home/lining/1.jpg");


    delete ota;
}