#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cqtopencvviewergl.h"
#include <time.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    camera_roi = std::vector<std::vector<int>>(4);
    ui->setupUi(this);
    connect(&show_timer, SIGNAL(timeout()), SLOT(onShowTimeout()));
}
void MainWindow::release_cap(){
    for(int i = (int)cap.size()-1; i >= 0; i--) {
        if(cap[i].isOpened())
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
    ui->opencvViewer_1->showImage(cv::Mat());
    ui->opencvViewer_2->showImage(cv::Mat());
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

    show_timer.start(show_time_interval);
}



void MainWindow::show_num_camera(int id, int num, CQtOpenCVViewerGl* cvViewer)
{
    if(num <= 0)
        return ;
    cv::Mat combine1;
//    #pragma omp parallel for
    for(int i = id; i < std::min((int)cap.size(), id+num); i++)
        if(cap[i].isOpened()) {
            cap[i]>>frames[i];//开销很大
            cv::resize(frames[i], frames[i], cv::Size(camera_w, camera_h));
        }

    for(int i = id; i < std::min((int)cap.size(), id+num); i++)
        if(cap[i].isOpened()) {
            int row = frames[i].rows;
            int col = frames[i].cols;
            cv::Rect rect(col*camera_roi[0][i]/100, row*camera_roi[1][i]/100, col*(camera_roi[2][i]-camera_roi[0][i])/100,
                    row*(camera_roi[3][i]-camera_roi[1][i])/100);
/*  rect 用法
 * rect(a, b, c, d) -- tl (a, b) br(a+c, b+d)
*/
            if(i == id)
                combine1 = frames[i](rect);
            else
                hconcat(combine1, frames[i](rect), combine1);
        }

    if(!combine1.empty())
        cvViewer->showImage(combine1);
}

void MainWindow::onShowTimeout() {
    show_num_camera(0, 4, ui->opencvViewer_1);
    show_num_camera(4, 4, ui->opencvViewer_2);
    show_num_camera(8, 2, ui->opencvViewer_3);

}


void MainWindow::getVectorFromXML(tinyxml2::XMLElement *config, const char* element, std::vector<int>& vec)
{
    vec.clear();
    std::string tmp(element);
    for(int i = 0; i < tmp.size(); ) {
        int j = tmp.find(',', i);
        if(j == std::string::npos) {
            vec.push_back(std::stoi(tmp.substr(i)));
            break;
        }
        else {
            vec.push_back(std::stoi(tmp.substr(i, j-i)));
            i = j+1;
        }
    }
}

void MainWindow::readConfig(const char* config_path) {
    tinyxml2::XMLDocument doc;
    if(doc.LoadFile(config_path) != 0)
        return;
    ui->config_textEdit->clear();
    tinyxml2::XMLElement *config = doc.RootElement();
    const char* tmp = config->FirstChildElement("camera_num")->GetText();
    camera_num = std::atoi(tmp);
    ui->config_textEdit->append("camera_num: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("camera_w")->GetText();
    camera_w = std::atoi(tmp);
    ui->config_textEdit->append("camera_w: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("camera_h")->GetText();
    camera_h = std::atoi(tmp);
    ui->config_textEdit->append("camera_h: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("show_time_interval")->GetText();
    show_time_interval = std::atoi(tmp);
    ui->config_textEdit->append("show_time_interval: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("camera_id_list")->GetText();
    getVectorFromXML(config, tmp, camera_id_list);
    ui->config_textEdit->append("camera_id_list: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("roi_left_top_x")->GetText();
    getVectorFromXML(config, tmp, camera_roi[0]);
    ui->config_textEdit->append("roi_left_top_x: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("roi_left_top_y")->GetText();
    getVectorFromXML(config, tmp, camera_roi[1]);
    ui->config_textEdit->append("roi_left_top_y: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("roi_right_bottom_x")->GetText();
    getVectorFromXML(config, tmp, camera_roi[2]);
    ui->config_textEdit->append("roi_right_bottom_x: "+QString(QLatin1String(tmp)));

    tmp = config->FirstChildElement("roi_right_bottom_y")->GetText();
    getVectorFromXML(config, tmp, camera_roi[3]);
    ui->config_textEdit->append("roi_right_bottom_y: "+QString(QLatin1String(tmp)));

}




void MainWindow::on_actionConfig_triggered()
{
    on_stop_pushButton_clicked();
    Dialog_Config a;
    a.exec();
    on_run_pushButton_clicked();
}
