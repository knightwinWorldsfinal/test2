#include "citycodeutils.h"

#include <QJsonArray>
#include <qfile.h>
#include <qjsondocument.h>
#include<QDebug>

CityCodeUtils::CityCodeUtils()
{

}

QString CityCodeUtils::getCityCodeFromName(QString cityName)
{
    //读取json文件，保存为QByteArray
    qDebug()<<"进入";
    if(cityMap.isEmpty()){
        initCitymap();
    }
    QMap<QString,QString>::iterator it =  cityMap.find(cityName);
    if(it == cityMap.end()){
        it = cityMap.find(cityName+"市");
        if(it ==cityMap.end()){
            it = cityMap.find(cityName+"县");
        }
        if(it ==cityMap.end()){
            it = cityMap.find(cityName+"区");
        }
        if(it ==cityMap.end()){
           return "";
        }
    }
    qDebug()<<it.value();
    return it.value();



}

void CityCodeUtils::initCitymap()
{
    QFile file("F:\\QT\\QTsource_code\\166-reportWeather-citys\\citycode.json");
    file.open(QIODevice::ReadOnly);
    QByteArray rawData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData);
    if(!jsonDoc.isNull() && jsonDoc.isArray()){
        QJsonArray jsonarry = jsonDoc.array();
        for (QJsonValue val: jsonarry){
            if(val.isObject()){
                QString cityname = val["city_name"].toString();
                QString cityCode = val["city_code"].toString();
                cityMap.insert(cityname,cityCode);
            }
        }
    }
}
