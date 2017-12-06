#ifndef KMLDIALOG_H
#define KMLDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QWidget>
#include <qdebug.h>
#include <QXmlStreamReader>
#include <QFileDialog>




class KMLDialog:public QObject
{
    Q_OBJECT
public:
    KMLDialog();
    QXmlStreamReader *xmlReader;
    QString getValue(const QString &name) ;
   Q_INVOKABLE void parseKML(QUrl kmlUrl);
   Q_INVOKABLE void exportKML(QString exportPath,QString fileNameStr,QString docDesciption,QString pmDesciption,QString pointType,QString coordinatesStr);
   Q_INVOKABLE void kmlToJson(QString kmlpath,QString fileName ,QString jsonPath);
    Q_INVOKABLE void jsonToKml(QUrl jsonPath,QString fileName,QString kmlPath);

   Q_PROPERTY( QStringList longitudeList READ longitudeList)
    Q_PROPERTY(QStringList latitudeList READ latitudeList)
    Q_PROPERTY(QStringList altitudeList READ altitudeList)

private:
   QStringList _longitudeList;
   QStringList longitudeList(void) { return _longitudeList; }

   QStringList _latitudeList;
   QStringList latitudeList(void) { return _latitudeList; }

   QStringList _altitudeList;
   QStringList altitudeList(void) { return _altitudeList; }
};

#endif // KMLDIALOG_H
