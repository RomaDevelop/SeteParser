#ifndef TAGNAMES_H
#define TAGNAMES_H

#include <QString>

namespace TagNames {

	const QString html {"html"};
	const QString head {"head"};
	const QString body {"body"};
	const QString title {"title"};
	const QString pre {"pre"};
	const QString h1 {"h1"};
	const QString h2 {"h2"};
	const QString h3 {"h3"};
	const QString h4 {"h4"};
	const QString h5 {"h5"};
	const QString h6 {"h6"};
	const QString b {"b"};
	const QString i {"i"};
	const QString tt {"tt"};
	const QString a {"a"};
	const QString blockquote {"blockquote"};
	const QString dl {"dl"};
	const QString ol {"ol"};
	const QString ul {"ul"};
	const QString div {"div"};
	const QString table {"table"};
	const QString tr {"tr"};
	const QString td {"td"};
	const QString th {"th"};
	const QString frameset {"frameset"};
	const QString noframes {"noframes"};
	const QString form {"form"};
	const QString select  {"select "};
	const QString textarea {"textarea "};
	const QString span {"span"};
	const QString path {"path"};
	const QString svg {"svg"};

	const QString p {"p"};
	const QString br {"br"};
	const QString dt {"dt"};
	const QString dd {"dd"};
	const QString li {"li"};
	const QString img {"img "};
	const QString hr {"hr"};
	const QString option {"option"};
	const QString input {"input"};

	const QStringList needClose { html,head,body,title,pre,h1,h2,h3,h4,h5,h6,b,i,tt,
				a,blockquote,dl,ol,ul,div,table,tr,td,th,frameset,noframes,form,
				select,textarea,span,path,svg };
	const QStringList notNeedClose { p,br,dt,dd,li,img,hr,option,input };
	const QStringList all = needClose + notNeedClose;
};

#endif // TAGNAMES_H
