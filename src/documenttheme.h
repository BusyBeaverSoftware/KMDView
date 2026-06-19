#ifndef DOCUMENTTHEME_H
#define DOCUMENTTHEME_H

#include <QString>
#include <QVector>

struct DocumentTheme
{
    QString id;
    QString name;
    QString pageBackground;
    QString canvasBackground;
    QString textColor;
    QString linkColor;
    QString styleSheet;
};

class DocumentThemes
{
public:
    static const QVector<DocumentTheme> &all();
    static DocumentTheme byId(const QString &id);
    static int indexOf(const QString &id);
};

#endif
