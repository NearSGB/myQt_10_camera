#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cqtopencvviewergl.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&show_timer, SIGNAL(timeout()), SLOT(onShowTimeout()));
}
void MainWindow::release_cap(){
    for(int i = (int)cap.size()-1; i >= 0; i--)
        if(cap[i].isOpened()) {
            cap[i].release();
            cap.pop_back();
        }

}


MainWindow::~MainWindow()
{
    release_cap();
    delete ui;
}


void MainWindow::on_exit_pushButton_clicked()
{
    this->close();
}

void MainWindow::on_stop_pushButton_clicked()
{
    isRun = false;
    show_timer.stop();
    release_cap();
}

void MainWindow::on_run_pushButton_clicked()
{
    if(isRun)
        return;
    isRun = true;
    readConfig(config_path);
    cap = std::vector<cv::VideoCapture>(camera_id_list.size());
    frames = std::vector<cv::Mat>(camera_id_list.size());
    for(int i = 0; i < camera_id_list.size(); i++)
        cap[i].open(camera_id_list[i]);

    for(int i = 0; i < camera_id_list.size(); i++) {
        camera_roi.push_back({0, 0, 100, 100});
    }
    show_timer.start(show_time_interval);
}

void MainWindow::onShowTimeout() {
    cv::Mat combine1, combine2;
    for(int i = 0; i < std::min((int)cap.size(), 6); i++)
        if(cap[i].isOpened()) {
            cap[i]>>frames[i];
            cv::resize(frames[i], frames[i], cv::Size(camera_w, camera_h));
            int row = frames[i].rows;
            int col = frames[i].cols;
            cv::Rect rect(col*camera_roi[i][0]/100, row*camera_roi[i][1]/100, col*(camera_roi[i][2]-camera_roi[i][0])/100,
                    row*(camera_roi[i][3]-camera_roi[i][1])/100);
/*  rect 用法
 * rect(a, b, c, d) -- tl (a, b) br(a+c, b+d)
*/
            if(i == 0)
                combine1 = frames[i](rect);
            else
                hconcat(combine1, frames[i](rect), combine1);
        }
    if(!combine1.empty())
        ui->opencvViewer_1->showImage(combine1);
    for(int i = 6; i < cap.size(); i++)
        if(cap[i].isOpened()) {
            cap[i]>>frames[i];
            cv::resize(frames[i], frames[i], cv::Size(camera_w, camera_h));
            int row = frames[i].rows;
            int col = frames[i].cols;
            cv::Rect rect(col*camera_roi[i][0]/100, row*camera_roi[i][1]/100, col*(camera_roi[i][2]-camera_roi[i][0])/100,
                    row*(camera_roi[i][3]-camera_roi[i][1])/100);
/*  rect 用法
 * rect(a, b, c, d) -- tl (a, b) br(a+c, b+d)
*/
            if(i == 6)
                combine2 = frames[i](rect);
            else
                hconcat(combine2, frames[i](rect), combine2);
        }
    if(!combine2.empty())
        ui->opencvViewer_2->showImage(combine2);
}


void MainWindow::readConfig(const std::string& config_path) {


}

void MainWindow::writeConfig(const std::string& config_path) {


}





void MainWindow::on_actionConfig_triggered()
{

}
