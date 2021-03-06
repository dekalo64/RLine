QT += sql core gui

TARGET   = crm
TEMPLATE = app

CONFIG += exceptions

QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
#    source/crm_dialog/dlg_refgrp.h \
#    source/crm_dialog/dlg_inventories.h \
#    source/crm_additionally/adl_accountingoperation.h \
    source/crm_additionally/adl_communicate.h \
    source/crm_core/core_logisticapplication.h \
    source/crm_core/core_logisticmainwindow.h \
    source/crm_core/core_logisticnamespace.h \
    source/crm_core/core_mdiwindow.h \
    source/crm_core/core_dictionarycore.h \
    source/crm_dictionary/dct_cppsst.h \
    source/crm_dialog/dlg_city.h \
    source/crm_dialog/dlg_additem.h \
    source/crm_dialog/dlg_connection.h \
    source/crm_dialog/dlg_contacts.h \
    source/crm_dialog/dlg_country.h \
    source/crm_dialog/dlg_cppsst.h \
    source/crm_dialog/dlg_customer.h \
    source/crm_dialog/dlg_supplier.h \
    source/crm_dialog/dlg_discount.h \
    source/crm_dialog/dlg_customer_g.h \
    source/crm_dialog/dlg_supplier_g.h \
    source/crm_dictionary/dct_contacts.h \
    source/crm_dictionary/dct_contractor.h \
    source/crm_dictionary/dct_countrycity.h \
    source/crm_dictionary/dct_customer.h \
    source/crm_dictionary/dct_positions.h \
    source/crm_dictionary/dct_priorities.h \
    source/crm_dictionary/dct_status.h \
    source/crm_dictionary/dct_suppliers.h \
    source/crm_dictionary/dct_task.h \
    source/crm_dialog/dlg_message.h

SOURCES += \
#    source/crm_dialog/dlg_refgrp.cpp \
#    source/crm_dialog/dlg_inventories.cpp \
#    source/crm_additionally/adl_accountingoperation.cpp \
    source/crm_additionally/adl_communicate.cpp \
    source/crm_core/core_logisticapplication.cpp \
    source/crm_core/core_logisticmainwindow.cpp \
    source/crm_core/core_main.cpp \
    source/crm_core/core_mdiwindow.cpp \
    source/crm_dialog/dlg_city.cpp \
    source/crm_dialog/dlg_connection.cpp \
    source/crm_dialog/dlg_contacts.cpp \
    source/crm_dialog/dlg_country.cpp \
    source/crm_dialog/dlg_additem.cpp \
    source/crm_dialog/dlg_customer.cpp \
    source/crm_dialog/dlg_supplier.cpp \
    source/crm_dialog/dlg_discount.cpp \
    source/crm_dialog/dlg_customer_g.cpp \
    source/crm_dialog/dlg_supplier_g.cpp \
    source/crm_dictionary/dct_contacts.cpp \
    source/crm_dictionary/dct_countrycity.cpp \
    source/crm_dictionary/dct_contractor.cpp \
    source/crm_dictionary/dct_customer.cpp \
    source/crm_dictionary/dct_positions.cpp \
    source/crm_dictionary/dct_priorities.cpp \
    source/crm_dictionary/dct_status.cpp \
    source/crm_dictionary/dct_suppliers.cpp \
    source/crm_dictionary/dct_task.cpp \
    source/crm_core/core_dictionarycore.cpp \
    source/crm_dictionary/dct_cppsst.cpp \
    source/crm_dialog/dlg_cppsst.cpp \
    source/crm_dialog/dlg_message.cpp

FORMS += \
    source/crm_dictionary/dct_contacts.ui \
    source/crm_dictionary/dct_countrycity.ui \
    source/crm_dictionary/dct_customer.ui \
    source/crm_dictionary/dct_cppsst.ui \
    source/crm_dictionary/dct_suppliers.ui \
    source/crm_dialog/dlg_connection.ui \
    source/crm_dialog/dlg_city.ui \
    source/crm_dialog/dlg_additem.ui \
    source/crm_dialog/dlg_country.ui \
    source/crm_dialog/dlg_contacts.ui \
    source/crm_dialog/dlg_cppsst.ui \
    source/crm_dialog/dlg_customer.ui \
    source/crm_dialog/dlg_supplier.ui \
    source/crm_dialog/dlg_discount.ui \
    source/crm_dialog/dlg_customer_g.ui \
    source/crm_dialog/dlg_supplier_g.ui \
#    source/crm_dialog/dlg_refgrp.ui \
#    source/crm_dialog/dlg_inventories.ui \
#    source/crm_additionally/adl_accountingoperation.ui


