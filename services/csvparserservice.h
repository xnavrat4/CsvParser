#ifndef CSVPARSERSERVICE_H
#define CSVPARSERSERVICE_H

#include <QString>
#include <QList>

class CsvParserService
{
public:
    CsvParserService();

    // Main processing methods
    QList<QList<QString>> processData(const QList<QList<QString>>& data);
    QList<QString> processRow(const QList<QString>& row);

    // Utility methods
    int containsZAdjustmentValue(const QList<QString>& stringList);
    double parseDigitsAsDouble(const QString& input);
    double roundTo(double value, int decimalPlaces);

private:
    static constexpr int Z_INDEX_COLUMN = 3;
    static constexpr int DECIMAL_PLACES_OUTPUT = 3;
    static constexpr int DECIMAL_PLACES_ADJUSTED = 2;
    static constexpr double Z_DIVISOR = 100.0;
};

#endif // CSVPARSERSERVICE_H
