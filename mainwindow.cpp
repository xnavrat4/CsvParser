#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QRegularExpression>
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
        // File was selected
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
    QFile file(m_filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->label->setText("Error opening file:" + file.errorString());
        return;
    }

    QList<QList<QString>> data;

    // Read and parse the CSV file
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(' ');

        auto newRow = QList<QString>();
        // Process the fields as needed
        for (const QString& field : fields) {
            if (!field.isEmpty()){
                newRow.append(field);
            }
        }
        data.append(newRow);
    }

    file.close();

    QList<QList<QString>> newData;

    for(const auto &row : data){
        auto newRow = row;
        auto index = containsZ(row);
        if (index == -1){
            newRow.replace(3, QString::number(row.at(3).toDouble(),'f',3));
            newData.append(newRow);
            continue;
        }

        auto zVal = row.at(index);
        auto numericValue = parseDigitsAsDouble(zVal);
        if (numericValue <= 0){
            newRow.replace(3, QString::number(row.at(3).toDouble(),'f',3));
            newData.append(newRow);
            continue;
        }
        numericValue /= 100;
        numericValue = round_up(numericValue, 3);
        auto originalValue = row.at(3).toDouble();
        auto newValue = originalValue - numericValue ;
        newValue = round_up(newValue, 2);
        newRow.replace(3, QString::number(newValue,'f',3));
        newRow.replace(index, "z " + QString::number(numericValue));
        newData.append(newRow);
    }

    auto filename = m_filePath;
    auto newFileName = filename.replace(".txt", "-parsed.txt");
    QFile newFile(newFileName);

    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->label->setText("Error opening new file:" + newFile.errorString());
        return;
    }

    QTextStream out(&newFile);

    // Write each string in the list to the file
    for (const auto& innerList : newData) {
        QString line;
        for (int i = 0; i < innerList.size(); ++i) {
            if (i < 4){
                line.append(innerList.at(i) + QString(' '));
                continue;
            }
            line.append(innerList.at(i) + QString(i == innerList.size() - 1  ? "\n" : " "));
        }
        out << line; // Write each string followed by a newline
    }
    newFile.close();

}

int MainWindow::containsZ(const QList<QString>& stringList) {
    for (int i = 0; i < stringList.size(); ++i) {
        QRegularExpression pattern("[zZ]\\d");
        if (pattern.match(stringList[i]).hasMatch()) {
            // "z" found in the
            return i;
        }
    }
    // No string with "z" found
    return -1;
}

double MainWindow::round_up(double value, int decimal_places) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimal_places) << value;
    double roundedValue;
    stream >> roundedValue;
    return roundedValue;
}

double MainWindow::parseDigitsAsDouble(const QString &input) {
    QRegularExpression pattern("\\d+");
    QRegularExpressionMatchIterator matches = pattern.globalMatch(input);

    double result = 0.0;

    while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
        QString digitString = match.captured();
        result += digitString.toDouble();
    }

    return result;
}
