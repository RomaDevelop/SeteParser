#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>
#define qdbg qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	//ui->listWidget->addItem("https://www.mail.ru");
	//ui->listWidget->addItem("https://www.avito.ru/moskva/kvartiry/sdam/na_dlitelnyy_srok/1-komnatnye-ASgBAgICA0SSA8gQ8AeQUswIjlk?f=ASgBAgECA0SSA8gQ8AeQUswIjlkCRegHFXsiZnJvbSI6MzMsInRvIjpudWxsfcaaDBV7ImZyb20iOjAsInRvIjo1MDAwMH0&footWalkingMetro=5&s=104");

    manager = new QNetworkAccessManager(this);

	connect(ui->pbParse,&QPushButton::clicked,[this](){
		QFile file("F:/C++/SiteParser/html example.html");
		file.open(QFile::ReadOnly);
		QString html = file.readAll();

		auto blocks = HTML::Parse(html);
		for(auto &block:blocks)
		{
			ui->plainTextEdit->appendPlainText(block.ToStr());
		}
	});

//	QString adressGroup;
//	auto valsList = HTML::GetValues(html,"style-item-address-KooqC");
//	if(valsList.size() != 1) qdbg << "error 1";
//	else adressGroup = valsList[0];

//	QString adress;
//	valsList = HTML::GetValues(adressGroup,"style-item-address__string-wt61A");
//	if(valsList.size() != 1) qdbg << "error 2";
//	else adress = valsList[0];
//	qdbg << adress;
//	qdbg << "--------------------------------";

//	QStringList metroVals;
//	metroVals = HTML::GetValues(adressGroup,"style-item-address-georeferences-item-TZsrp");
//	if(metroVals.size() == 0) qdbg << "error 3";

//	for(auto &metroVal:metroVals)
//	{
//		qdbg << metroVal;
//		qdbg << "--------------------------------";
//	}
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

void MainWindow::on_pushButton_clicked()
{

}
