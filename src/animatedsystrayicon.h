#ifndef ANIMATEDSYSTRAYICON_H
#define ANIMATEDSYSTRAYICON_H

#include <QObject>
#include <QWidget>
#include <QIcon>
#include <QTimer>
#include <QSystemTrayIcon>

class AnimatedSysTrayIcon : public QSystemTrayIcon
{
public:
    AnimatedSysTrayIcon(QIcon icon, QWidget *parent = 0);
    void run();
    void add_ani_icon(QIcon icon);
    void animate_start();
    void animate_stop();

protected:
    void schedule_timer();
    void update_trayicon();
    void restore_icon();

private:
    int _interval;
    QTimer *ani_timer;
    bool ani_stop;
    //schedule_lock;
    QList<QIcon> ani_icons;
    int ani_idx;
    QIcon mainicon;
};

#endif // ANIMATEDSYSTRAYICON_H
