#ifndef ANIMATEDSYSTRAYICON_H
#define ANIMATEDSYSTRAYICON_H

#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QList>
#include <QIcon>
#include <QMenu>

class AnimatedSysTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    AnimatedSysTrayIcon(const QIcon &icon, int interval=1, QWidget *parent = 0);
    void add_ani_icon(QIcon icon);
    void restore_icon();
    void start_animate();
    void stop_animate();
    void schedule_timer(int interval);

public slots:
    void onAniTimerTimeout();
    void stop();
    void start();
    void doShowGUI();
    void doHideGUI();
    void trayExit();
signals:
    void stopSync();
    void startSync();
    void showGUI();
    void hideGUI();
    void exitGUI();

protected:
    void update_trayicon();
private:
    QIcon mainicon;
    QTimer *ani_timer;
    bool ani_stop;
    QList<QIcon> ani_icons;
    int ani_idx;
    int _interval;
    QMenu *traymenu;
};

#endif // ANIMATEDSYSTRAYICON_H
