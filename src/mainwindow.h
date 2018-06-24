#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QProgressBar>
#include <QProcess>
#include <QMenu>
#include <QCheckBox>
#include <QCloseEvent>

#include "const.h"
#include "animatedsystrayicon.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    void init_gui();
    void init_systray();
    QStringList GetPIDbyName(char* ps_name);
    #ifdef Q_OS_WIN32
    void killProcessByName(const char *filename);
    #endif

private slots:
    void app_exit();
    void run();
    void show_gui();
    void syncthing_stop();
    void on_actionExit_triggered();
    void on_actionStart_Syncronization_triggered();
    void on_actionStop_Syncronization_triggered();
    void on_actionOpen_in_external_browser_triggered();
    void start_loading();
    void loading(int progress);
    void finish_loading(bool finished);
    void set_title(QString title);
    QString process_failed(QProcess::ProcessError state);
    void process_dataReady();
    void process_stateChanged(QProcess::ProcessState state);
    void setZoomIn();
    void setZoomOut();
    void setZoomDef();

private:
    Ui::MainWindow *ui;
    QTransform tf;
    QPixmap pixmap_syncthingui;
    QPixmap pixmap_syncthingui0;
    QPixmap pixmap_syncthingui1;
    QPixmap pixmap_syncthingui2;
    QPixmap pixmap_syncthingui3;
    QProcess *process;
    QProgressBar *progressbar;
    AnimatedSysTrayIcon *tray;
    QMenu *traymenu;
    QCheckBox *chrt;
    QCheckBox *ionice;
};

#endif // MAINWINDOW_H
