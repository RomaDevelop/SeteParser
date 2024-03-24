#include "html.h"

void HTML::ParseTegs()
{
	int size = html.size();
	bool quatsNow = false;
	int tegStart = -1;
	for(int i=0; i<size; i++)
	{
		if(!quatsNow && html[i] == '"') { quatsNow = true; continue; }
		if(quatsNow && html[i] == '"') { quatsNow = false; continue; }

		if(!quatsNow)
		{
			if(tegStart == -1 && html[i] == '<')
			{
				tegStart = i;
				continue;
			}

			if(tegStart != -1 && html[i] == '<')
				LogsSt::Error("HTML::ParseTegs: < nested in teg");

			if(tegStart != -1 && html[i] == '>')
			{
				tegs.push_back(Teg(html, tegStart, i));
				tegs[tegs.size()-1].DecodeTeg();
				tegStart = -1;
			}
		}
	}
}

std::vector<Block> HTML::Parse(const QString &html)
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

void Teg::DecodeTeg()
{
	QString text = GetText();
	text.replace("= ","=");
	text.replace(" =","=");
	QStringList words;
	QString word;
	bool quatsNow = false;
	for(int i=0; i<text.size(); i++)
	{
		if(!quatsNow && html[i] == '"') { quatsNow = true; }
		else if(quatsNow && html[i] == '"') { quatsNow = false; }

		if(!quatsNow)
		{
			if(text[i] == ' ')
			{
				words += word;
				word = "";
				continue;
			}
			else if(text[i] == '=')
			{
				words += word;
				words += "=";
				word = "";
				continue;
			}
		}

		word += text[i];
	}
	words += word;

	//LogsSt::Log("text: " + text + "; words: " + words.join(';'));

	for(int i=0; i<words.size(); i++)
		if(words[i].size() == 0) LogsSt::Error("Teg::ParseTeg: empty word["+QSn(i)+"] in Teg " + IndexesAndSrcTextToStr());

	if(words.size() % 3 == 1)
	{
		if(words[0][0] == '/')
		{
			words[0].remove(0,1);
			type = closer;
		}
		else type = opener;

		name = words[0];

		for(int i=1; i<words.size(); i+=3)
		{
			attributes.push_back(Attribute(words[i],words[i+2]));
		}
	}
	else LogsSt::Error("Teg::ParseTeg: wrong words.size() = " + QSn(words.size()) + " in Teg " + IndexesAndSrcTextToStr());
}
