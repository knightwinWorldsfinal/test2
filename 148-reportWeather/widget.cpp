#include "widget.h"
#include "ui_widget.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QPainter>
#include <qjsonobject.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFont(QFont("Arial",15));
    this->setFixedSize(556,910);
    setWindowFlag(Qt::FramelessWindowHint);

    menuQuit = new QMenu(this);
    QAction *openAct = new QAction(QIcon(":/res/close.png"),tr("退出"),this);
    menuQuit->addAction(openAct);//在menu中添加退出action
    connect(menuQuit,&QMenu::triggered,this,[=](){
        this->close();
    });
    ui->widget_0404->installEventFilter(this);
    ui->widget_0405->installEventFilter(this);

    //Qt的Http编程
    manager = new QNetworkAccessManager(this);
    connect(manager,&QNetworkAccessManager::finished,[=](){
        qDebug()<<"manager finish !";
    });
    strurl = "http://v1.yiketianqi.com/api?unescape=1&version=v91&appid=22222442&appsecret=9OgkJZmV&ext=&cityid=";
    QUrl url(strurl);

    //    QUrl urlTianQi("");
    QNetworkRequest res(url);
    reply = manager->get(res);
    //网络请求后进行数据读取
    connect(manager,&QNetworkAccessManager::finished,this,&Widget::ReadHttpRply);

    //根据keys，设置icon的路径
    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");

    mWeekList << ui->labelday0 << ui->labelday1
              << ui->labelday2 << ui->labelday3
              << ui->labelday4 << ui->labelday5;
    mDateList << ui->labelDate0 << ui->labelDate1
              << ui->labelDate2 << ui->labelDate3
              << ui->labelDate4 << ui->labelDate5;
    mIconList << ui->label_WeatherIcon0 << ui->label_WeatherIcon1
              << ui->label_WeatherIcon2 << ui->label_WeatherIcon3
              << ui->label_WeatherIcon4 << ui->label_WeatherIcon5;
    mWeaTypeList << ui->label_Weathertype0 << ui->label_Weathertype1
                 << ui->label_Weathertype2 << ui->label_Weathertype3
                 << ui->label_Weathertype4 << ui->label_Weathertype5;
    mAirqList << ui->label_airq0 << ui->label_airq1
              << ui->label_airq2 << ui->label_airq3
              << ui->label_airq4 << ui->label_airq5;
    mFxList << ui->label_FX0 << ui->label_FX1
            << ui->label_FX2 << ui->label_FX3
            << ui->label_FX4 << ui->label_FX5;
    mFlList << ui->label_FL0 << ui->label_FL1
            << ui->label_FL2 << ui->label_FL3
            << ui->label_FL4 << ui->label_FL5;


}
/* QMenu menu;
 QAction *at = actions[0]; // Assumes actions is not empty
 foreach (QAction *a, actions)
     menu.addAction(a);
 menu.exec(pos, at);*/

Widget::~Widget()
{
    delete ui;
}


void Widget::ReadHttpRply(QNetworkReply *reply)
{
    int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() == QNetworkReply::NoError && resCode ==200){
        QByteArray data = reply->readAll();
        analysisWeatherJsonDatanew(data);
//        analysisWeatherJsonData(data);
        qDebug()<<QString::fromUtf8(data);
    }else{
        qDebug()<< "请求失败："<<reply->errorString();
    }

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        menuQuit->exec(QCursor::pos()); //QCursor::pos()为鼠标当前位置，否则menu位置会很远
    }
    if(event->button() == Qt::LeftButton){
        mOffset = event->globalPos()-this->pos();//计算出鼠标位置与控件位置的偏移
    }
}


void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mOffset);//鼠标按下左键移动时控件跟随鼠标
}

//解析api获得的json数据
void Widget::analysisWeatherJsonData(QByteArray data)
{
    QJsonDocument jsondoc = QJsonDocument::fromJson(data);
    if(jsondoc.isObject() && !jsondoc.isNull()){
        //解析日期
        QJsonObject jsonobj = jsondoc.object();
        QString date = jsonobj["date"].toString();
        QString week = jsonobj["week"].toString();
        ui->label_CurrentDate->setText(date+"  "+week);
        //解析城市名称
        QString cityname = jsonobj["city"].toString();
        ui->label_cityname->setText(cityname+"市");
        //解析温度
        QString tem = jsonobj["tem"].toString();
        QString tem1 = jsonobj["tem1"].toString();
        QString tem2 = jsonobj["tem2"].toString();
        ui->label_nowtemperature->setText(tem);
        ui->label_temprange->setText(tem2+"~"+tem1+"℃");
        //解析天气类型、风向
        QString wea = jsonobj["wea"].toString();
        QString win = jsonobj["win"].toString();
        QString win_speed = jsonobj["win_speed"].toString();
        //设置第二栏
        ui->label_weathertype->setText(wea);
        ui->labelWeather->setPixmap(mTypeMap[wea]);
        //设置第三栏
        ui->labelFXtype->setText(win);
        ui->labelFXlevel->setText(win_speed);
        ui->labelPMdata->setText(jsonobj["air_pm25"].toString());
        ui->labelairdata->setText(jsonobj["air_level"].toString());
        ui->labelShiduvalue->setText(jsonobj["humidity"].toString());

    }
}

void Widget::analysisWeatherJsonDatanew(QByteArray data)
{
    QJsonDocument jsondoc = QJsonDocument::fromJson(data);
    QJsonArray newdata = jsondoc["data"].toArray();
    QJsonObject aqi = jsondoc["aqi"].toObject();
    days[0].mPm25 = aqi["pm25"].toString();
    days[0].mAirq = aqi["pm25_desc"].toString();
    days[0].mCity = jsondoc["city"].toString();
    for(int i=0;i<newdata.size();i++){
        //解析日期
        QJsonObject obj = newdata[i].toObject();
        days[i].mDate = obj["date"].toString();
        days[i].mWeek = obj["week"].toString();
        days[i].mWeathType = obj["wea"].toString();
        days[i].mTemp = obj["tem"].toString();
        days[i].mTempLow = obj["tem2"].toString();
        days[i].mTempHigh = obj["tem1"].toString();
        days[i].mFx = obj["win"].toArray()[0].toString();
        days[i].mFl = obj["win_speed"].toString();
        days[i].mAirq = obj["air_level"].toString();
        days[i].mTips = obj["air_tips"].toString();
        days[i].mHu = obj["humidity"].toString();
    }
    ui->label_CurrentDate->setText(days[0].mDate+"  "+days[0].mWeek);
    //解析城市名称
    ui->label_cityname->setText(days[0].mCity+"市");
    //解析温度
    ui->label_nowtemperature->setText(days[0].mTemp);
    ui->label_temprange->setText(days[0].mTempLow+"～"+days[0].mTempHigh+"℃");
    //解析天气类型、风向
    ui->labelFXtype->setText(days[0].mFx);
    ui->labelFXlevel->setText(days[0].mFl);
    //设置第二栏
    ui->label_weathertype->setText(days[0].mWeathType);
    ui->labelWeather->setPixmap(mTypeMap[days[0].mWeathType]);
    //设置第三栏
    ui->labelShiduvalue->setText(days[0].mHu);
    //更新UI
    updateUI();
}

void Widget::updateUI(){

    QPixmap pixmap;
    QString str1;
    //设置日期
    ui->label_CurrentDate->setText(days[0].mDate+"  "+days[0].mWeek);
    //解析城市名称
    ui->label_cityname->setText(days[0].mCity+"市");
    //解析温度
    ui->label_nowtemperature->setText(days[0].mTemp);
    ui->label_temprange->setText(days[0].mTempLow+"～"+days[0].mTempHigh+"℃");
    //解析天气类型、风向
    ui->labelFXtype->setText(days[0].mFx);
    ui->labelFXlevel->setText(days[0].mFl);
    //设置第二栏
    int index0 = days[0].mWeathType.indexOf("转");
    if(index0 != -1){
        str1 = days[0].mWeathType.left(index0);
    }else{
        str1 = days[0].mWeathType;
    }
    ui->label_weathertype->setText(days[0].mWeathType);
    ui->labelWeather->setPixmap(mTypeMap[days[0].mWeathType.left(index0)]);
    //设置第三栏
    ui->labelPMdata->setText(days[0].mPm25);
    ui->labelairdata->setText(days[0].mAirq);
    ui->labelShiduvalue->setText(days[0].mHu);

    for(int i = 0; i < 6; i++){
        mWeekList[i]->setText(days[i].mWeek);

        mDateList[i]->setText(days[i].mDate);
        mIconList[i]->setPixmap(mTypeMap[days[i].mWeathType]);
        mWeekList[i]->setText(days[i].mWeathType);
        mAirqList[i]->setText(days[i].mAirq);
        mFlList[i]->setText(days[i].mFl);
        mFxList[i]->setText(days[i].mFx);
    }

    for(int i = 0; i < 6; i++){


        mWeekList[i]->setText(days[i].mWeek);
        mWeekList[0]->setText("今天");
        mWeekList[1]->setText("明天");
        mWeekList[2]->setText("后天");
        QStringList dayList = days[i].mDate.split("-");
        mDateList[i]->setText(dayList.at(1)+"-"+dayList.at(2));//2024-01-24

        //缩放图片大小和label大小能匹配
        int index = days[i].mWeathType.indexOf("转");
        if(index != -1){
            pixmap = mTypeMap[days[i].mWeathType.left(index)];
        }else{
            pixmap = mTypeMap[days[i].mWeathType];
        }
        pixmap = pixmap.scaled(mIconList[i]->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        mIconList[i]->setMaximumHeight(50);
        mIconList[i]->setMaximumWidth(ui->widget_2->width()/6.5);
        mIconList[i]->setPixmap(pixmap);
        mWeaTypeList[i]->setText(days[i].mWeathType);

        QString airQ = days[i].mAirq;
        mAirqList[i]->setText(airQ);
        if( airQ == "优"){
             mAirqList[i]->setStyleSheet("background-color: rgb(150, 213, 32); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "良"){
             mAirqList[i]->setStyleSheet("background-color: rgb(208, 107, 39); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "轻度"){
             mAirqList[i]->setStyleSheet("background-color: rgb(255, 199, 199); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "中度"){
             mAirqList[i]->setStyleSheet("background-color: rgb(255, 17, 17); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "重度"){
             mAirqList[i]->setStyleSheet("background-color: rgb(153, 0, 0); border-radius: 7px; color: rgb(230, 230, 230)");
        }



        mFxList[i]->setText(days[i].mFx);
        index = days[i].mFl.indexOf("转");
        if(index != -1){
            mFlList[i]->setText(days[i].mFl.left(index));
        }else{
            mFlList[i]->setText(days[i].mFl);
        }

    }
    update();
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget_0404 && event->type() == QEvent::Paint){
        drawTempLineHigh();
        return true;//表示事件已被处理
    }
    if(watched == ui->widget_0405 && event->type() == QEvent::Paint){
         drawTempLineLow();
         return true;//表示事件已被处理
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::drawTempLineHigh()
{
    QPainter painter(ui->widget_0404);
    painter.setPen(Qt::yellow);

    int ave;
    int sum = 0;
    int offset = 0;
    int middle = ui->widget_0404->height()/2;
    for(int i=0;i<6;i++){
        sum+=days[i].mTempHigh.toInt();
    }
    ave = sum/6;
    qDebug()<<days[1].mTempHigh.toInt();
    qDebug()<<ave;
    //定义出6个点
    QPoint points[6];
    for(int i=0;i<6;i++){
        points[i].setX(mAirqList[i]->x()+mAirqList[i]->width()/2);
        offset = (days[i].mTempHigh.toUInt() - ave)*2;
        points[i].setY(middle - offset);
        painter.drawEllipse(QPoint(points[i]),4,4);
        //画出6天温度
        painter.drawText(points[i].x()-15,points[i].y()-15,days[i].mTempHigh+"℃");
    }
    for(int i=0;i<5;i++){
        painter.drawLine(points[i],points[i+1]);
    }
}

void Widget::drawTempLineLow(){
    QPainter painter(ui->widget_0405);
    painter.setPen(Qt::blue);

    int ave;
    int sum = 0;
    int offset = 0;
    int middle = ui->widget_0405->height()/2;
    for(int i=0;i<6;i++){
        sum+=days[i].mTempLow.toInt();
    }
    ave = sum/6;
    qDebug()<<days[1].mTempLow.toInt();
    qDebug()<<ave;
    //定义出6个点
    QPoint points[6];
    for(int i=0;i<6;i++){
        points[i].setX(mAirqList[i]->x()+mAirqList[i]->width()/2);
        offset = (days[i].mTempLow.toUInt() - ave)*2;
        points[i].setY(middle - offset);
        painter.drawEllipse(QPoint(points[i]),4,4);
        //画出6天温度
        painter.drawText(points[i].x()-15,points[i].y()-15,days[i].mTempLow+"℃");
    }
    for(int i=0;i<5;i++){
        painter.drawLine(points[i],points[i+1]);
    }

}

void Widget::on_pushButton_clicked()
{
    QString cityNamefromUser = ui->lineEdit->text();
    QString cityCode = citycodeutils.getCityCodeFromName(cityNamefromUser);
    if(cityCode!=""){
        strurl += "&cityid="+cityCode;
        manager->get(QNetworkRequest(QUrl(strurl)));
    }else{
        QMessageBox mes;
        mes.setWindowTitle("错误");
        mes.setText("请输入正确的名称");
        mes.setStyleSheet("QPushButton{color:bule}");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
    }

}
