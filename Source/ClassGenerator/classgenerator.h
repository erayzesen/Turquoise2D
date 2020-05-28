#pragma once

#include "QList"
#include "QVariantMap"
#include "QTextStream"
#include "QDebug"
#include "QFile"
#include "QRegularExpression"
#include "QString"
#include "QDir"
enum class ClassAccessTypes{Public,Private,PublicStatic,PrivateStatic,PublicVirtual,PrivateVirtual};
class ClassGenerator
{
public:
    QString name;
    QString *includeStr=new QString;
    QString *includeCppStr=new QString;
    QString *headerStr=new QString;
    QString *cppStr=new QString;
    QString *headerMacroStr=new QString;
    QString *cppMacroStr=new QString;
    QList<QVariantMap> variableDefinesList;
    QList<QVariantMap> functionDefinesList;
    QString *nameSpacesStr=new QString;
    ClassGenerator(QString className,QStringList inheritanceClasses=QStringList());
    void setHeaderIncludes(QList<QString> classPathList);
    void setCppIncludes(QList<QString> classPathList);
    void addMacroToHeader(QString macroCode);
    void addMacroToCpp(QString macroCode);
    void defineNewVariable(ClassAccessTypes accessType,QString type,QString name,QString value);
    void addNameSpace(QString typeName);
    void defineNewFunction(ClassAccessTypes accessType,QString returnType,QString functionName,QString parametersField,QString functionCode);
    void compileClass(bool oneHeaderFile=false);
    void writeToFile(QString fileName,QString targetFolder,bool oneHeaderFile=false);
    void applyAutoFormat(QString *str);
    QString accessTypeToString(ClassAccessTypes accessType);
};
