#ifndef CITYCODEUTILS_H
#define CITYCODEUTILS_H

#include <qmap.h>



class CityCodeUtils
{
public:
    CityCodeUtils();
    QMap<QString,QString> cityMap = {};
    QString getCityCodeFromName (QString cityname);
    void initCitymap();
};

#endif // CITYCODEUTILS_H
