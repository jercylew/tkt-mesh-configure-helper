#include "meshdbholder.h"
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include "globalsavingbox.h"

MeshDBHolder::MeshDBHolder(const QString &meshUUID, QObject *parent) :
    QObject(parent)
{
    this->m_meshUUID=meshUUID;
}

MeshDBHolder::~MeshDBHolder()
{
    if(db.isOpen())
    {
        db.close();
    }
}

QList<InitDbSql> MeshDBHolder::getInitDbSqlList()
{
    QList<InitDbSql> initDbSqlList;
    InitDbSql nodeGroupTableSql;
    nodeGroupTableSql.sqlName="Create Node Group Table";
    nodeGroupTableSql.sqlText="create table if not exists node_group("
                              "id int not null primary key,"
                              "name varchar(50),"
                              "color varchar(10),"
                              "group_data varchar(1000)"
                              ")";
    initDbSqlList.push_back(nodeGroupTableSql);

    InitDbSql controlPolicyTableSql;
    controlPolicyTableSql.sqlName="Create Control Item Policy Table";
    controlPolicyTableSql.sqlText="create table if not exists control_policy("
                                          "id int not null primary key,"
                                          "group_id int not null,"
                                          "name varchar(50),"
                                          "color varchar(10),"
                                          "description varchar(200),"
                                          "sub_id int not null default 0"
                                          ")";
    initDbSqlList.push_back(controlPolicyTableSql);

    InitDbSql controlItemTableSql;
    controlItemTableSql.sqlName="Create Control Item Table";
    controlItemTableSql.sqlText="create table if not exists control_item("
                                "id int not null primary key,"  //The id denoting itself
                                "group_id int not null,"        //The group associated with
                                "policy_sub_id int not null,"   //The sub id identifying the policy
                                "control_data varchar(1000)"    //The actual json-formatted control data
                                ")";

    initDbSqlList.push_back(controlItemTableSql);

    InitDbSql lightNodeTableSql;
    lightNodeTableSql.sqlName="Create Executer Node Table";
    lightNodeTableSql.sqlText="create table if not exists executer_node("
                              "executer_id varchar(50) not null primary key,"
							  "executer_type varchar(50) not null,"
                              "executer_name varchar(50),"
                              "executer_data varchar(1000)"
                              ")";
    initDbSqlList.push_back(lightNodeTableSql);

    InitDbSql sensorNodeTableSql;
    sensorNodeTableSql.sqlName="Create Sensor Node Table";
    sensorNodeTableSql.sqlText="create table if not exists sensor_node("
                               "sensor_id varchar(50) not null primary key,"
                               "sensor_type varchar(30) not null,"
                               "sensor_name varchar(50),"
                               "sensor_data varchar(1000)"
                               ")";
    initDbSqlList.push_back(sensorNodeTableSql);

    InitDbSql historyDataTableSql;
    historyDataTableSql.sqlName="Create History Data Table";
    historyDataTableSql.sqlText="create table if not exists history_data("
                                "id int not null primary key,"
                                "year int not null,"
                                "month int not null,"
                                "day int not null,"
                                "group_id int not null,"
                                "node_id int not null,"
                                "energy double not null)";
    initDbSqlList.push_back(historyDataTableSql);

    InitDbSql controlDateTableSql;
    controlDateTableSql.sqlName="Create Control Date Table";
    controlDateTableSql.sqlText="create table if not exists control_date("
                                "id varchar(50) not null primary key,"
                                "group_id int not null,"
                                "month int not null,"
                                "day int not null,"
                                "policy_id integer"
                                ")";
    initDbSqlList.push_back(controlDateTableSql);

    InitDbSql templateControlPolicyTableSql;
    templateControlPolicyTableSql.sqlName="Create Template Control Policy Table";
    templateControlPolicyTableSql.sqlText="create table if not exists template_control_policy("
                                                  "policy_id varchar(50) not null primary key,"
                                                  "policy_name varchar(50),"
                                                  "group_type varchar(50),"
                                                  "create_msec int not null"
                                                  ")";
    initDbSqlList.push_back(templateControlPolicyTableSql);

    InitDbSql templateControlPolicyPropertyTableSql;
    templateControlPolicyPropertyTableSql.sqlName="Create Template Control Policy Property Table";
    templateControlPolicyPropertyTableSql.sqlText="create table if not exists template_control_policy_property("
                                          "policy_id varchar(100) not null,"
                                          "property_name varchar(100) not null,"
                                          "property_value varchar(1000),"
                                          "primary key (policy_id, property_name)"
                                          ")";
    initDbSqlList.push_back(templateControlPolicyPropertyTableSql);


    InitDbSql templateControlItemTableSql;
    templateControlItemTableSql.sqlName="Create Template Control Item Table";
    templateControlItemTableSql.sqlText="create table if not exists template_control_item("
                                                  "item_id varchar(100) not null primary key,"
                                                  "policy_id varchar(100) not null,"
                                                  "start_minute int not null,"
                                                  "control_type varchar(50)"
                                                  ")";
    initDbSqlList.push_back(templateControlItemTableSql);

    InitDbSql templateControlItemPropertyTableSql;
    templateControlItemPropertyTableSql.sqlName="Create Template Control Item Property Table";
    templateControlItemPropertyTableSql.sqlText="create table if not exists template_control_item_property("
                                                "item_id varchar(100) not null,"
                                                "property_name varchar(100) not null,"
                                                "property_value varchar(1000),"
                                                "primary key (item_id, property_name)"
                                                ")";
    initDbSqlList.push_back(templateControlItemPropertyTableSql);


    return initDbSqlList;
}

QString MeshDBHolder::getDbPath()
{
    return GlobalSavingBox::getInstance()->getMeshDBFilePath();
}

void MeshDBHolder::initDB(bool initDbStructure)
{
    if(db.isOpen())
    {
        db.close();
    }
    if(QSqlDatabase::contains(this->m_meshUUID))
    {
        db=QSqlDatabase::database(this->m_meshUUID);
    }
    else
    {
        db=QSqlDatabase::addDatabase("QSQLITE", this->m_meshUUID);
    }
    db.setDatabaseName(getDbPath());
    db.open();
    if(initDbStructure)
    {
        QList<InitDbSql> initDbSqlList=getInitDbSqlList();
        bool lastQueryResultIsEmpty=true;
        for(int i=0; i<initDbSqlList.size(); i++)
        {
            QSqlQuery query(db);
            InitDbSql initDbSql=initDbSqlList.at(i);
            qDebug()<<initDbSql.sqlText;
            if(initDbSql.execWhenLastQueryRecordNumberIsZero)
            {
                if(lastQueryResultIsEmpty)
                {
                    query.exec(initDbSql.sqlText);
                    if(query.next())
                    {
                        lastQueryResultIsEmpty=false;
                    }
                    else
                    {
                        lastQueryResultIsEmpty=true;
                    }
                    qDebug()<<initDbSql.sqlName<<", error="<<query.lastError().text();
                }
            }
            else
            {
                query.exec(initDbSql.sqlText);
                if(query.next())
                {
                    lastQueryResultIsEmpty=false;
                }
                else
                {
                    lastQueryResultIsEmpty=true;
                }
                qDebug()<<initDbSql.sqlName<<", error="<<query.lastError().text();
            }
            qDebug()<<"lastQueryResultIsEmpty="<<lastQueryResultIsEmpty;
        }
    }
}

QSqlDatabase MeshDBHolder::getDb()
{
    return this->db;
}
