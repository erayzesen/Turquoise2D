#include "classgenerator.h"

ClassGenerator::ClassGenerator(QString className,QStringList inheritanceClasses)
{
    name=className;
    QTextStream hStream(headerStr);
    //Header macro definations start
    hStream<<"#ifndef "+className+"_h\n";
    hStream<<"#define "+className+"_h\n\n";

    hStream<<"%headerMacros%\n\n";


    hStream<<"%includes%\n";
    hStream<<"%namespaces%\n";
    if(inheritanceClasses.count()==0){
        hStream<<"class "<<className<<"\n{\n";
    }else{
        QTextStream inheritStr(new QString());
        for(int i=0;i<inheritanceClasses.count();i++){
            inheritStr<<"public "<<inheritanceClasses.at(i);
            if(i!=inheritanceClasses.count()-1){
                inheritStr<<",";
            }
        }
        hStream<<"class "<<className<<" : "<< inheritStr.readAll()<<"\n{\n";

    }
    hStream<<"public:\n%publicDefines%\n";
    hStream<<"private:\n%privateDefines%\n";
    hStream<<"};\n";

    //Header macro definations end
    hStream<<"\n\n";
    hStream<<"#endif //End of "<< className<<".h";

    //qDebug()<<hStream.readAll();
    QTextStream cppStream(cppStr);

    //include header file to cpp file
    cppStream<<"%includes%\n";
    cppStream<<"#include \""<<className<<".h\"\n\n";
    cppStream<<"%cppMacros%\n\n";
}

void ClassGenerator::setHeaderIncludes(QList<QString> classPathList)
{
    QTextStream incStream(includeStr);
    for(int i=0;i<classPathList.count();i++){
        incStream<<"#include \""<<classPathList.at(i)<<"\"\n";
    }
}

void ClassGenerator::setCppIncludes(QList<QString> classPathList)
{
    QTextStream incStream(includeCppStr);
    for(int i=0;i<classPathList.count();i++){
        incStream<<"#include \""<<classPathList.at(i)<<"\"\n";
    }
}

void ClassGenerator::addMacroToHeader(QString macroCode)
{
    QTextStream macro(headerMacroStr);
    macro<<macroCode<<"\n";
}

void ClassGenerator::addMacroToCpp(QString macroCode)
{
    QTextStream macro(cppMacroStr);
    macro<<macroCode<<"\n";
}

void ClassGenerator::defineNewVariable(ClassAccessTypes accessType,QString type, QString name, QString value)
{
    QVariantMap vm;
    vm.insert("feature",accessTypeToString(accessType));
    vm.insert("type",type);
    vm.insert("name",name);
    vm.insert("value",value);
    variableDefinesList.append(vm);
}

void ClassGenerator::addNameSpace(QString typeName)
{
    QTextStream nsStream(nameSpacesStr);
    nsStream<<"using namespace "<<typeName<<";\n";
}

void ClassGenerator::defineNewFunction(ClassAccessTypes accessType,QString returnType, QString functionName,QString parametersField, QString functionCode)
{

    QVariantMap vm;
    vm.insert("feature",accessTypeToString(accessType));
    vm.insert("returnType",returnType);
    vm.insert("functionName",functionName);
    vm.insert("parametersField",parametersField);
    vm.insert("functionCode",functionCode);
    functionDefinesList.append(vm);
}


void ClassGenerator::compileClass(bool oneHeaderFile)
{
    headerStr->replace("%includes%",*includeStr);
    headerStr->replace("%namespaces%",*nameSpacesStr);
    headerStr->replace("%headerMacros%",*headerMacroStr);
    cppStr->replace("%includes%",*includeCppStr);
    cppStr->replace("%cppMacros%",*cppMacroStr);
    QTextStream publicDefines(new QString());
    QTextStream privateDefines(new QString());
    QTextStream cppDefines(new QString());
    //Define variables
    for(int i=0;i<variableDefinesList.count();i++){
        QVariantMap var=variableDefinesList.at(i);
        QTextStream valueDefine(new QString());
        if(var.value("feature")=="public static" || var.value("feature")=="private static"){
            valueDefine<<"static ";
        }else if(var.value("feature")=="public virtual" || var.value("feature")=="private virtual"){
            valueDefine<<"virtual ";
        }
        if(var.value("type").toString()!=""){
            valueDefine<<var.value("type").toString()<<" ";
        }
        valueDefine<<var.value("name").toString();
        if(var.value("value").toString()!=""){
            valueDefine<<"="<<var.value("value").toString();
        }
        valueDefine<<";\n";
        QString defStr=valueDefine.readAll();
        QString feature=var.value("feature").toString();
        if(feature=="public" || feature=="public static"){
            publicDefines<<defStr;
        }else if(feature=="private" || feature=="private static"){
            privateDefines<<defStr;
        }
    }
    //Define Functions
    for(int i=0;i<functionDefinesList.count();i++){
        QVariantMap func=functionDefinesList.at(i);
        QTextStream funcDefine(new QString());
        QTextStream funcDefineCpp(new QString());
        if(func.value("feature")=="public static" || func.value("feature")=="private static"){
            funcDefine<<"static ";
        }else if(func.value("feature")=="public virtual" || func.value("feature")=="private virtual"){
            funcDefine<<"virtual ";
        }
        if(func.value("returnType").toString()!=""){
            funcDefine<<func.value("returnType").toString()<<" ";
        }
        funcDefine<<func.value("functionName").toString()<<"("<<func.value("parametersField").toString()<<")";
        if(oneHeaderFile==true){
            funcDefine<<"{\n"<<func.value("functionCode").toString()<<"\n}\n";
        }else{
            funcDefine<<";\n";
            if(func.value("returnType").toString()!=""){
                funcDefineCpp<<func.value("returnType").toString()<<" ";
            }
            funcDefineCpp<<name<<"::"<<func.value("functionName").toString()<<"("<<func.value("parametersField").toString()<<")";
            funcDefineCpp<<"\n{\n"<<func.value("functionCode").toString()<<"\n}\n";
        }
        QString defStr=funcDefine.readAll();
        QString feature=func.value("feature").toString();
        if(feature=="public" || feature=="public static" || feature=="public virtual"){
            publicDefines<<defStr;
        }else if(feature=="private" || feature=="private static" || feature=="private virtual"){
            privateDefines<<defStr;
        }
        cppDefines<<funcDefineCpp.readAll();
    }
    headerStr->replace("%publicDefines%",publicDefines.readAll());
    headerStr->replace("%privateDefines%",privateDefines.readAll());
    *cppStr+=cppDefines.readAll();
    applyAutoFormat(headerStr);
    applyAutoFormat(cppStr);

}

void ClassGenerator::writeToFile(QString fileName,QString targetFolder, bool oneHeaderFile)
{
    compileClass(oneHeaderFile);
    if(!QDir(targetFolder).exists()){
        QDir().mkpath(targetFolder);
    }

    QString headerPath=targetFolder+"/"+fileName+".h";
    qDebug()<<"header path: "<<headerPath;
    QFile hFile(headerPath);
    if(hFile.open(QIODevice::WriteOnly)){
        hFile.write(QByteArray(headerStr->toStdString().c_str()));
        hFile.close();
    }

    if(!oneHeaderFile){
        QString cppPath=targetFolder+"/"+fileName+".cpp";
        qDebug()<<"cpp path: "<<cppPath;
        QFile cppFile(cppPath);
        if(cppFile.open(QIODevice::WriteOnly)){
            cppFile.write(QByteArray(cppStr->toStdString().c_str()));
            cppFile.close();
        }
    }

}

void ClassGenerator::applyAutoFormat(QString *str)
{
    QTextStream st(str);
    QTextStream tempSt(new QString());
    int tabCount=0;
    //Setting tab space
    int tabSize=4;
    QString spaceTabStr;
    for(int i=0;i<tabSize;i++)
        spaceTabStr+=" ";
    //Checking lines
    while(!st.atEnd()){
        QString line=st.readLine();
        if(line=="public:" || line=="private:" || line=="static public:" || line=="static private:"){
            tempSt<<line<<"\n";
            continue;
        }
        if(line.indexOf("}")>-1){
            tabCount--;
        }
        for(int i=0;i<tabCount;i++)
            line.prepend(spaceTabStr);
        tempSt<<line<<"\n";
        if(line.indexOf("{")>-1){
            tabCount++;
        }
    }
    *str=tempSt.readAll();
}

QString ClassGenerator::accessTypeToString(ClassAccessTypes accessType)
{
    QString result;
    switch (accessType){
    case ClassAccessTypes::Public:
        result="public";
        break;
    case ClassAccessTypes::Private:
        result="static";
        break;
    case ClassAccessTypes::PublicStatic:
        result="public static";
        break;
    case ClassAccessTypes::PrivateStatic:
        result="private static";
        break;
    case ClassAccessTypes::PublicVirtual:
        result="public virtual";
        break;
    case ClassAccessTypes::PrivateVirtual:
        result="private virtual";
        break;
    }
    return result;
}

