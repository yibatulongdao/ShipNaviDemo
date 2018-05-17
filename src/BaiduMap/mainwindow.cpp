#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QImage>
#include <QBitmap>
#include <QPixmap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_netMgr = new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_static_image_clicked()
{
    m_paramter["center"] =ui->center->text();
    m_paramter["width"] = QString::number(ui->staticImage->width());
    m_paramter["height"] = QString::number(ui->staticImage->height());
    m_paramter["zoom"] = QString::number(ui->zoom->value());
    m_paramter["mcode"] = ui->mcode->text();

    QStringList paramters;
    foreach (QString key, m_paramter.keys()) {
        paramters << QString("%1=%2").arg(key,m_paramter.value(key));
    }
    QString strUrl = ui->url->text().toHtmlEscaped();
    strUrl+= paramters.join("&");


    QNetworkRequest req;

    req.setUrl(QUrl(strUrl));
    QNetworkReply* pReply = m_netMgr->get(req);
    connect(pReply,SIGNAL(finished()),SLOT(getStaticImageFinished()));
}

void MainWindow::getStaticImageFinished()
{
    QNetworkReply* pReply = qobject_cast<QNetworkReply*>(sender());
    if(pReply)
    {
        QByteArray bz = pReply->readAll();
        QImage imgSrc = QImage::fromData(bz);
        if(!imgSrc.isNull())
        {
            ui->staticImage->setPixmap(QPixmap::fromImage(imgSrc));
        }
    }
}

void MainWindow::on_btn_query_clicked()
{
    QString strUrl = ui->queryUrl->text();//.toHtmlEscaped();
    QNetworkRequest req;

    req.setUrl(QUrl(strUrl));
    QNetworkReply* pReply = m_netMgr->get(req);
    connect(pReply,SIGNAL(finished()),SLOT(getQueryFinished()));
}

void MainWindow::getQueryFinished()
{
    QNetworkReply* pReply = qobject_cast<QNetworkReply*>(sender());
    if(pReply)
    {
        QByteArray bzjson = pReply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(bzjson);
        QByteArray beautyjson = doc.toJson(QJsonDocument::Indented);
        ui->query_result->setText(QString(bzjson));
    }
}
