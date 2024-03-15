#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#define qdbg qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString url = item->text();
    response = manager->get(QNetworkRequest(QUrl(url)));
    connect(response, &QNetworkReply::finished, [this, url](){
        QString html = response->readAll();
        pages.push_back(new Page(url, html,ui->verticalLayout,ui->plainTextEdit));
    });
}

Page::Page(QString url_, QString html_, QVBoxLayout *layout, QPlainTextEdit *textEdit_):
    url {url_},
    html {html_},
    textEdit {textEdit_}
{
    showURL = new QPushButton("showURL");
    showHtml = new QPushButton("showHtml");
    saveHtml = new QPushButton("saveHtml");
    showAds = new QPushButton("showAds");
    QHBoxLayout *hlo = new QHBoxLayout;
    hlo->addWidget(showURL);
    hlo->addWidget(showHtml);
    hlo->addWidget(saveHtml);
    hlo->addWidget(showAds);
    layout->addLayout(hlo);

    QObject::connect(showURL, &QPushButton::clicked, [this](){
        textEdit->setPlainText(url);
    });

    QObject::connect(showHtml, &QPushButton::clicked, [this](){
        textEdit->setPlainText(html);
    });

    QObject::connect(saveHtml, &QPushButton::clicked, [this](){
        QFile file("d:/html.txt");
        file.open(QFile::WriteOnly);
        file.write(html.toUtf8());
    });

    QObject::connect(showAds, &QPushButton::clicked, [this](){
        textEdit->setPlainText(ads.join('\n'));
    });

    int index = html_.indexOf("iva-item-sliderLink-uLz1v");
    while(index != -1)
    {
        html_.remove(0,index+QString("iva-item-sliderLink-uLz1v").length());
        index = html_.indexOf("iva-item-sliderLink-uLz1v");

        QString ad = html_.left(html_.indexOf("\" rel=\"noopener\""));
        ad.remove(0,ad.indexOf("href=\""));
        ads += ad;
    }
}
