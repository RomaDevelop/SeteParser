#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>

#include "MyQShortings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Attribute
{
	QString name;
	QString value;
};

struct Block
{
	QString blockDefining;
	QString blockText;
	QString blockFinal;

	std::vector<Attribute> attributes;
	std::vector<Block> nestedBlocks;

	QString ToStr() { return "blockDefining [" + blockDefining + "]\nblockText [" + blockText + "]\nblockFinal [" + blockFinal + "]\n"; }
};

//struct ParseResult
//{
//	std::vector<Block> blocks;
//	bool hasNested
//};

struct HTML
{
	static std::vector<Block> Parse(const QString &html)
	{
		std::vector<Block> blocks;
		int curIndex = 0;
		int len = html.length();
		bool blockStarted = false;
		bool blockDefiningNow = false;
		bool blockTextNow = false;
		bool blockFinalNow = false;
		bool quatsNow = false;
		bool blockStarterOrFinisher = false;
		while(curIndex < len)
		{
			if(!blockStarted)
			{
				blockStarted = true;
				blocks.push_back(Block());
			}
			Block& curBlock = blocks[blocks.size()-1];

			if(!quatsNow && html[curIndex] == '"') quatsNow = true;
			if(quatsNow && html[curIndex] == '"') quatsNow = false;

			blockStarterOrFinisher = false;
			if(!quatsNow && !blockDefiningNow && !blockTextNow && html[curIndex] == '<')
			{
				blockDefiningNow = true;
				blockStarterOrFinisher = true;
			}

			if(!quatsNow && blockTextNow && html[curIndex] == '<')
			{
				blockTextNow = false;
				blockFinalNow = true;
				blockStarterOrFinisher = true;
			}

			if(!quatsNow && blockDefiningNow && html[curIndex] == '>')
			{
				blockDefiningNow = false;
				blockTextNow = true;
				blockStarterOrFinisher = true;
			}

			if(!quatsNow && blockFinalNow && html[curIndex] == '>')
			{
				blockFinalNow = false;
				blockStarted = false;
				blockStarterOrFinisher = true;
			}

			if(blockDefiningNow && !blockStarterOrFinisher) curBlock.blockDefining += html[curIndex];
			if(blockTextNow && !blockStarterOrFinisher) curBlock.blockText += html[curIndex];
			if(blockFinalNow && !blockStarterOrFinisher) curBlock.blockFinal += html[curIndex];

			curIndex++;
		}
		return blocks;
	}

	static QStringList GetValues(const QString &html, QString className)
	{
		QStringList ret;
		int size = html.size();
		int index = html.indexOf(className, 0);
		while(index != -1)
		{
			QString value;
			bool valueNow = false;
			bool tegNow = false;
			int countNested = 0;
			for(int i=index; i<size; i++)
			{
				if(valueNow) value+= html[i];

				if(html[i] == '>') valueNow = true;

				if(html[i] == '<')
				{
					if(i+1 < size)
					{
						if(html[i+1] == '/')
						{
							if(countNested) countNested--;
							else
							{
								value.chop(1);
								break;
							}
						}
						else countNested++;
					}
					else qdbg << "wrong html 1";
				}
			}

			ret += value;

			index = html.indexOf(className, index + className.length());
		}
		return ret;
	}
};

struct Metro
{
	QString metro;
	int doMetroPeshkomMinut;
};

struct Ad
{
	QString url;
	QString html;
	QString caption;
	QString adress;
	std::vector<Metro> metros;
};

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
