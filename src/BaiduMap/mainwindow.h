#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}
class QNetworkAccessManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_static_image_clicked();
    void getStaticImageFinished();
    void on_btn_query_clicked();
    void getQueryFinished();
private:
    Ui::MainWindow *ui;

    QNetworkAccessManager* m_netMgr;
    QMap<QString,QString> m_paramter;
};

#endif // MAINWINDOW_H
