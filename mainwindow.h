#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString m_filePath;
    QMap<int, int> m_spaceCount = {{1, 14}, {2, 3}, {3, 3}, {4,2}};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getFilePath() const;
    void setFilePath(const QString &newFilePath);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    int containsZ(const QList<QString> &stringList);
    double round_up(double value, int decimal_places);
};
#endif // MAINWINDOW_H
