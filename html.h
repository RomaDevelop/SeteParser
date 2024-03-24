#ifndef HTML_H
#define HTML_H

#include <QString>
#include <QDebug>

#define QSn QString::number
#define qdbg qDebug()

class LogsSt
{
public:
	static void Log(const QString &text) { qDebug() << text; }
	static void Warning(const QString &text) { qDebug() << text; }
	static void Error(const QString &text) { qDebug() << text; }
};

struct Attribute
{
	QString name;
	QString value;

	Attribute(QString name_, QString value_): name{name_}, value{value_} {}

	QString ToStr() { return "["+name+"="+value+"]"; }
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

struct Teg
{
	const QString &html;
	int startIndex;
	int endIndex;

	int type = undefined;
	QString name;
	std::vector<Attribute> attributes;

	Teg(const QString &html_, int startIndex_, int endIndex_): html{html_}, startIndex{startIndex_}, endIndex {endIndex_} {}

	void DecodeTeg();

	QString GetText() const { return html.mid(startIndex+1,endIndex-startIndex-1);}
	QString IndexesAndSrcTextToStr() const
	{
		return "["+QSn(startIndex)+"-"+QSn(endIndex)+"]:["+GetText()+"]";
	}
	QString DecodedToStr()
	{
		QString ret;
		ret = "["+TypeToStr()+"]["+name+"]";
		if(attributes.size()) ret += "\n    Attributes:";
		for(uint i=0; i<attributes.size(); i++)
		{
			ret += attributes[i].ToStr();
		}
		return ret;
	}

	enum type{undefined, opener, closer};
	QString TypeToStr()
	{
		QString ret = "error";
		switch (type) {
		case undefined: ret = "undefined"; break;
		case opener: ret = "opener"; break;
		case closer: ret = "closer"; break;
		default: ret = "error";
		}
		return ret;
	}
};

struct HTML
{
	QString html;
	std::vector<Teg> tegs;
	void ParseTegs();

	QString TegsTextToStr()
	{
		QString ret;
		for(auto &teg:tegs)
		{
			ret += teg.IndexesAndSrcTextToStr() + "\n";
		}
		return ret;
	}

	QString TegsDecodedToStr()
	{
		QString ret;
		for(auto &teg:tegs)
		{
			ret += teg.DecodedToStr() + "\n";
		}
		return ret;
	}

	static std::vector<Block> Parse(const QString &html);
};



#endif // HTML_H
