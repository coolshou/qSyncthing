#include "animatedsystrayicon.h"

#include <QDebug>

AnimatedSysTrayIcon::AnimatedSysTrayIcon(QIcon icon, QWidget *parent)
{
    //parent = parent;
    _interval = 1000;
    this->mainicon = icon;
    setIcon(this->mainicon);
}
void AnimatedSysTrayIcon::schedule_timer()
{
    ani_timer = new QTimer(this);
    ani_timer->setInterval(_interval);
    //connect(ani_timer, SIGNAL() , this, SLOT(run()));

}
void AnimatedSysTrayIcon::run()
{
    //try {
        update_trayicon();
    //} catch {
        //
    //}
    /*
     * catch() {
        //Exception as e:
        //# logging.exception("Exception in running periodic thread")
        //raise("Exception in running periodic thread")
    } {
        //with self.schedule_lock:
            if (not ani_stop)
                schedule_timer();
    }*/
}
void AnimatedSysTrayIcon::update_trayicon()
{
    //"""use ani_icons to loop update trayicon """
    int total = ani_icons.length();

    ani_idx = ani_idx + 1;
    //# print("ani_idx: %s" % ani_idx)
    if (ani_idx > (total - 1))
        ani_idx = 0;
    QIcon icon = ani_icons[ani_idx];
    setIcon(icon);
}
void AnimatedSysTrayIcon::add_ani_icon(QIcon icon)
{
    ani_icons.append(icon);
}
void AnimatedSysTrayIcon::animate_start()
{
    //Mimics Thread standard start method
    int total = ani_icons.length();
    //total = len(self.ani_icons);
    //# print("total: %s" % total)

    if (total < 2)
    {
        qDebug() <<"Too few icons to do animate!!";
        return;
        /*
        try:
            raise NameError("Too few icons to do animate!!")
        finally:
            raise
            */
    }
    schedule_timer();
}
void AnimatedSysTrayIcon::animate_stop()
{
        //"""stop animate tray icon"""
        //with self.schedule_lock:
    ani_stop = true;
    if (ani_timer->isActive())
        ani_timer->stop();
    restore_icon();
}
void AnimatedSysTrayIcon::restore_icon()
{
    //"""restore to original icon"""
    ani_idx = 0;
    setIcon(mainicon);
}
