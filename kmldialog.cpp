#include "kmldialog.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QJsonValue"

KMLDialog::KMLDialog()
{

}

void KMLDialog::parseKML(QUrl kmlUrl)
{
    qDebug() <<"url "<<kmlUrl;
    QFile kmlFile(kmlUrl.toLocalFile());
    if( kmlFile.exists())
    {
        if(!kmlFile.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QString error = kmlFile.errorString();
            qDebug() <<"error "<<error;
            return ;
        }
        else
        {
            xmlReader = new QXmlStreamReader(&kmlFile);

            qDebug()<<"reader" <<xmlReader;
            while (!xmlReader->atEnd())
            {
                QXmlStreamReader::TokenType token = xmlReader->readNext();
                if(token== QXmlStreamReader::StartDocument)
                {
                    continue;
                }
                if(xmlReader->isStartElement())
                {
                    QString str = xmlReader->name().toString();
                    if(str =="kml")
                    {
                        //最外层kml
                        qDebug()<<"kml ------";
                    }
                    else if(str == "Document")
                    {
                        //Document" 层
                        qDebug()<<"Document ---------";
                    }
                    else  if(str == "name" )
                    {
                        //FIXME  过滤后打印失败 ？？？
                        //                        if( xmlReader->readElementText().endsWith(".kml"))
                        //                        {
                        //取kml文件名字 过滤掉 placemark 里面的name
                        qDebug()<<"file name "<<xmlReader->readElementText();
                        //                        }
                    }
                    else if(str == "Placemark")
                    {
                        //placeMark层
                        qDebug()<<"placeMark------";
                    }
                    else if(str == "LineString")
                    {
                        //LineString 为航线     Polygon 为航域     Point 为点
                        qDebug()<<"LineString------";
                    }
                    else if ( str == "coordinates")
                    {
                       QString text = xmlReader->readElementText();
                        text = text.replace("\n","");
                        text = text.replace("\t","");
                        //所有的str 分割为经纬高相匹配的list
                        QList<QString> LLAlist;
                        //坑  每组经纬高数据后  根据“  ”来分割 ！！！
                         qDebug()<<"text ====  "<<text;
                          LLAlist.append(text.split(" "));
                          LLAlist.removeLast();
                         for(int i=0;i< LLAlist.count();i++)
                         {
                              QString str = LLAlist[i];
                              QList <QString> list;
                              list.append(str.split(","));

                              //经度的List
                             _longitudeList.append(list[0]);
//                              qDebug()<<" lon "<<i<<"    "<<list[0];
                             _latitudeList.append(list[1]);
                             _altitudeList.append(list[2]);
                         }
                    }
                }
            }
        }
    }
   qDebug()<<" -----------"<<_longitudeList;
}

void KMLDialog::exportKML(QString exportPath,QString fileNameStr,QString docDesciption,QString pmDesciption,QString pointType,QString coordinatesStr)
{
    QFile kmlFile(exportPath); //"/Users/apple/Desktop/myTest.kml"
    if (!kmlFile.open(QFile::WriteOnly )) {

           qDebug() <<"error : "<< kmlFile.errorString();
           return;
       }
    QXmlStreamWriter writer(&kmlFile);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeStartElement("kml");
    writer.writeStartElement("Document");
    writer.writeTextElement("name",fileNameStr+".kml");
    writer.writeTextElement("description",docDesciption);


    //中间style 样式暂时先不写
   writer.writeStartElement("Placemark");
   writer.writeTextElement("name",fileNameStr);
   writer.writeTextElement("description",pmDesciption);

   //styleUrl 忽略
   writer.writeStartElement(pointType);
   writer.writeTextElement("tessellate","1");
   writer.writeTextElement("altitudeMode","relativeToGround");
   writer.writeTextElement("coordinates",coordinatesStr);


    writer.writeEndElement();//pointType  end
    writer.writeEndElement();//placeMark end
    writer.writeEndElement(); // document end
    writer.writeEndElement(); //kml end
    writer.writeEndDocument();//结束document
    kmlFile.close();
}

void KMLDialog::kmlToJson(QString kmlpath,QString fileName ,QString jsonPath)
{
    QFile kmlFile(kmlpath);
    if( kmlFile.exists())
    {
        if(!kmlFile.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QString error = kmlFile.errorString();
            qDebug() <<"error "<<error;
            return ;
        }else
        {

        }
    }
}

void KMLDialog::jsonToKml(QUrl jsonPath,QString fileName,QString kmlPath)
{
        QFile jsonFile(jsonPath.toLocalFile());
        if(jsonFile.exists())
        {
            if(!jsonFile.open(QIODevice::ReadWrite | QIODevice::Text))
            {
                QString error = jsonFile.errorString().toUtf8();
                qDebug() <<"error "<<error;
                return ;
            }
            else
            {
                QString value = jsonFile.readAll();
                QJsonDocument doc =  QJsonDocument::fromJson(value.toUtf8());
                QJsonObject obj = doc.object();
                QJsonValue jsonValue = obj.value(QString("mission"));
                QJsonObject missionItems = jsonValue.toObject();
                QJsonArray itemsArr = missionItems["items"].toArray();

                QString temp;
                QJsonArray tempA ;
                QJsonArray valueArray;
                temp.append("\n\t\t\t\t");
                for(int i= 0;i<itemsArr.count();i++)
                {
                    QJsonObject testO= itemsArr[i].toObject();
                    QJsonValue testV =  testO.value(QString("params"));
                 //   QJsonObject tempObj = testV.toObject();
                    QJsonArray paramsArr =testV.toArray();
                    //NOTE :单独转精度丢失，故用数组处理

                    //调整经纬高顺序
                    tempA.append(paramsArr[5]);
                    tempA.append(paramsArr[4]);
                    tempA.append(paramsArr[6]);
                }
                for(int i=0;i<tempA.count();i++)
                {
                     valueArray.append(tempA[i]);

                    // temp.endsWith(" ");
                }

                for(int i=0;i<valueArray.count();i++)
                {
                     QString str = QString::number(valueArray[i].toDouble(),10,9);
                     if((i+1)%3 ==0)
                     {
                         temp.append(str+" ");
                     }
                     else
                     {
                         temp.append(str+",");
                     }
                }
                 temp.append("\n\t\t\t");
                 qDebug()<<"-----------"<<temp;
                 exportKML("/Users/apple/Desktop/jsonToKml.kml","qtTest","doc 描述","placemark 描述","LineString",temp);
            }
        }
}
