QT += sql core gui

TARGET   = crm
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    source/additionally/communicate.h \
    source/core/mdiwindow.h \
    source/core/logisticnamespace.h \
    source/core/logisticmainwindow.h \
    source/core/logisticapplication.h \
    source/core/dictionarycore.h \
    source/dialog/newitemdialog.h \
    source/dialog/dictionarydialog.h \
    source/dialog/countrydialog.h \
    source/dialog/contactsdialog.h \
    source/dialog/connectiondialog.h \
    source/dialog/citydialog.h \
    source/dictionary/tasktype.h \
    source/dictionary/suppliers.h \
    source/dictionary/status.h \
    source/dictionary/priorities.h \
    source/dictionary/positions.h \
    source/dictionary/dictionarytemplate.h \
    source/dictionary/countryandcity.h \
    source/dictionary/contractortype.h \
    source/dictionary/contacts.h \
    source/dictionary/customer.h \
#    source/dialog/refgrpdialog.h \
#    source/dialog/inventoriesdialog.h \
#    source/additionally/accountingoperation.h

SOURCES += \
    source/additionally/communicate.cpp \
    source/core/mdiwindow.cpp \
    source/core/main.cpp \
    source/core/logisticmainwindow.cpp \
    source/core/logisticapplication.cpp \
    source/core/dictionarycore.cpp \
    source/dialog/newitemdialog.cpp \
    source/dialog/dictionarydialog.cpp \
    source/dialog/countrydialog.cpp \
    source/dialog/contactsdialog.cpp \
    source/dialog/connectiondialog.cpp \
    source/dialog/citydialog.cpp \
    source/dictionary/tasktype.cpp \
    source/dictionary/suppliers.cpp \
    source/dictionary/status.cpp \
    source/dictionary/priorities.cpp \
    source/dictionary/positions.cpp \
    source/dictionary/dictionarytemplate.cpp \
    source/dictionary/countryandcity.cpp \
    source/dictionary/contractortype.cpp \
    source/dictionary/contacts.cpp \
    source/dictionary/customer.cpp \
#    source/dialog/refgrpdialog.cpp \
#    source/dialog/inventoriesdialog.cpp \
#    source/additionally/accountingoperation.cpp

FORMS += \
    source/dialog/newitemdialog.ui \
    source/dialog/dictionarydialog.ui \
    source/dialog/countrydialog.ui \
    source/dialog/contactsdialog.ui \
    source/dialog/connectiondialog.ui \
    source/dialog/citydialog.ui \
    source/dictionary/suppliers.ui \
    source/dictionary/dictionarytemplate.ui \
    source/dictionary/countryandcity.ui \
    source/dictionary/contacts.ui \
    source/dictionary/customer.ui \
#    source/dialog/refgrpdialog.ui \
#    source/dialog/inventoriesdialog.ui \
#    source/additionally/accountingoperation.ui


