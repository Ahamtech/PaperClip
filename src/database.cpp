/*
 * database.cpp
 *
 *  Created on: 17-Dec-2014
 *      Author: perl
 */
#include "database.hpp"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include <QtSql/QtSql>
#include <QDate>
#include <QDebug>
#include <QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/data/SqlDataAccess>
using namespace bb::cascades;
using namespace bb::data;

//class bb::cascades::GroupDataModel;
Database::Database(QObject *parent) :
        QObject(parent), DB_PATH("./data/PaperClip.db")
{
    sqlda = new SqlDataAccess(DB_PATH);
    qDebug() << "thisi is the DB" << DB_PATH;
}
Database::~Database()
{
}
bool Database::openDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_PATH);
    qDebug() << db.isValid();
    qDebug() << db.open();
    bool ok = db.open();
    return ok;
}
bool Database::initDatabase()
{ //call this method with the name of the database with
    if (openDatabase()) {
        qDebug() << "PaperClip database  created";
        QString authsTable =
                "create table if not exists auths(service varchar primary key,auth int,token varchar,tokensecret varchar,lastupdate number,username varchar,kindle_email_address varchar)";
        QString contentTable =
                "create table if not exists content(id varchar primary key unique,service varchar,date number,title varchar,url varchar ,snippet varchar,message varchar,favourite bool,archive bool,image varchar,author varchar,wordcount varchar,bookid varchar)";
        QString queueTable =
                "create table if not exists queue(id number,service varchar,date number,content number,type varchar)";
        QSqlQuery queryAuths(authsTable);
        QSqlQuery queryContent(contentTable);
        QSqlQuery queryQueue(queueTable);
        qDebug() << " auths table created" << queryAuths.isActive();
        qDebug() << "content table is created" << queryContent.isActive();
        qDebug() << "queue table is created" << queryQueue.isActive();
        return true;
    } else
        return false;
}

QVariant Database::executeQuery(QString q)
{
    return sqlda->execute(q);
}
QSqlQuery Database::executeSqlQuery(QString q)
{
    QSqlQuery query(q);
    qDebug() << "delete query is called" << query.isValid();
    return query;
}
void Database::updateusername(QString username, QString service)
{
    if (username.isEmpty() || service.isEmpty()) {
        qDebug() << "user name or the service is empty";
    } else {
        qDebug() << username <<"this is the usename";
        QSqlQuery query;
        query.prepare("update auths set username = ? where service = ?");
        query.bindValue(0, username);
        query.bindValue(1, service);
        if (query.exec()) {
            qDebug() << "query execution succcessfully";
        } else
            qDebug() << "query execution erros" << query.lastError();
    }
}
int Database::getRowCount(QString tabname){
    QSqlQuery q;
     q.prepare(QString("SELECT COUNT (*) FROM %1").arg(tabname));
     if(q.exec()){
         int rows= 0;
              if (q.next()) {
                     rows= q.value(0).toInt();
              }
              return rows;
     }
     else qDebug()<<q.lastError();

}
void Database::updateLastUpdate(QString service, int num)
{
    if (service.isEmpty() || num<0) {
        qDebug() << "lastupdate or the service is empty";
    } else {
        qDebug() << num <<"this is the lastupdate";
        QSqlQuery query;
        query.prepare("update auths set lastupdate = ? where service = ?");
        query.bindValue(0, num);
        query.bindValue(1, service);
        if (query.exec()) {
            qDebug() << "query execution succcessfully";
        } else
            qDebug() << "query execution erros" << query.lastError();
    }
}
void Database::insertQueue(QVariantMap map,QString type){
    QString query;
   if(type == "page")
       query =  "INSERT INTO queue (service,date,content,type)values(:service,:addeddate,:content,:type)";
   if(type == "article")
       query =  "INSERT INTO queue (service,date,id,type)values(:service,:addeddate,:id,:type)";
   sqlda->execute(query,map);
}

void Database::insertContent(QVariantMap map){
//    int id,QString service,int date,QString title,QString url,QString snippet,QString image,bool favourite,bool archive
    QString query =  "INSERT INTO content (id,service,date,title,url,snippet,image,favourite,archive,author,wordcount,bookid)values(:id,:service,:date,:title,:url,:snippet,:image,:favourite,:archive,:author,:wordcount,:bookid)";
   sqlda->execute(query,map);
}
void Database::updateContent(QVariantMap map){
    QString query = "update content set message = :message where id = :id";
    sqlda->execute(query,map);
}
GroupDataModel * Database::getQueryModel(QString query)
{
    GroupDataModel *model = new GroupDataModel(QStringList());
    QVariant data = sqlda->execute(query);
    model->insertList(data.value<QVariantList>());
    return model;
}
int Database::getTableSizeByQuery(QString query){
    QSqlQuery q;
     q.prepare(query);
     if(q.exec()){
         int rows= 0;
              if (q.next()) {
                     rows= q.value(0).toInt();
              }
              return rows;
     }
     else qDebug()<<q.lastError();
}
int Database::getTableSize(QString tabname){
    QSqlQuery q;
     q.prepare(QString("SELECT COUNT (*) FROM %1").arg(tabname));
     if(q.exec()){
         int rows= 0;
              if (q.next()) {
                     rows= q.value(0).toInt();
              }
              return rows;
     }
     else qDebug()<<q.lastError();
};

bool Database::insertQuery(QString query,QVariantMap bind){
    sqlda->execute(query,bind);
    return sqlda->hasError();
}
