#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Text File"),
        defaultPath,
        tr("Text Files (*.txt);;All Files (*)")
        );

    if (!fileName.isEmpty()) {
        m_filePath = fileName;
        ui->lineEdit->setText(m_filePath);
    } else {
        m_filePath = QString();
        ui->lineEdit->setText(QString());
    }
}

QString MainWindow::getFilePath() const
{
    return m_filePath;
}

void MainWindow::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
}

void MainWindow::on_pushButton_2_clicked()
{
    QList<QList<QString>> data = readCsvFile(m_filePath);
    if (data.isEmpty()) {
        return; // Error already displayed in readCsvFile
    }

    QList<QList<QString>> processedData = m_processorService.processData(data);
    writeProcessedFile(m_filePath, processedData);
}

QList<QList<QString>> MainWindow::readCsvFile(const QString& filePath)
{
    QList<QList<QString>> data;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->label->setText("Error opening file: " + file.errorString());
        return data;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(' ', Qt::SkipEmptyParts);
        data.append(fields.toVector().toList());
    }

    file.close();
    return data;
}

QString MainWindow::generateOutputFilePath(const QString& originalPath)
{
    QFileInfo fileInfo(originalPath);
    QString baseName = fileInfo.completeBaseName();
    QString extension = fileInfo.completeSuffix();
    QString dirPath = fileInfo.absolutePath();

    return dirPath + "/" + baseName + "-parsed." + extension;
}

void MainWindow::writeProcessedFile(const QString& originalPath,
                                    const QList<QList<QString>>& data)
{
    QString outputPath = generateOutputFilePath(originalPath);
    QFile file(outputPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->label->setText("Error opening new file: " + file.errorString());
        return;
    }

    QTextStream out(&file);

    for (const auto& row : data) {
        QStringList rowData;
        for (int i = 0; i < row.size(); ++i) {
            rowData.append(row.at(i));
        }
        out << rowData.join(' ') << '\n';
    }

    file.close();
    ui->label->setText("File processed successfully: " + outputPath);
}

void MainWindow::on_lineEdit_textChanged(const QString &fileName)
{
    m_filePath = fileName;
    ui->lineEdit->setText(m_filePath);
}
