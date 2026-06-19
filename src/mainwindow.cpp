#include "mainwindow.h"

#include "documenttheme.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QColor>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPalette>
#include <QResizeEvent>
#include <QShowEvent>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QTextBrowser>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

namespace {
constexpr int kMinContentWidth = 480;
constexpr int kDefaultContentWidth = 720;
constexpr int kFramePaddingH = 48;
constexpr int kFramePaddingV = 44;
constexpr int kDocumentMargin = 24;
constexpr int kCanvasPadding = 32;
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadSettings();
    setupUi();
    setupActions();
    applyTheme();
    updateWidthConstraints();
    updateWindowTitle();
}

void MainWindow::setupUi()
{
    setAcceptDrops(true);
    resize(960, 720);

    m_canvas = new QWidget(this);
    m_canvas->setObjectName(QStringLiteral("canvas"));
    m_canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_contentRoot = new QWidget(m_canvas);
    auto *rootLayout = new QVBoxLayout(m_contentRoot);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto *row = new QWidget(m_contentRoot);
    row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(kCanvasPadding, kCanvasPadding, kCanvasPadding, kCanvasPadding);
    rowLayout->setSpacing(0);

    m_documentFrame = new QFrame(row);
    m_documentFrame->setObjectName(QStringLiteral("documentFrame"));
    m_documentFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    auto *frameLayout = new QVBoxLayout(m_documentFrame);
    frameLayout->setContentsMargins(kFramePaddingH, kFramePaddingV, kFramePaddingH, kFramePaddingV);
    frameLayout->setSpacing(0);

    m_browser = new QTextBrowser(m_documentFrame);
    m_browser->setOpenExternalLinks(true);
    m_browser->setFrameShape(QFrame::NoFrame);
    m_browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_browser->setPlaceholderText(tr("Open a Markdown file to preview it."));
    m_browser->document()->setDocumentMargin(kDocumentMargin);
    frameLayout->addWidget(m_browser, 1);

    rowLayout->addStretch();
    rowLayout->addWidget(m_documentFrame);
    rowLayout->addStretch();
    rootLayout->addWidget(row, 1);

    auto *canvasLayout = new QVBoxLayout(m_canvas);
    canvasLayout->setContentsMargins(0, 0, 0, 0);
    canvasLayout->addWidget(m_contentRoot, 1);

    setCentralWidget(m_canvas);

    m_statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_statusLabel, 1);
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setupActions()
{
    auto *fileMenu = menuBar()->addMenu(tr("&File"));

    auto *openAction = fileMenu->addAction(tr("&Open…"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFileDialog);

    auto *reloadAction = fileMenu->addAction(tr("&Reload"));
    reloadAction->setShortcut(QKeySequence::Refresh);
    connect(reloadAction, &QAction::triggered, this, &MainWindow::reloadCurrentFile);

    fileMenu->addSeparator();

    auto *quitAction = fileMenu->addAction(tr("&Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    auto *viewMenu = menuBar()->addMenu(tr("&View"));

    auto *themeGroup = new QActionGroup(this);
    themeGroup->setExclusive(true);

    for (const DocumentTheme &theme : DocumentThemes::all()) {
        auto *themeAction = viewMenu->addAction(theme.name);
        themeAction->setCheckable(true);
        themeAction->setData(theme.id);
        themeGroup->addAction(themeAction);

        if (theme.id == m_currentThemeId) {
            themeAction->setChecked(true);
        }

        connect(themeAction, &QAction::triggered, this, [this, theme]() {
            setTheme(theme.id);
        });
    }

    auto *helpMenu = menuBar()->addMenu(tr("&Help"));

    auto *aboutAction = helpMenu->addAction(tr("&About KMDView"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(
            this,
            tr("About KMDView"),
            tr("<h3>KMDView</h3>"
               "<p>Version %1</p>"
               "<p>A KDE markdown preview application.</p>"
               "<p>Copyright 2026</p>")
                .arg(QApplication::applicationVersion()));
    });

    auto *aboutQtAction = helpMenu->addAction(tr("About &Qt"));
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    auto *toolbar = addToolBar(tr("Main"));
    toolbar->setMovable(false);
    toolbar->addAction(openAction);
    toolbar->addAction(reloadAction);
    toolbar->addSeparator();

    toolbar->addWidget(new QLabel(tr("Width:"), toolbar));

    m_widthSlider = new QSlider(Qt::Horizontal, toolbar);
    m_widthSlider->setMinimum(kMinContentWidth);
    m_widthSlider->setValue(m_contentWidth);
    m_widthSlider->setMinimumWidth(160);
    m_widthSlider->setToolTip(tr("Adjust the maximum width of the document column (up to the window width)"));
    toolbar->addWidget(m_widthSlider);

    m_widthLabel = new QLabel(toolbar);
    m_widthLabel->setMinimumWidth(52);
    toolbar->addWidget(m_widthLabel);

    connect(m_widthSlider, &QSlider::valueChanged, this, &MainWindow::setContentWidth);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    m_contentWidth = settings.value(QStringLiteral("contentWidth"), kDefaultContentWidth).toInt();
    m_fullWidth = settings.value(QStringLiteral("fullWidth"), false).toBool();
    m_currentThemeId = settings.value(QStringLiteral("theme"), QStringLiteral("light")).toString();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue(QStringLiteral("contentWidth"), m_contentWidth);
    settings.setValue(QStringLiteral("fullWidth"), m_fullWidth);
    settings.setValue(QStringLiteral("theme"), m_currentThemeId);
}

int MainWindow::availableMaxWidth() const
{
    if (!m_canvas) {
        return kMinContentWidth;
    }

    const int canvasWidth = m_canvas->width();
    return qMax(kMinContentWidth, canvasWidth - (2 * kCanvasPadding));
}

void MainWindow::updateWidthConstraints()
{
    const int maxWidth = availableMaxWidth();
    const int minWidth = qMin(kMinContentWidth, maxWidth);

    if (m_fullWidth || m_contentWidth > maxWidth) {
        m_contentWidth = maxWidth;
    }

    m_contentWidth = qBound(minWidth, m_contentWidth, maxWidth);
    m_fullWidth = (m_contentWidth >= maxWidth);

    if (m_contentRoot && m_canvas) {
        m_contentRoot->setMinimumHeight(m_canvas->height());
    }

    if (m_widthSlider) {
        m_widthSlider->blockSignals(true);
        m_widthSlider->setMinimum(minWidth);
        m_widthSlider->setMaximum(maxWidth);
        m_widthSlider->setValue(m_contentWidth);
        m_widthSlider->blockSignals(false);
    }

    applyContentWidth();
}

void MainWindow::applyContentWidth()
{
    if (m_documentFrame) {
        m_documentFrame->setFixedWidth(m_contentWidth);
    }

    if (m_widthLabel) {
        if (m_fullWidth) {
            m_widthLabel->setText(tr("100%"));
        } else {
            m_widthLabel->setText(tr("%1 px").arg(m_contentWidth));
        }
    }
}

void MainWindow::setContentWidth(int width)
{
    const int maxWidth = availableMaxWidth();
    const int minWidth = qMin(kMinContentWidth, maxWidth);
    m_contentWidth = qBound(minWidth, width, maxWidth);
    m_fullWidth = (m_contentWidth >= maxWidth);

    if (m_widthSlider && m_widthSlider->value() != m_contentWidth) {
        m_widthSlider->blockSignals(true);
        m_widthSlider->setMinimum(minWidth);
        m_widthSlider->setMaximum(maxWidth);
        m_widthSlider->setValue(m_contentWidth);
        m_widthSlider->blockSignals(false);
    }

    applyContentWidth();
    saveSettings();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateWidthConstraints();
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    updateWidthConstraints();
}

void MainWindow::setTheme(const QString &themeId)
{
    m_currentThemeId = themeId;
    applyTheme();
    saveSettings();
}

void MainWindow::applyTheme()
{
    const DocumentTheme theme = DocumentThemes::byId(m_currentThemeId);

    if (m_documentFrame) {
        m_documentFrame->setStyleSheet(
            QStringLiteral("QFrame#documentFrame { background: %1; border-radius: 10px; }")
                .arg(theme.pageBackground));
    }

    if (m_canvas) {
        m_canvas->setStyleSheet(
            QStringLiteral("QWidget#canvas { background: %1; }").arg(theme.canvasBackground));
    }

    if (m_browser) {
        const QColor textColor(theme.textColor);
        const QColor pageColor(theme.pageBackground);
        const QColor linkColor(theme.linkColor);

        QPalette palette = m_browser->palette();
        palette.setColor(QPalette::Text, textColor);
        palette.setColor(QPalette::WindowText, textColor);
        palette.setColor(QPalette::Base, pageColor);
        palette.setColor(QPalette::Window, pageColor);
        palette.setColor(QPalette::Link, linkColor);
        palette.setColor(QPalette::LinkVisited, linkColor);
        m_browser->setPalette(palette);

        m_browser->setStyleSheet(
            QStringLiteral("QTextBrowser { background: transparent; border: none; color: %1; }")
                .arg(theme.textColor));
        m_browser->document()->setDefaultStyleSheet(theme.styleSheet);

        if (m_browser->source().isValid()) {
            m_browser->reload();
        }
    }
}

void MainWindow::openFile(const QString &path)
{
    const QFileInfo info(path);
    if (!info.exists() || !info.isFile()) {
        showError(tr("File not found: %1").arg(path));
        return;
    }

    if (!info.isReadable()) {
        showError(tr("Cannot read file: %1").arg(path));
        return;
    }

    m_currentPath = info.absoluteFilePath();
    m_browser->setSearchPaths({info.absolutePath()});
    m_browser->setSource(QUrl::fromLocalFile(m_currentPath));
    applyTheme();

    m_statusLabel->setText(m_currentPath);
    statusBar()->showMessage(tr("Loaded %1").arg(info.fileName()), 3000);
    updateWindowTitle();
}

void MainWindow::openFileDialog()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Open Markdown File"),
        m_currentPath.isEmpty() ? QDir::homePath() : QFileInfo(m_currentPath).absolutePath(),
        tr("Markdown Files (*.md *.mkd *.markdown);;All Files (*)"));

    if (path.isEmpty()) {
        return;
    }

    openFile(path);
}

void MainWindow::reloadCurrentFile()
{
    if (m_currentPath.isEmpty()) {
        statusBar()->showMessage(tr("No file open"), 2000);
        return;
    }

    openFile(m_currentPath);
}

void MainWindow::updateWindowTitle()
{
    if (m_currentPath.isEmpty()) {
        setWindowTitle(tr("KMDView"));
        return;
    }

    setWindowTitle(tr("%1 — KMDView").arg(QFileInfo(m_currentPath).fileName()));
}

void MainWindow::showError(const QString &message)
{
    statusBar()->showMessage(message, 5000);
    QMessageBox::warning(this, tr("KMDView"), message);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    const QString path = urls.first().toLocalFile();
    if (path.isEmpty()) {
        return;
    }

    openFile(path);
    event->acceptProposedAction();
}
