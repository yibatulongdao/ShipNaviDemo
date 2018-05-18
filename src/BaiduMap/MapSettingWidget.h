#ifndef MAPSETTINGWIDGET_H
#define MAPSETTINGWIDGET_H

#include <QMainWindow>

namespace Ui {
class MapSettingWidget;
}

class MapSettingWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapSettingWidget(QWidget *parent = 0);
    ~MapSettingWidget();

private slots:
    void on_btn_picture_clicked();

    void on_btn_location_clicked();

private:
    Ui::MapSettingWidget *ui;

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAPSETTINGWIDGET_H
