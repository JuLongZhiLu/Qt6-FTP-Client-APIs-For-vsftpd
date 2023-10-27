/*
版本：0.0.1
*/
#include "Qt6Ftp.h"
#include <QObject>

void Qt6Ftp::sendCommand(QString command){
    QByteArray byte;
    byte=command.toUtf8();
    controlSocket->write(byte.data());
    controlSocket->waitForBytesWritten();
}

QString Qt6Ftp::getDataTransferCompleteRespond(){
    if(controlSocket->waitForReadyRead()){
        QByteArray byte;
        controlSocket->readAll();
        return byte.data();
    }else{
        return "getDataTransferCompleteRespond error";
    }
}

Qt6Ftp::IpAndPort Qt6Ftp::getIpAndPortFromRespone(std::string respond){
    //respond likes "227 Entering Passive Mode (192,168,52,141,9,137)."
    //use regex to get 192,168,52,141,9 and 137
    //respond类似于"227 Entering Passive Mode (192,168,52,141,9,137)."
    //使用正则表达式去获取到192,168,52,141,9和137这6个数字
    //qDebug()<<respond.c_str();


    IpAndPort ipAndPort;
    std::regex pattern("[0-9]+");
    std::smatch matches;
    std::string result[7];
    int i=0;


    while(std::regex_search(respond, matches, pattern)){
        result[i++]=matches.str();
        respond = matches.suffix();
    }

    ipAndPort.Ip=result[1]+"."+result[2]+"."+result[3]+"."+result[4];
    ipAndPort.port=std::stoi(result[5])*256+std::stoi(result[6]);

    return ipAndPort;
}

//get every sentence before "\r\n" from respond
//and put every sentence into std::vector<std::string>
//从respond中提出中每一句位于"\r\n"后面的句子
//并把句子放入到std::vector<std::string>中
std::vector<std::string> Qt6Ftp::organizeDirectoryData(std::string respond){
    std::regex pattern("((.+?)(?=\\r\\n))");
    std::smatch matches;
    std::vector<std::string> resultSet;

    while(std::regex_search(respond, matches, pattern)){
        resultSet.push_back(matches.str());
        respond = matches.suffix();
    }
    return resultSet;
}

Qt6Ftp::Qt6Ftp(){
    controlSocket = new QTcpSocket(this);
    dataSocket= new QTcpSocket(this);
}

Qt6Ftp::~Qt6Ftp(){
    close();

    if(controlSocket!=nullptr)
        delete controlSocket;
    if(dataSocket!=nullptr)
        delete dataSocket;
}

void Qt6Ftp::close(){
    controlSocket->close();
    dataSocket->close();
}

QString Qt6Ftp::connectToFtpServer(QString host,int port){
    this->host=host;
    this->port=port;

    controlSocket->connectToHost(host, port);

    if(controlSocket->waitForConnected()){
        controlSocket->waitForReadyRead();
        QByteArray byte = controlSocket->readAll();
        return byte.data();
    }else{
        return "connectToHost error:connecting to server failed";
    }
}

QString Qt6Ftp::user(QString userName){
    this->userName=userName;
    QString userCmd="USER "+userName+"\r\n";

    sendCommand(userCmd);

    if (controlSocket->waitForReadyRead()) {
        QByteArray byte = controlSocket->readAll();
        return byte.data();
    } else {
        return "user error";
    }
}

QString Qt6Ftp::password(QString userPassword){
    this->userPassword=userPassword;
    QString passCmd="PASS "+userPassword+"\r\n";

    sendCommand(passCmd);

    if (controlSocket->waitForReadyRead()) {
        QByteArray byte = controlSocket->readAll();
        return byte.data();
    } else {
        return "password error";
    }
}

QString Qt6Ftp::pasvMode(){
    IpAndPort ipAndPort;
    QString pasvCmd="PASV\r\n";
    QString respond;

    sendCommand(pasvCmd);

    if (controlSocket->waitForReadyRead()) {
       QByteArray byte = controlSocket->readAll();
       ipAndPort=getIpAndPortFromRespone(byte.data());
       respond=byte.data();
    } else {
       return "pasvMode error:getting ip and port failed";
    }

    dataSocket->close();
    dataSocket->connectToHost(ipAndPort.Ip.c_str(), ipAndPort.port);
    if(dataSocket->waitForConnected()){
        respond+=" | ";
        respond+="dataSocket build successfully";
    }else{
        return "pasvMode error:building dataSocket failed";
    }

    return respond;
}

QString Qt6Ftp::list(std::vector<std::string>& respond){
    QByteArray byte;
    QString listCmd="LIST\r\n";
    QString result;

    result+=pasvMode();
    result+=" | ";

    sendCommand(listCmd);

    if(controlSocket->waitForReadyRead()){
        byte=controlSocket->readAll();
        result+=byte.data();
    }else{
        return "list error:controlSocket read error";
    }

    if (dataSocket->waitForReadyRead()) {
        byte = dataSocket->readAll();
        respond=organizeDirectoryData(byte.data());
        result+=getDataTransferCompleteRespond();
    } else {
        return "list error:dataSocket read error";
    }
    return result;
}

QString Qt6Ftp::get(QString filename,QString& respond){
    QString retrCmd="RETR "+filename+"\r\n";
    QString result;

    result+=pasvMode();
    result+=" | ";

    sendCommand(retrCmd);

    if(controlSocket->waitForReadyRead()){
        QByteArray byte=controlSocket->readAll();
        result=byte.data();
    }else{
        return "get error:controlSocket read error";
    }

    if (dataSocket->waitForReadyRead()) {
        respond = dataSocket->readAll();
        result+=getDataTransferCompleteRespond();
    } else {
        return "get error:dataSocket read error";
    }
    return result;
}



