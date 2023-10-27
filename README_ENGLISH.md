# Qt6-FTP-Client-APIs-For-vsftpd

[中文 ](README.md) | [English](README_ENGLISH.md)

### Introduction
FTP client APIs for vsftpd written in Qt6

### Features
- PASV mode
- GET
- LIST

### How to use it
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

