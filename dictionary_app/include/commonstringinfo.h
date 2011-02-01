#ifndef COMMONSTRINGINFO_H
#define COMMONSTRINGINFO_H

#include<QString>
#include<QStringList>
#include<QMap>

class CommonStringInfo
{
public:
    typedef enum _InfoType{
        Error_Common_NoSpace = 0,
        Error_Common_NameError,
        Info_Total
    }InfoType;

public:
    class Release{
    public:
        ~Release()
        {

        }
    };

public:
    const QString& getString(InfoType type);
    static CommonStringInfo* getInstance();

protected:
    CommonStringInfo();

private:
    QMap<int, QString> m_infoList;
    static CommonStringInfo* instance;
};

#endif // COMMONSTRINGINFO_H
