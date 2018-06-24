#include "animatedsystrayicon.h"
#include <QVariant>

#include <QDebug>


AnimatedSysTrayIcon::AnimatedSysTrayIcon(const QIcon &icon, int interval, QWidget *parent)
{
    this->setParent(parent);
    this->mainicon = icon;
    setIcon(icon);
    ani_timer = new QTimer(this);
    ani_timer->setInterval(interval*1000);
    ani_stop = false;
    ani_idx = 0;
    _interval = interval;
    //menu
    traymenu = new QMenu(parent);
    traymenu->addAction("qSuncthing")->setDisabled(true);
    traymenu->addSeparator();
    traymenu->addAction("Stop Sync", this ,SLOT(stop()));
    traymenu->addAction("Restart Sync", this ,SLOT(start()));
    traymenu->addSeparator();
    traymenu->addAction("Show", this ,SLOT(doShowGUI()));
    traymenu->addAction("Hide", this ,SLOT(doHideGUI()));
    traymenu->addSeparator();
    //# traymenu.addAction("Open Web", lambda: open_new_tab(URL))
    //# traymenu.addAction("Quit All", lambda: self.close())
    traymenu->addAction("Quit All", this ,SLOT(trayExit()));

    //traymenu->show();
    setContextMenu(traymenu);
}

void AnimatedSysTrayIcon::add_ani_icon(QIcon icon)
{
    this->ani_icons.append(icon);
}

void AnimatedSysTrayIcon::restore_icon()
{
    ani_idx = 0;
    setIcon(mainicon);
}

void AnimatedSysTrayIcon::start_animate()
{
    int total = ani_icons.length();

    if (total < 2){
        qDebug() << "Too few icons to do animate!!";
        /*
        try
            raise NameError("Too few icons to do animate!!");
        finally:
            raise;
            */
        return;
    }
    schedule_timer(_interval);
}

void AnimatedSysTrayIcon::stop_animate()
{
    ani_stop = true;
    if (ani_timer->isActive())
        ani_timer->stop();
    restore_icon();
}

void AnimatedSysTrayIcon::schedule_timer(int interval)
{
    if (!ani_timer)
            ani_timer = new QTimer(this);
    ani_timer->setInterval(interval);
    connect(ani_timer,SIGNAL(timeout()), this, SLOT(onAniTimerTimeout()));
}

void AnimatedSysTrayIcon::onAniTimerTimeout()
{
    update_trayicon();
    if (! ani_stop)
        schedule_timer(_interval);
}

void AnimatedSysTrayIcon::stop()
{
    emit stopSync();
}

void AnimatedSysTrayIcon::start()
{
    emit startSync();
}

void AnimatedSysTrayIcon::doShowGUI()
{
    emit showGUI();
}

void AnimatedSysTrayIcon::doHideGUI()
{
    emit hideGUI();
}

void AnimatedSysTrayIcon::trayExit()
{
    //setVisible(false);
    emit exitGUI();
}

void AnimatedSysTrayIcon::update_trayicon()
{
    int total = ani_icons.length();

    ani_idx = ani_idx + 1;
    //# print("ani_idx: %s" % self.ani_idx)
    if (ani_idx > (total - 1))
        ani_idx = 0;
    QIcon icon = ani_icons[ani_idx];
    setIcon(icon);
}
