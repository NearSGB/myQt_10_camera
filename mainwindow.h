#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "cqtopencvviewergl.h"
#include <QTimer>
#include <tinyxml2.h>
#include <dialog_config.h>
#include <omp.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getVectorFromXML(tinyxml2::XMLElement *config, const char *element, std::vector<int> &vec);

    void show_num_camera(int id, int num, CQtOpenCVViewerGl* cvViewer);
    
private slots:
    void on_exit_pushButton_clicked();

    void on_stop_pushButton_clicked();

    void on_run_pushButton_clicked();

    void onShowTimeout();

    void on_actionConfig_triggered();

private:
    void release_cap();
    void readConfig(const char *);

    int camera_num = 0;
    bool isRun = false;
    Ui::MainWindow *ui;
    std::vector<cv::VideoCapture> cap; //用来读取视频结构
    std::vector<int> camera_id_list;
    std::vector<cv::Mat> frames;
    std::vector<std::vector<int>> camera_roi;
    std::vector<int> camera_roi_all;
    int show_time_interval = 50;
    QTimer show_timer;
    const char* config_path = "config.xml";
    int camera_w = 960, camera_h = 540;
    cv::Mat combine1, combine2;
};

#endif // MAINWINDOW_H
