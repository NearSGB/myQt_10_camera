#ifndef DIALOG_CONFIG_H
#define DIALOG_CONFIG_H

#include <QDialog>
#include <string>
#include <tinyxml2.h>
#include <QLineEdit>
namespace Ui {
class Dialog_Config;
}

class Dialog_Config : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Config(QWidget *parent = 0);
    ~Dialog_Config();
    void insertintoXML(tinyxml2::XMLDocument& , tinyxml2::XMLElement *, const char* , QLineEdit* );

private slots:
    void on_exit_pushButton_clicked();

    void on_ok_pushButton_clicked();

    void on_camera_num_lineEdit_editingFinished();

private:
    void writeConfig(const char *);

    void readConfig(const char *);

    const char* config_path = "config.xml";
    Ui::Dialog_Config *ui;
    int camera_num = 0;

};

#endif // DIALOG_CONFIG_H
