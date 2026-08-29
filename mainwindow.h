#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QApplication>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConvert();
    void onCopy();
    void onClear();
    void onLoadFile();
    void onSaveFile();
    void onExit();
    void onAbout();
    void onStatusBarMessage(const QString &message, int timeout = 3000);
    void onAutoConvert(const QString &text);

private:
    QTextEdit *inputEdit;
    QTextEdit *outputEdit;
    QPushButton *convertButton;
    QPushButton *copyButton;
    QPushButton *clearButton;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QLabel *wordCountLabel;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *loadAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *clearAction;
    QAction *aboutAction;

    bool isMinorWord(const QString &word) const;
    QString toTitleCaseWord(const QString &word) const;
    QString titleCase(const QString &input) const;

    void setupUI();
    void createMenuBar();
    void createStatusBar();
    void connectSignals();
    void updateWordCount();
    QString getCurrentTheme() const;
};

#endif // MAINWINDOW_H
