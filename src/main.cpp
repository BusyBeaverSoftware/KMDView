#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName(QStringLiteral("kmdview"));
    QApplication::setApplicationDisplayName(QStringLiteral("KMDView"));
    QApplication::setApplicationVersion(QStringLiteral("0.1.0"));
    QApplication::setOrganizationName(QStringLiteral("KDE"));
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setDesktopFileName(QStringLiteral("org.kde.kmdview"));
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.kmdview")));

    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("Preview Markdown documents."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(
        QStringLiteral("files"),
        QObject::tr("Markdown files to open."),
        QStringLiteral("[files…]"));
    parser.process(app);

    MainWindow window;
    window.show();

    const QStringList files = parser.positionalArguments();
    for (const QString &file : files) {
        window.openFile(QDir::cleanPath(file));
        break;
    }

    return app.exec();
}
