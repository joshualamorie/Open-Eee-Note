#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include "inputdialog.h"
class QString;
#ifdef ___cplusplus
extern "C" {
#endif

    void setDefaultFont();
    bool checkPassword(const QString& pwd, InputDialog* pDialog);
    bool isSystemPasswordExist();

    int getExtendSDCardFreeSpace();
    int getInternalSDCardFreeSpace();
    bool isEnoughSapceInternalSDCard();
    bool isEnoughSpaceExtendSDCard();
    void setProcessPriority(int priority);

    void setCPUAuto();
    void setCPUP4();
    int isAutoCPUState();


#ifdef ___cplusplus
}
#endif

#endif // COMMONFUNCTION_H
