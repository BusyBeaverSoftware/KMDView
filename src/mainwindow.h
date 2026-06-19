#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QString>

class QFrame;
class QLabel;
class QSlider;
class QTextBrowser;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void openFile(const QString &path);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void setupUi();
    void setupActions();
    void loadSettings();
    void saveSettings();
    void openFileDialog();
    void reloadCurrentFile();
    void updateWindowTitle();
    void showError(const QString &message);
    int availableMaxWidth() const;
    void updateWidthConstraints();
    void applyContentWidth();
    void setContentWidth(int width);
    void setTheme(const QString &themeId);
    void applyTheme();

    QPointer<QWidget> m_canvas;
    QPointer<QWidget> m_contentRoot;
    QPointer<QFrame> m_documentFrame;
    QPointer<QTextBrowser> m_browser;
    QPointer<QLabel> m_statusLabel;
    QPointer<QSlider> m_widthSlider;
    QPointer<QLabel> m_widthLabel;
    QString m_currentPath;
    QString m_currentThemeId;
    int m_contentWidth = 720;
    bool m_fullWidth = false;
};

#endif
