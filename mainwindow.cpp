#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>
#include <QtGlobal>

static QString toLowerQt(const QString &str)
{
    return str.toLower();
}

static QString toTitleCaseWordQt(const QString &word)
{
    if (word.isEmpty())
        return word;

    QString result = word.toLower();
    result[0] = result[0].toUpper();
    return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createMenuBar();
    createStatusBar();
    connectSignals();

    resize(700, 500);
    setMinimumSize(500, 400);

    setWindowTitle("Title Case Converter");

    onStatusBarMessage("Ready", 2000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QGroupBox *inputGroup = new QGroupBox("Input", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);

    inputEdit = new QTextEdit(this);
    inputEdit->setPlaceholderText("Enter text here...");
    inputEdit->setFont(QFont("Courier", 12));
    inputEdit->setAcceptRichText(false);
    inputEdit->setMinimumHeight(120);
    inputEdit->setTabChangesFocus(true);
    inputLayout->addWidget(inputEdit);

    mainLayout->addWidget(inputGroup);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    loadButton = new QPushButton("📂 Load", this);
    loadButton->setToolTip("Load text from file");
    buttonLayout->addWidget(loadButton);

    saveButton = new QPushButton("💾 Save", this);
    saveButton->setToolTip("Save converted text to file");
    buttonLayout->addWidget(saveButton);

    buttonLayout->addStretch();

    convertButton = new QPushButton("🔄 Convert", this);
    convertButton->setToolTip("Convert text to title case");
    convertButton->setDefault(true);
    convertButton->setMinimumWidth(120);
    buttonLayout->addWidget(convertButton);

    clearButton = new QPushButton("🗑️ Reset", this);
    clearButton->setToolTip("Reset all fields");
    buttonLayout->addWidget(clearButton);

    buttonLayout->addStretch();

    copyButton = new QPushButton("📋 Copy", this);
    copyButton->setToolTip("Copy converted text to clipboard");
    copyButton->setMinimumWidth(120);
    buttonLayout->addWidget(copyButton);

    mainLayout->addLayout(buttonLayout);

    QGroupBox *outputGroup = new QGroupBox("Output", this);
    QVBoxLayout *outputLayout = new QVBoxLayout(outputGroup);

    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);
    outputEdit->setFont(QFont("Courier", 12));
    outputEdit->setAcceptRichText(false);
    outputEdit->setMinimumHeight(120);
    outputLayout->addWidget(outputEdit);

    mainLayout->addWidget(outputGroup);

    wordCountLabel = new QLabel("Word count: 0", this);
    wordCountLabel->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(wordCountLabel);
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    fileMenu = menuBar->addMenu("&File");

    loadAction = new QAction("&Open...", this);
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setStatusTip("Load text from file");
    fileMenu->addAction(loadAction);

    saveAction = new QAction("&Save...", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip("Save converted text to file");
    fileMenu->addAction(saveAction);

    fileMenu->addSeparator();

    exitAction = new QAction("&Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip("Exit application");
    fileMenu->addAction(exitAction);

    editMenu = menuBar->addMenu("&Edit");

    copyAction = new QAction("&Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip("Copy converted text to clipboard");
    editMenu->addAction(copyAction);

    clearAction = new QAction("&Reset", this);
    clearAction->setShortcut(QKeySequence::Delete);
    clearAction->setStatusTip("Reset all fields");
    editMenu->addAction(clearAction);

    helpMenu = menuBar->addMenu("&Help");

    aboutAction = new QAction("&About", this);
    aboutAction->setStatusTip("About this application");
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals()
{
    connect(convertButton, &QPushButton::clicked, this, &MainWindow::onConvert);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::onCopy);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClear);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadFile);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveFile);

    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onCopy);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClear);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    connect(inputEdit, &QTextEdit::textChanged, this, [this]()
            {
                this->updateWordCount();
                this->onAutoConvert(inputEdit->toPlainText());
            });
}

void MainWindow::updateWordCount()
{
    QString text = inputEdit->toPlainText();
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    wordCountLabel->setText(QString("Word count: %1").arg(words.count()));
}

bool MainWindow::isMinorWord(const QString &word) const
{
    static const QVector<QString> minorWords = {
        "a", "an", "the",
        "and", "but", "or", "for", "nor",
        "on", "at", "to", "by", "in", "for", "of", "with",
        "up", "off", "over", "under", "through", "during",
        "without", "within"};

    QString lower = word.toLower();
    return minorWords.contains(lower);
}

QString MainWindow::toTitleCaseWord(const QString &word) const
{
    if (word.isEmpty())
        return word;

    QString result = word.toLower();
    result[0] = result[0].toUpper();
    return result;
}

QString MainWindow::titleCase(const QString &input) const
{
    if (input.trimmed().isEmpty())
    {
        return QString();
    }

    QStringList words = input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (words.isEmpty())
    {
        return QString();
    }

    words[0] = toTitleCaseWord(words[0]);

    for (int i = 1; i < words.size(); ++i)
    {
        if (!isMinorWord(words[i]))
        {
            words[i] = toTitleCaseWord(words[i]);
        }
        else
        {
            words[i] = words[i].toLower();
        }
    }

    return words.join(" ");
}

void MainWindow::onConvert()
{
    QString input = inputEdit->toPlainText();

    if (input.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Error",
                             "Please enter a text!");
        onStatusBarMessage("Error: No text entered", 3000);
        return;
    }

    QString result = titleCase(input);
    outputEdit->setPlainText(result);

    onStatusBarMessage(QString("✅ Conversion successful (%1 words)")
                           .arg(input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).count()),
                       3000);
}

void MainWindow::onCopy()
{
    QString text = outputEdit->toPlainText();

    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Error",
                             "No text to copy!");
        onStatusBarMessage("Error: No text to copy", 3000);
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);

    onStatusBarMessage("✅ Text copied", 3000);
}

void MainWindow::onClear()
{
    inputEdit->clear();
    outputEdit->clear();
    updateWordCount();
    onStatusBarMessage("🧹 All reset", 2000);
}

void MainWindow::onLoadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open text file",
                                                    QDir::homePath(),
                                                    "Text file (*.txt);;All files (*.*)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error",
                              QString("Cannot open file:\n%1").arg(file.errorString()));
        onStatusBarMessage("Error on opening file", 3000);
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    inputEdit->setPlainText(content);
    updateWordCount();
    onStatusBarMessage(QString("📂 File opened: %1").arg(fileName), 3000);
}

void MainWindow::onSaveFile()
{
    QString text = outputEdit->toPlainText();
    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Error",
                             "No text to save!");
        onStatusBarMessage("Error: No text to save", 3000);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save output",
                                                    QDir::homePath() + "/title_case_output.txt",
                                                    "Text files (*.txt);;All files (*.*)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error",
                              QString("Cannot save file:\n%1").arg(file.errorString()));
        onStatusBarMessage("Error on file saving", 3000);
        return;
    }

    QTextStream out(&file);
    out << text;
    file.close();

    onStatusBarMessage(QString("💾 Saved: %1").arg(fileName), 3000);
}

void MainWindow::onExit()
{
    qApp->quit();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About Title Case Converter",
                       "<h2>Title Case Converter</h2>"
                       "<p>Version 1.0.0</p>"
                       "<p>A tool to convert text to <b>Title Case</b> "
                       "according to English rules.</p>"
                       "<p>Developed with Qt6 und C++17</p>"
                       "<p>Developed by Oliver Berning</p>"
                       "<p>License: GPL v3</p>");
}

void MainWindow::onStatusBarMessage(const QString &message, int timeout)
{
    statusBar()->showMessage(message, timeout);
}

void MainWindow::onAutoConvert(const QString &text)
{
    if (!text.trimmed().isEmpty())
    {
        QString result = titleCase(text);
        outputEdit->setPlainText(result);
    }
}
