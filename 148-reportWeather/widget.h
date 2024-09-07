#ifndef WIDGET_H
#define WIDGET_H

#include <QMenu>
#include <QMouseEvent>
#include <QWidget>
#include <QDebug>
#include <QNetworkReply>
#include <QLabel>
#include"citycodeutils.h"
#include "day.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Day days[7];
    QList<QLabel *> mDateList;
    QList<QLabel *> mWeekList;
    QList<QLabel *> mIconList;
    QList<QLabel *> mWeaTypeList;
    QList<QLabel *> mAirqList;
    QList<QLabel *> mFxList;
    QList<QLabel *> mFlList;
public slots:
    void ReadHttpRply(QNetworkReply *reply);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void analysisWeatherJsonData(QByteArray data);
    void analysisWeatherJsonDatanew(QByteArray data);
    QString getCityCodeFromJson(QString cityNamefromUser);
    void updateUI();
    bool eventFilter(QObject *watched, QEvent *event) override;
    void drawTempLineHigh();
    void drawTempLineLow();



private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QMenu *menuQuit;
    QPoint mOffset;
    QNetworkReply *reply;
    QString strurl;
    QNetworkAccessManager *manager;
    CityCodeUtils citycodeutils;
    QMap<QString,QString> mTypeMap;

};
#endif // WIDGET_H
