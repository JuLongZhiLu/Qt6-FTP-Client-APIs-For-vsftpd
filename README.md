# Qt6-FTP-Client-APIs-For-vsftpd

[中文 ](README.md) | [English](README_ENGLISH.md)

### 简介
用Qt6写的针对vsftpd的FTP客户端APIs

### 目前支持的功能
- PASV模式
- GET
- LIST

### 如何使用
```cpp
    #include "Qt6Ftp.h"

    Qt6Ftp ftp;
    QString respond;

    respond=ftp.connectToFtpServer("192.168.52.142",21);
    qDebug()<<respond;

    respond=ftp.user("ftpuser");
    qDebug()<<respond;

    respond=ftp.password("ubuntu");
    qDebug()<<respond;

    std::vector<std::string> items;
    respond=ftp.list(items);
    qDebug()<<respond;
    for(auto elem:items){
        qDebug()<<elem.c_str();
    }

    QString fileContent;
    respond=ftp.get("003.txt",fileContent);
    qDebug()<<respond;
    qDebug()<<fileContent;

    respond=ftp.list(items);
    qDebug()<<respond;
    for(auto elem:items){
        qDebug()<<elem.c_str();
    }

    ftp.close();
```
![image](https://github.com/JuLongZhiLu/Qt6-FTP-Client-APIs-For-vsftpd/assets/130359590/79c9377c-afb1-44d6-9546-864c0a21401b)

