#include "csvparserservice.h"
#include <QRegularExpression>
#include <sstream>
#include <iomanip>

CsvParserService::CsvParserService()
{
}

QList<QString> CsvParserService::processRow(const QList<QString>& row)
{
    if (row.size() <= Z_INDEX_COLUMN) {
        return row; // Invalid row, return as-is
    }

    // Index    Y (easting) X (northing)    Z (elevation)   Point description
    // 1        524192.49   1156428.45      249.160         cep
    QList<QString> processedRow = row;

    // Get z index as double
    double zIndexNumericValue = row.at(Z_INDEX_COLUMN).toDouble();
    // ensure that Z index has N decimal places. (249.16 => 249.160)
    processedRow.replace(Z_INDEX_COLUMN, QString::number(zIndexNumericValue, 'f', DECIMAL_PLACES_OUTPUT));

    int zAdjustmentIndex = containsZAdjustmentValue(row);
    // Z index adjustment is not present =>
    if (zAdjustmentIndex == -1) {
        return processedRow;
    }

    // Process Z adjustment value
    double zAdjustmentNumericValue = parseDigitsAsDouble(row.at(zAdjustmentIndex));

    // the adjustment value should always be bigger than zero. If not, it is not valid
    if (zAdjustmentNumericValue <= 0) {
        return processedRow;
    }

    // normalize the unit of adjustment value
    zAdjustmentNumericValue = roundTo(zAdjustmentNumericValue / Z_DIVISOR, DECIMAL_PLACES_OUTPUT);

    double adjustedZIndex = roundTo(zIndexNumericValue - zAdjustmentNumericValue, DECIMAL_PLACES_ADJUSTED);

    processedRow.replace(Z_INDEX_COLUMN, QString::number(adjustedZIndex, 'f', DECIMAL_PLACES_OUTPUT));
    processedRow.replace(zAdjustmentIndex, "z " + QString::number(zAdjustmentNumericValue));

    return processedRow;
}

QList<QList<QString>> CsvParserService::processData(const QList<QList<QString>>& data)
{
    QList<QList<QString>> processedData;
    processedData.reserve(data.size());

    for (const auto& row : data) {
        processedData.append(processRow(row));
    }

    return processedData;
}

// Z index can be adjusted by point description starting with z and digit (e.g. z10)
int CsvParserService::containsZAdjustmentValue(const QList<QString>& stringList)
{
    QRegularExpression pattern("[zZ]\\d");

    for (int i = 0; i < stringList.size(); ++i) {
        if (pattern.match(stringList[i]).hasMatch()) {
            return i;
        }
    }

    return -1;
}

double CsvParserService::roundTo(double value, int decimalPlaces)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimalPlaces) << value;
    double roundedValue;
    stream >> roundedValue;
    return roundedValue;
}

double CsvParserService::parseDigitsAsDouble(const QString& input)
{
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
