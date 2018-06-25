#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QShortcut>
#include <QMessageBox>
#include <QApplication>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tf = QTransform();
    pixmap_syncthingui = QPixmap(":/images/qSyncthing.svg");
    pixmap_syncthingui0 = QPixmap(":/images/qSyncthing.svg");
    tf.rotate(90.0);
    pixmap_syncthingui1 = pixmap_syncthingui0.transformed(tf);
    tf.rotate(180.0);
    pixmap_syncthingui2 = pixmap_syncthingui0.transformed(tf);
    tf.rotate(270.0);
    pixmap_syncthingui3 = pixmap_syncthingui0.transformed(tf);

    process = new QProcess(this);
    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(process_failed(QProcess::ProcessError)));
    //# QProcess emits `readyRead` when there is data to be read
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(process_dataReady()));
    connect(process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(process_stateChanged(QProcess::ProcessState)));

    init_gui();
    init_systray();
    run();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (tray->isVisible()){
        if (tray->supportsMessages()){
            tray->showMessage("Info",
                                  "The program will keep running in the "
                                  "system tray. To terminate the program,"
                                  " choose <b>Quit</b> in the context "
                                  "menu of the system tray entry.");
        }else{
            qDebug() << " System tray not supports balloon messages ";
        }
        hide();
        event->ignore();
    }
}
QString MainWindow::process_failed(QProcess::ProcessError state)
{
    //"""Read and return errors."""
    QString t = "ERROR:Fail:Syncthing blow up in pieces!";
    ui->statusBar->showMessage(t);
    qDebug() << t << "Wheres your God now?" << state;
    //return QString(process->readAllStandardError()).strip().lower();
    return QString(process->readAllStandardError());
}
void MainWindow::process_dataReady()
{
        //"""get process stdout/strerr when data ready"""

        QString msg = QString(process->readAll());
        if (msg != "")
        {
            //msg=msg.replace("\\n","");
            msg=msg.remove("\n");
            ui->consoletextedit->append(msg);
        }
        //# TODO: format the msg to remove extra b and \n
        /*
        QString lines = msg.split("'");
        QString tmp = lines[1];
        tmp = tmp.splitlines(0);
        lines = tmp[0].split("\\n");
        */
        /*TODO
        for (line in lines)
        {
            if (line != "")
            {
                //# print("1: %s" % line)
                ui->consoletextedit.append(line);
            }
        }*/
        ui->consoletextedit->ensureCursorVisible();
        //# autoscroll to last line's first character
        ui->consoletextedit->moveCursor(QTextCursor::End);
        ui->consoletextedit->moveCursor(QTextCursor::StartOfLine);

}

void MainWindow::process_stateChanged(QProcess::ProcessState state)
{
    //""" procress_stateChanged """
    //# TODO handle procress_stateChanged
    qDebug() <<"procress_stateChanged: " << state ;//<<" \n sig:" << sig;

}
void MainWindow::init_gui()
{
    progressbar = new QProgressBar(this);
    progressbar->setVisible(false);
    //todo: get SYNCTHING + ' --version'
    ui->statusBar->addPermanentWidget(progressbar);
    setMinimumSize(900, 600);
    setMaximumSize(1280, 1024);
    resize(minimumSize());
    //center();

    //QWebEngineView
    connect(ui->webEngineView, SIGNAL(loadStarted()),this, SLOT(start_loading()));
    connect(ui->webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(finish_loading(bool)));
    connect(ui->webEngineView, SIGNAL(loadProgress(int)), this, SLOT(loading(int)));
    connect(ui->webEngineView, SIGNAL(titleChanged(QString)), this, SLOT(set_title(QString)));
    connect(ui->webEngineView->page(), SIGNAL(linkHovered(QString)), this, SLOT(set_title(QString)));

    QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), this, SLOT(setZoomIn()));
    QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this, SLOT(setZoomOut()));
    QShortcut(QKeySequence(Qt::CTRL + Qt::Key_0), this, SLOT(setZoomDef()));
    QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(app_exit()));

    //# backend options
    chrt = new QCheckBox("Smooth CPU ", this);
    chrt->setChecked(true);
    ionice = new QCheckBox("Smooth HDD ", this);
    ionice->setChecked(true);
    chrt->setToolTip("Use Smooth CPUs priority (recommended)");
    ionice->setToolTip("Use Smooth HDDs priority (recommended)");
    chrt->setStatusTip(chrt->toolTip());
    ionice->setStatusTip(ionice->toolTip());

    ui->mainToolBar->addAction(QIcon(":/images/stop.svg"),
                               "Stop Sync", this ,SLOT(syncthing_stop()));
    ui->mainToolBar->addAction(QIcon(":/images/start.svg"),
                               "Restart Sync", this ,SLOT(run()));
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(chrt);
    ui->mainToolBar->addWidget(ionice);

}
void MainWindow::setZoomIn()
{
    ui->webEngineView->setZoomFactor(ui->webEngineView->zoomFactor() + 0.2);
}
void MainWindow::setZoomOut()
{
    ui->webEngineView->setZoomFactor(ui->webEngineView->zoomFactor() - 0.2);
}
void MainWindow::setZoomDef()
{
    ui->webEngineView->setZoomFactor(1);
}
void MainWindow::start_loading()
{
    //"""show progressbar when downloading data"""
    progressbar->show();
}
void MainWindow::loading(int progress)
{
    progressbar->setValue(progress);
}
void MainWindow::finish_loading(bool finished)
{
    //"""Finished loading content."""
    if (not finished){
        //# TODO: When loading fail, what should we do?
        qDebug() <<"load fail";
        if (process->state() == QProcess::NotRunning)
        {
            run();
            ui->webEngineView->reload();
        }
        //# if self.process.state != QProcess.Running:
        //#    print("syncthing is not running: %s" % self.process.state())
        //# pass
    }
    qDebug() <<"finish_loading: " << finished;
    //# TODO: WebEngineView does not have following function?
    //# self.view.settings().clearMemoryCaches()
    //# self.view.settings().clearIconDatabase()

    //# print("finish_loading %s" % datetime.strftime(datetime.now(),
    //#                                              '%Y-%m-%d %H:%M:%S'))
    //# TODO: following line need 6 sec to finish!!
    //# TODO: (" INFO: Loading Web UI increases >250Mb RAM!.")
    //# self.view.page().mainFrame().evaluateJavaScript(BASE_JS)
    //# print("finish_loading %s" % datetime.strftime(datetime.now(),
    //#                                             '%Y-%m-%d %H:%M:%S'))
    progressbar->hide();
}

void MainWindow::set_title(QString title)
{
    if (title.length() > 0)
        setWindowTitle(title);
}
void MainWindow::on_actionExit_triggered()
{
    close();
}
void MainWindow::syncthing_stop()
{
    //"""syncthing stop"""
    qDebug() << "try to stop syncthing";
    process->kill();
    //
    QByteArray ba = QString(SYNCTHING).toLocal8Bit();
    char *c_str2 = ba.data();
    GetPIDbyName(c_str2);
    //kill exist
    /*
        print("try to stop syncthing")
        self.process.kill()
        # check there is no other syncthing is running!
        for proc in psutil.process_iter():
            # check whether the process name matches
            # print("procress: %s " % proc.name())
            if proc.name() == SYNCTHING:
                proc.kill()
                */
}
#ifdef Q_OS_WIN32
void MainWindow::killProcessByName(const char *filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                          (DWORD) pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}
#endif
void MainWindow::run()
{
    //"""Run bitch run!."""
    //# Stop first!
    syncthing_stop();

    QString command_to_run_syncthing = "";
    if (ionice->isChecked())
        command_to_run_syncthing.append("ionice --ignore --class 3 ");
    if (chrt->isChecked())
        command_to_run_syncthing.append("chrt --verbose --idle 0 ");

    QString t = QString(SYNCTHING).append(" -no-browser");
    command_to_run_syncthing.append(t);
    qDebug() << "command_to_run_syncthing: " << command_to_run_syncthing;

    process->start(command_to_run_syncthing);
    if (not process->waitForStarted())
        process_failed(QProcess::FailedToStart);

}

void MainWindow::on_actionStart_Syncronization_triggered()
{
    run();
}

void MainWindow::on_actionStop_Syncronization_triggered()
{
    syncthing_stop();
}

void MainWindow::on_actionOpen_in_external_browser_triggered()
{
    //open_new_tab(URL);
    QDesktopServices::openUrl(QUrl(URL));
}
void MainWindow::init_systray()
{
    tray = new AnimatedSysTrayIcon(QIcon(pixmap_syncthingui),1, this);
    tray->add_ani_icon(QIcon(pixmap_syncthingui0));
    tray->add_ani_icon(QIcon(pixmap_syncthingui1));
    tray->add_ani_icon(QIcon(pixmap_syncthingui2));
    tray->add_ani_icon(QIcon(pixmap_syncthingui3));


    //tray->setToolTip(__doc__.strip().capitalize());
    /*
    //# to test animate
    //# traymenu.addAction("start", lambda: self.tray.animate_start())
    //# traymenu.addAction("stop", lambda: self.tray.animate_stop())
    //# traymenu.addSeparator()
    //# traymenu.addAction("Open Web", lambda: open_new_tab(URL))
    //# traymenu.addAction("Quit All", lambda: self.close())
    */
    connect(tray, SIGNAL(startSync()), this, SLOT(run()));
    connect(tray, SIGNAL(stopSync()), this, SLOT(syncthing_stop()));
    connect(tray, SIGNAL(hideGUI()), this, SLOT(hide()));
    connect(tray, SIGNAL(showGUI()), this, SLOT(show_gui()));
    connect(tray, SIGNAL(exitGUI()), this, SLOT(app_exit()));
    //tray->show();
    tray->setVisible(true);

}
void MainWindow::app_exit()
{
    QString sInfo =  QString(L'Quit ').append(SYNCTHING).append(L' ?');
    bool the_conditional_is_true = QMessageBox::question(
        this, SYNCTHING, sInfo,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No) == QMessageBox::Yes;
    if (the_conditional_is_true){
        syncthing_stop();
        tray->stop_animate();
        exit(0);
    }else{
        //tray->setVisible(true);
    }
}
QStringList MainWindow::GetPIDbyName(char* ps_name)
{
    //getAllRunningProcesses();
#ifdef Q_OS_LINUX
    //linux
    QStringList ls;
    size_t iSize;
    FILE *fp;
    char *rs=(char*)calloc(1,200);
    char *cmd=(char*)calloc(1,200);
    sprintf(cmd,"pidof %s",ps_name);
    fp=popen(cmd,"r");
    iSize = fread(rs,1,200,fp);
    fclose(fp);
    if (iSize>0){
        //qDebug() << "GetPIDbyName: " << rs ;
        QString qstr = QString(QLatin1String(rs));
        //qDebug() << qstr;
         ls= qstr.simplified().split(" ");
        /*
        for (int i=0; i< ls.count(); i++){
            qDebug() << "1: " <<ls[i];
        }*/
        //return atoi(rs);
    }
    return ls;

#elif Q_OS_WIN32
    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.setReadChannelMode(QProcess::MergedChannels);
//    process.start("cmd.exe /C echo test");
    process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");

    process.waitForStarted(1000);
    process.waitForFinished(1000);

    QByteArray list = process.readAll();
    qDebug() << "Read" << list.length() << "bytes";
    qDebug() << list;
#endif
}
void MainWindow::show_gui()
{
    showNormal();
    //# webview require 70Mb to show webpage
    ui->webEngineView->load(QUrl(URL));
}
