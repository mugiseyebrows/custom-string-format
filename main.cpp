
#include <QCoreApplication>
#include <QDebug>
#include <QRandomGenerator>

double floorWithPrec(double value, int prec) {
    return floor(abs(value) * pow(10.0, prec)) / pow(10.0, prec) * (value < 0 ? -1.0 : 1.0);
}

int countIntDigits(double value, int prec) {
    double t = round(abs(value) * pow(10.0, prec)) / pow(10.0, prec);
    return t >= 1.0 ? log10(t) + 1 : 0;
}

QString toString(const QString& format, double value, bool truncate = false) {
    int fieldWidth0 = format.size();
    int intDigits0 = format.indexOf(".");
    int prec = fieldWidth0 - intDigits0 - 1;
    if (truncate) {
        value = floorWithPrec(value, prec);
    }
    int intDigits = countIntDigits(value, prec);
    int fieldWidth = qMax(intDigits0, intDigits) + 1 + prec + (value < 0 ? 1 : 0);
    QString formated = QString("%1").arg(value, fieldWidth, 'f', prec, QChar('0'));
    int trim = 0;
    while (format.size() - trim - 1 > -1
           && formated.size() - trim - 1 > -1
           && format[format.size() - trim - 1] == "#"
           && formated[formated.size() - trim - 1] == "0")  {
        trim++;
    }
    return formated.mid(0, formated.size() - trim);
}


void test2() {
    QRandomGenerator* gen = QRandomGenerator::global();
    QString format = "00.00##";
    double v = -0.01;
    while (abs(v) < 1000) {
        v = v * (1.0 + gen->bounded(0.1));
        qDebug() << QString("%1 rounded %2 truncated %3")
                    .arg(v,0,'f',6)
                    .arg(toString(format, v, false))
                    .arg(toString(format, v, true)).toStdString().c_str();
    }
}

void test1() {
    QString format = "00.00##";

    Q_ASSERT(toString(format, 1.23) == "01.23");
    Q_ASSERT(toString(format, 10) == "10.00");

    Q_ASSERT(toString(format, 0.535353, true) == "00.5353");
    Q_ASSERT(toString(format, 0.535353, false) == "00.5354");

    Q_ASSERT(toString(format, 831.5) == "831.50");
    Q_ASSERT(toString(format, 1.2) == "01.20");

    Q_ASSERT(toString(format, 99.99999, false) == "100.00");
    Q_ASSERT(toString(format, 99.99999, true) == "99.9999");

    Q_ASSERT(toString(format, -1.23) == "-01.23");
    Q_ASSERT(toString(format, -99.99999, false) == "-100.00");
    Q_ASSERT(toString(format, -99.99999, true) == "-99.9999");

    Q_ASSERT(toString(format, 1e-7, true) == "00.00");
    Q_ASSERT(toString(format, 1e-7, false) == "00.00");

    Q_ASSERT(toString(format, -1e-7, true) == "00.00");
    Q_ASSERT(toString(format, -1e-7, false) == "-00.00");

    Q_ASSERT(toString(format, 0.0, true) == "00.00");
    Q_ASSERT(toString(format, 0.0, false) == "00.00");

    qDebug() << "test1 passed";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    test1();

    return 0;
}
