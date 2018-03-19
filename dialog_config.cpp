#include "dialog_config.h"
#include "ui_dialog_config.h"
#include <string>
#include <tinyxml2.h>

Dialog_Config::Dialog_Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Config)
{
    ui->setupUi(this);
    readConfig(config_path);
}

Dialog_Config::~Dialog_Config()
{
    delete ui;

}


void Dialog_Config::insertintoXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *config, const char* name, QLineEdit* editname)
{
    tinyxml2::XMLElement *node = doc.NewElement(name);
    node->InsertEndChild(doc.NewText(editname->text().toStdString().c_str()));
    config->InsertEndChild(node);
}

void Dialog_Config::writeConfig(const char* config_path) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *config = doc.NewElement("config");
    doc.InsertEndChild(config);
    insertintoXML(doc, config, "camera_num", ui->camera_num_lineEdit);
    insertintoXML(doc, config, "camera_id_list", ui->camera_id_list_lineEdit);
    insertintoXML(doc, config, "camera_w", ui->camera_w_lineEdit);
    insertintoXML(doc, config, "camera_h", ui->camera_h_lineEdit);
    insertintoXML(doc, config, "show_time_interval", ui->show_time_interval_lineEdit);
    insertintoXML(doc, config, "roi_left_top_x", ui->roi_left_top_x_lineEdit);
    insertintoXML(doc, config, "roi_left_top_y", ui->roi_left_top_y_lineEdit);
    insertintoXML(doc, config, "roi_right_bottom_x", ui->roi_right_bottom_x_lineEdit);
    insertintoXML(doc, config, "roi_right_bottom_y", ui->roi_right_bottom_y_lineEdit);
    doc.SaveFile(config_path);
}

void Dialog_Config::readConfig(const char* config_path) {
    tinyxml2::XMLDocument doc;
    if(doc.LoadFile(config_path) != 0)
        return;
    tinyxml2::XMLElement *config = doc.RootElement();
    ui->camera_num_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("camera_num")->GetText())));
    ui->camera_id_list_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("camera_id_list")->GetText())));
    ui->camera_w_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("camera_w")->GetText())));
    ui->camera_h_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("camera_h")->GetText())));
    ui->show_time_interval_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("show_time_interval")->GetText())));
    ui->roi_left_top_x_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("roi_left_top_x")->GetText())));
    ui->roi_left_top_y_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("roi_left_top_y")->GetText())));
    ui->roi_right_bottom_x_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("roi_right_bottom_x")->GetText())));
    ui->roi_right_bottom_y_lineEdit->setText(QString(QLatin1String(config->FirstChildElement("roi_right_bottom_y")->GetText())));
}

void Dialog_Config::on_exit_pushButton_clicked()
{
    this->close();
}

void Dialog_Config::on_ok_pushButton_clicked()
{
    writeConfig(config_path);
    this->close();
}

void Dialog_Config::on_camera_num_lineEdit_editingFinished()
{

    int tmp = ui->camera_num_lineEdit->text().toInt();
    if(tmp < 0 || tmp > 15 || camera_num == tmp)
        return;
    camera_num = tmp;
    QString show;
    for(int i = 0; i < camera_num; i++)
        show += QString::number(i)+",";
    show.remove(show.size()-1, 1);
    ui->camera_id_list_lineEdit->setText(show);
    show.clear();
    for(int i = 0; i < camera_num; i++)
        show += QString::number(0)+",";
    show.remove(show.size()-1,1);
    ui->roi_left_top_x_lineEdit->setText(show);
    ui->roi_left_top_y_lineEdit->setText(show);
    show.clear();
    for(int i = 0; i < camera_num; i++)
        show += QString::number(100)+",";
    show.remove(show.size()-1,1);
    ui->roi_right_bottom_x_lineEdit->setText(show);
    ui->roi_right_bottom_y_lineEdit->setText(show);

}
