#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include "csvparserservice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getFilePath() const;
    void setFilePath(const QString &newFilePath);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_textChanged(const QString &fileName);

private:
    Ui::MainWindow *ui;
    QString m_filePath;
    CsvParserService m_processorService;

    // File I/O methods
    QList<QList<QString>> readCsvFile(const QString& filePath);
    void writeProcessedFile(const QString& originalPath,
                            const QList<QList<QString>>& data);
    QString generateOutputFilePath(const QString& originalPath);
};

#endif // MAINWINDOW_H
