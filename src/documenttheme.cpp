#include "documenttheme.h"

namespace {

QString baseTypography(const QString &textColor, const QString &headingColor, const QString &mutedColor,
                       const QString &codeBackground, const QString &codeColor, const QString &linkColor,
                       const QString &quoteBorder, const QString &tableBorder, const QString &hrColor)
{
    return QStringLiteral(
               "body {"
               "  color: %1;"
               "  font-family: 'Noto Sans', 'Segoe UI', 'Cantarell', sans-serif;"
               "  font-size: 17px;"
               "  line-height: 1.65;"
               "  margin-top: 0;"
               "  margin-bottom: 0;"
               "}"
               "p, li, span, div, td, th, dd, dt, strong, em { color: %1; }"
               "h1, h2, h3, h4, h5, h6 {"
               "  color: %2;"
               "  font-weight: 600;"
               "  line-height: 1.25;"
               "  margin-top: 1.4em;"
               "  margin-bottom: 0.5em;"
               "}"
               "h1 { font-size: 2.1em; border-bottom: 1px solid %10; padding-bottom: 0.25em; }"
               "h2 { font-size: 1.65em; border-bottom: 1px solid %10; padding-bottom: 0.2em; }"
               "h3 { font-size: 1.35em; }"
               "h4 { font-size: 1.15em; }"
               "p { margin: 0.85em 0; }"
               "ul, ol { margin: 0.85em 0; padding-left: 1.6em; }"
               "li { margin: 0.35em 0; }"
               "a { color: %6; text-decoration: none; }"
               "a:hover { text-decoration: underline; }"
               "code {"
               "  background: %4;"
               "  color: %5;"
               "  font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace;"
               "  font-size: 0.92em;"
               "  padding: 0.12em 0.35em;"
               "  border-radius: 4px;"
               "}"
               "pre {"
               "  background: %4;"
               "  color: %5;"
               "  font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace;"
               "  font-size: 0.9em;"
               "  line-height: 1.5;"
               "  padding: 14px 16px;"
               "  border-radius: 8px;"
               "  white-space: pre-wrap;"
               "  margin: 1em 0;"
               "}"
               "blockquote {"
               "  border-left: 4px solid %7;"
               "  color: %3;"
               "  margin: 1em 0;"
               "  padding: 0.2em 0 0.2em 1em;"
               "}"
               "table { border-collapse: collapse; margin: 1em 0; width: 100%; }"
               "th, td { border: 1px solid %8; padding: 0.45em 0.75em; text-align: left; }"
               "th { font-weight: 600; background: %4; }"
               "hr { border: none; border-top: 1px solid %9; margin: 1.5em 0; }"
               "img { max-width: 100%; height: auto; border-radius: 4px; }")
        .arg(textColor, headingColor, mutedColor, codeBackground, codeColor, linkColor, quoteBorder,
             tableBorder, hrColor, hrColor);
}

const QVector<DocumentTheme> kThemes = {
    {QStringLiteral("light"),
     QStringLiteral("Light"),
     QStringLiteral("#ffffff"),
     QStringLiteral("#eef1f5"),
     QStringLiteral("#1f2933"),
     QStringLiteral("#1565c0"),
     baseTypography(QStringLiteral("#1f2933"), QStringLiteral("#102a43"), QStringLiteral("#486581"),
                    QStringLiteral("#f0f4f8"), QStringLiteral("#243b53"), QStringLiteral("#1565c0"),
                    QStringLiteral("#3daee9"), QStringLiteral("#d9e2ec"), QStringLiteral("#d9e2ec"))},
    {QStringLiteral("dark"),
     QStringLiteral("Dark"),
     QStringLiteral("#1e1e1e"),
     QStringLiteral("#121212"),
     QStringLiteral("#e4e4e4"),
     QStringLiteral("#6cb6ff"),
     baseTypography(QStringLiteral("#e4e4e4"), QStringLiteral("#ffffff"), QStringLiteral("#b0b0b0"),
                    QStringLiteral("#2d2d2d"), QStringLiteral("#d4d4d4"), QStringLiteral("#6cb6ff"),
                    QStringLiteral("#3daee9"), QStringLiteral("#404040"), QStringLiteral("#404040"))},
    {QStringLiteral("sepia"),
     QStringLiteral("Sepia"),
     QStringLiteral("#fbf0d9"),
     QStringLiteral("#ece2cc"),
     QStringLiteral("#3c2f1e"),
     QStringLiteral("#7a4a1a"),
     baseTypography(QStringLiteral("#3c2f1e"), QStringLiteral("#2a2115"), QStringLiteral("#5c4a32"),
                    QStringLiteral("#efe3c8"), QStringLiteral("#3c2f1e"), QStringLiteral("#7a4a1a"),
                    QStringLiteral("#a67c52"), QStringLiteral("#d9cbb0"), QStringLiteral("#d9cbb0"))},
    {QStringLiteral("solarized-light"),
     QStringLiteral("Solarized Light"),
     QStringLiteral("#fdf6e3"),
     QStringLiteral("#eee8d5"),
     QStringLiteral("#073642"),
     QStringLiteral("#268bd2"),
     baseTypography(QStringLiteral("#073642"), QStringLiteral("#002b36"), QStringLiteral("#586e75"),
                    QStringLiteral("#eee8d5"), QStringLiteral("#073642"), QStringLiteral("#268bd2"),
                    QStringLiteral("#2aa198"), QStringLiteral("#ddd6c1"), QStringLiteral("#ddd6c1"))},
    {QStringLiteral("solarized-dark"),
     QStringLiteral("Solarized Dark"),
     QStringLiteral("#002b36"),
     QStringLiteral("#001f27"),
     QStringLiteral("#839496"),
     QStringLiteral("#268bd2"),
     baseTypography(QStringLiteral("#839496"), QStringLiteral("#eee8d3"), QStringLiteral("#93a1a1"),
                    QStringLiteral("#073642"), QStringLiteral("#93a1a1"), QStringLiteral("#268bd2"),
                    QStringLiteral("#2aa198"), QStringLiteral("#094552"), QStringLiteral("#094552"))},
    {QStringLiteral("high-contrast"),
     QStringLiteral("High Contrast"),
     QStringLiteral("#ffffff"),
     QStringLiteral("#000000"),
     QStringLiteral("#000000"),
     QStringLiteral("#0000cc"),
     baseTypography(QStringLiteral("#000000"), QStringLiteral("#000000"), QStringLiteral("#222222"),
                    QStringLiteral("#f0f0f0"), QStringLiteral("#000000"), QStringLiteral("#0000cc"),
                    QStringLiteral("#000000"), QStringLiteral("#000000"), QStringLiteral("#000000"))},
};

} // namespace

const QVector<DocumentTheme> &DocumentThemes::all()
{
    return kThemes;
}

DocumentTheme DocumentThemes::byId(const QString &id)
{
    for (const DocumentTheme &theme : kThemes) {
        if (theme.id == id) {
            return theme;
        }
    }
    return kThemes.first();
}

int DocumentThemes::indexOf(const QString &id)
{
    for (int i = 0; i < kThemes.size(); ++i) {
        if (kThemes.at(i).id == id) {
            return i;
        }
    }
    return 0;
}
