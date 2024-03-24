#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QDebug>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>

#include "html.h"
//#include "MyQShortings.h"

#define qdbg qDebug()

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Page
{
    QString url;
    QString html;
    QStringList ads;
    QPushButton *showURL;
    QPushButton *showHtml;
    QPushButton *saveHtml;
    QPushButton *showAds;
    QPlainTextEdit *textEdit;

    Page(QString url_, QString html_, QVBoxLayout *layout, QPlainTextEdit *textEdit_);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;

    QNetworkAccessManager* manager;
    QNetworkReply *response;
    std::vector<Page*> pages;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
	void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
