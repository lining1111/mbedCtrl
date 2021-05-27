#!/bin/bash

#------------------------参数说明----------------------------
#--接收
#   localPath         -本地文件路径
#   remotePath        -远程文件路径
#   serverIP          -远程服务器IP
#   sftpUser          -sftp用户名
#   sftpPass          -sftp密码
#--变量
#   SYSDATE           -系统日期
#-----------------------------------------------------------

if [[ $# != 4 ]]; then
  exit
fi

# 接收参数
localPath=$1
remotePath=$2
sftpUser=$3
sftpPass=$4

BIN_NAME[0]=beeComm
#BIN_NAME[1]=logic_main_debug
#BIN_NAME[2]=pre_pay

SYSDATE=$(date +%Y%m%d)
# [函数]处理日期文件夹
createFolder() {
  cd $1
  if [[ ! -d ${SYSDATE} ]]; then
    mkdir ${SYSDATE}
    chmod 755 ${SYSDATE}
  fi
  cd ${SYSDATE}
}

# 获取文档中文件版本信息
getVersion() {
  cat $1 | while read line; do
    #echo "${line}"
    fileName=$(echo $line | cut -d ' ' -f 1)
    version=$(echo $line | cut -d ' ' -f 2)
    #echo "$fileName:$version"
    if [ $fileName == $2 ]; then
      echo $version
    fi
  done
}

getVersionTest() {
  version=$(getVersion /home/lining/version beeComm)
  echo "$version"
}

versionFile="version"

#创建更新文件保存的日期文件夹,并进入
createFolder $localPath
#获取远程version文件
DOWNLOAD="${remotePath}/${versionFile} --ftp-user=${sftpUser} --ftp-password=${sftpPass}"
wget -N ${DOWNLOAD}

#遍历要更新的文件的本地版本和远端版本
for bin in ${BIN_NAME[*]}; do

  #local file version
  local_version=$(getVersion $localPath/$versionFile $bin)
  echo "$bin local version $local_version"
  #remote file version
  remote_version=$(getVersion $localPath/$SYSDATE/$versionFile $bin)
  echo "${bin}  remote version $remote_version"

  if [ $local_version -lt $remote_version ]; then
    echo "begin update"

      #download file
  DOWNLOAD="${remotePath}/${bin} --ftp-user=${sftpUser} --ftp-password=${sftpPass}"
  wget -N ${DOWNLOAD}
  # get file
  GETFILE=${localPath}/${SYSDATE}/${bin}

  #add x
  chmod 777 ${GETFILE}

    mv ${LOCALFILE} ${LOCALFILE}${SYSDATE}
    cd ${localPath}
    cd ${SYSDATE}

    cp $GETFILE ${localPath}
  fi

done
