//
// Created by root on 2021/5/21.
//

#include "ota/Ota.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <openssl/sha.h>
#include <fstream>
#include <cassert>

Ota::Ota() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

Ota::~Ota() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

static size_t headerCallback(char *buffer, size_t size, size_t nitems, void *userdata) {
    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    return nitems * size;
}

static size_t discardFunc(void *ptr, size_t size, size_t nmemb, void *stream) {
    return size * nmemb;
}

static size_t readCallback(void *ptr, size_t size, size_t nmemb, void *stream) {
    FILE *f = (FILE *) stream;
    size_t n;
    if (ferror(f)) {
        return CURL_READFUNC_ABORT;
    }
    std::cout << "--- read func ---" << std::endl;
    n = fread(ptr, size, nmemb, f) * size;
    return n;
}

static size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *stream) {
    FILE *f = (FILE *) stream;
    size_t n;
    if (ferror(f)) {
        return CURL_READFUNC_ABORT;
    }
    std::cout << "--- write func ---" << std::endl;
    n = fwrite(ptr, size, nmemb, f);
    return n;
}

// work for both download and upload
static int progressCallback(void *p,
                            curl_off_t dltotal,
                            curl_off_t dlnow,
                            curl_off_t ultotal,
                            curl_off_t ulnow) {
    Ota *ftp = (Ota *) p;

    long upload_len = ulnow;
    long upload_total = ultotal;
    long download_len = dlnow;
    long download_total = dltotal;


    CURL *curl = ftp->curl;
    curl_off_t curtime = 0;

    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);

    /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
    if ((curtime - ftp->lastruntime) >= 3000000) {
        ftp->lastruntime = curtime;
        printf("TOTAL TIME: %.1f\n", curtime);
    }

    // do something to display the progress
    if (ultotal) {
        printf("UP: %ld bytes of %ld bytes\n", upload_len, upload_total);
        printf("UP progress: %0.2f\n", ((double) upload_len / (double) upload_total));
        ftp->fileProgress = upload_len;
        if (upload_len >= upload_total)
            return 1;
    }

    if (dltotal) {
        printf("DOWN: %ld bytes of %ld bytes \n", download_len, download_total);
        printf("DOWN progress: %0.2f\n", ((double) download_len / (double) download_total));
        ftp->fileProgress = download_len;
        if (download_len >= download_total)
            return 1;
    }

    return 0;
}

int Ota::Upload(const string filePath, const string url) {

    CURLcode ret = CURLE_GOT_NOTHING;
    FILE *file = nullptr;
    struct stat file_info;
    //1.获得上传文件的大小
    if (stat(filePath.c_str(), &file_info)) {
        cout << "can open file:" << filePath << " errno:" << strerror(errno) << endl;
        return -1;
    }
    fileSize = file_info.st_size;
    cout << "local file size:" << to_string(fileSize) << " bytes" << endl;

    file = fopen(filePath.c_str(), "rb");

    if (curl == nullptr) {
        return -1;
    }
    curl_off_t uploaded_len = 0;//已经上传的长度

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//上传后路径
    curl_easy_setopt(curl, CURLOPT_UPLOAD, true);//设置上传
    if (!usr.empty() && !passwd.empty()) {
        string usr_key = usr + ":" + passwd;
        curl_easy_setopt(curl, CURLOPT_USERPWD, usr_key.c_str());
    }
    if (timeout) {
        curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
    }

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &uploaded_len);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardFunc);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);//read callback
    curl_easy_setopt(curl, CURLOPT_READDATA, file);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, fileSize);
// curl_easy_setopt(curl, CURLOPT_FTPPORT, "-");
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);

    //set upload progress
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

    //    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // upload: 断点续传
    for (int i = 0; i < tryMax; i++) {
        /* are we resuming? */
        if (i) {
            /* yes */
            /* determine the length of the file already written */
            /*
            * With NOBODY and NOHEADER, libcurl will issue a SIZE
            * command, but the only way to retrieve the result is
            * to parse the returned Content-Length header. Thus,
            * getContentLengthfunc(). We need discardfunc() above
            * because HEADER will dump the headers to stdout
            * without it.
            */
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
            ret = curl_easy_perform(curl);
            if (ret != CURLE_OK)
                continue;
            curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
            fseek(file, uploaded_len, SEEK_SET);
            curl_easy_setopt(curl, CURLOPT_APPEND, 1L);
        } else {
            curl_easy_setopt(curl, CURLOPT_APPEND, 0L);
        }

        ret = curl_easy_perform(curl);
        if (ret == CURLE_OK) {
            break;
        }
    }

    fclose(file);

    int curl_state = 0;
    if (ret == CURLE_OK)
        curl_state = 1;
    else {
        fprintf(stderr, "%s\n", curl_easy_strerror(ret));
        curl_state = 0;
    }
    return curl_state;

}

int Ota::Download(const string filePath, const string url) {
    CURLcode ret = CURLE_GOT_NOTHING;
    FILE *file = nullptr;
    struct stat file_info;
    curl_off_t local_file_len = -1;
    bool use_resume = false;
    // get local file info, if success ,set resume mode
    if (stat(filePath.c_str(), &file_info) == 0) {
        local_file_len = file_info.st_size;
        use_resume = true;
    }

    file = fopen(filePath.c_str(), "ab+");

    if (file == nullptr) {
        perror(nullptr);
        return 0;
    }

    if (curl == nullptr) {
        return -1;
    }
    curl_off_t downloaded_len = 0;//已经下载的长度

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//下载路径
    curl_easy_setopt(curl, CURLOPT_UPLOAD, false);//设置下载
    if (!usr.empty() && !passwd.empty()) {
        string usr_key = usr + ":" + passwd;
        curl_easy_setopt(curl, CURLOPT_USERPWD, usr_key.c_str());
    }
    if (timeout) {
        curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
    }

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &downloaded_len);
    // 断点续传 set download resume, if use resume, set current local file length
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, use_resume ? local_file_len : 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    //set download progress
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

//    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    ret = curl_easy_perform(curl);

    fclose(file);

    int curl_state = 0;
    if (ret == CURLE_OK)
        curl_state = 1;
    else {
        fprintf(stderr, "%s\n", curl_easy_strerror(ret));
        curl_state = 0;
    }
    return curl_state;
}

int Ota::FileVerifySHA1(string fileName, string digest) {
    int ret = -1;


    SHA_CTX sha1_ctx;

    FILE *file = nullptr;

    unsigned char hex_sha1result[20] = {0};
    string sha1_result;

    char dataBuf[1024] = {0};

    int len;
    int t = 0;
    int i;

    file = fopen(fileName.c_str(), "rb");

    if (file == nullptr) {
        fprintf(stderr, "file:%s not exist\n", fileName.c_str());
        return -1;
    }

    do {
        SHA1_Init(&sha1_ctx);
        while (!feof(file)) {
            bzero(dataBuf, sizeof(dataBuf));

            len = fread(dataBuf, 1, sizeof(dataBuf), file);
            if (len) {
                t += len;
                //printf("len = [%d] 1\n", len);
                SHA1_Update(&sha1_ctx, dataBuf, len);   //将当前文件块加入并更新SHA1
            }
        }

        SHA1_Final(hex_sha1result, &sha1_ctx);       //获取SHA1

        puts("file SHA1:");
        for (i = 0; i < sizeof(hex_sha1result); i++)   //将SHA1以16进制输出
        {
            printf("%02x", (int) hex_sha1result[i]);

            char buf[3];
            bzero(buf, 3);
            sprintf(buf, "%02x", hex_sha1result[i]);
            sha1_result += buf;
        }
        puts("\n");

    } while (0);

    fclose(file);
    cout << digest << endl;
    cout << sha1_result << endl;

    if (sha1_result == digest) {
        ret = 0;
    }

    return ret;
}

int Ota::GetFileDigest(string digestFileName, map<string, string> &map_digest) {

    map_digest.clear();

    std::ifstream reader(digestFileName);
    assert(reader);

    if (reader.is_open()) {
        while (!reader.eof()) {
            std::string line;
            std::getline(reader, line);
            if (line.empty()) continue;

            const auto parse = [](const std::string &str) {
                std::string tmp, key, value;
                for (size_t i = 0, len = str.length(); i < len; ++i) {
                    const char ch = str[i];
                    if (ch == ' ') {
                        if (i > 0 && str[i - 1] != ' ' && key.empty()) {
                            key = tmp;
                            tmp.clear();
                        }
                    } else {
                        tmp.push_back(ch);
                    }
                    if (i == len - 1) {
                        value = tmp;
                    }
                }
                return std::make_pair(key, value);
            };

            auto kv = parse(line);
            map_digest[kv.first] = kv.second;
        }
        reader.close();
    }
    return 0;
}

string Ota::BackupFileByDate(string fileName) {

    string realFileName;

    const char *p = strrchr(fileName.c_str(), '/') + 1;
    realFileName = p;

    string backupFilename;

    ifstream in;

    in.open(fileName, ios::binary);

    if (in.fail()) {
        cout << "file not open:" << fileName << endl;
        backupFilename = "";
        return backupFilename;
    }

    //根据日期创建目录
    char dateStr[16];
    time_t tm = 0;
    struct tm *ptm = nullptr;
    time(&tm);
    ptm = localtime(&tm);

#define SaveDirFormat "%04d%02d%02d%02d%02d%02d" //20210524094031---2021-05-24 09:40:31

    bzero(dateStr, sizeof(dateStr));
    sprintf(dateStr, SaveDirFormat, ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday,
           ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

    string savePath;
    savePath = getcwd(nullptr, 0);
    savePath.append("/");
    savePath.append(dateStr);

    struct stat st;
    if (stat(dateStr, &st) != 0) {
        mkdir(savePath.c_str(), 0775);
    }

    backupFilename = savePath + "/" + realFileName;
    ofstream out;

    out.open(backupFilename, ios::binary);
    if (out.fail()) {
        cout << "file not open:" << backupFilename << endl;
        backupFilename = "";
        return backupFilename;
    }

    out << in.rdbuf();

    in.close();
    out.close();

    return backupFilename;
}


