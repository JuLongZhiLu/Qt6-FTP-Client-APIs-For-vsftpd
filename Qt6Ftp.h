/*
版本：0.0.1
*/
#ifndef QT6FTP_H
#define QT6FTP_H
#include <iostream>
#include <regex>
#include <QTcpSocket>
#include <vector>

class Qt6Ftp:public QObject{
private:
    int port;                           //端口号
    QString host;                       //FTP服务器地址
    QString userName;                   //用户名
    QString userPassword;               //用户密码
    QTcpSocket *controlSocket=nullptr;  //控制端口
    QTcpSocket *dataSocket=nullptr;     //数据端口
    QString mode;

protected:
    //用来存储IP地址和端口的结构体
    struct IpAndPort{
        std::string Ip;
        int port;
    };
    //从vsftpd的响应消息中读取出IP地址和端口
    IpAndPort getIpAndPortFromRespone(std::string respone);
    //用来整理通过list()获取到的目录信息
    std::vector<std::string> organizeDirectoryData(std::string respone);
    //清除数据发送完成后的系统发来的响应消息
    QString getDataTransferCompleteRespond();
    //发送命令
    void sendCommand(QString command);

public:
    //无参构造函数
    Qt6Ftp();
    //析构函数
    ~Qt6Ftp();
    //连接FTP服务器
    QString connectToFtpServer(QString host,int port);
    //输入账户
    QString user(QString userName);
    //输入密码
    QString password(QString userPassword);
    //关闭FTP连接
    void close();
    //以PASV模式连接服务器
    QString pasvMode();
    //获取目录信息
    QString list(std::vector<std::string>& respond);
    //获取文件
    QString get(QString filename,QString& respond);
};

#endif // QT6FTP_H
