#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T15:57:01
#
#-------------------------------------------------

QT       += core gui xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VaccinationSchedule
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += console

SOURCES += \
        main.cpp \
    vaccine.cpp \
    main_widget.cpp \
    registry_widget.cpp \
    vaccines_widget.cpp \
    schedule_widget.cpp \
    add_child_widget.cpp \
    add_vaccine_widget.cpp \
    settings_model.cpp \
    vaccines_settings_widget.cpp \
    children_model.cpp \
    child_model.cpp \
    vaccines_model.cpp \
    schedule_model.cpp \
    proxysearchmodel.cpp \
    qdateedit_delegate.cpp \
    qlineedit_delegate.cpp

HEADERS += \
    vaccine.h \
    main_widget.h \
    registry_widget.h \
    vaccines_widget.h \
    schedule_widget.h \
    add_child_widget.h \
    add_vaccine_widget.h \
    settings_model.h \
    vaccines_settings_widget.h \
    children_model.h \
    child_model.h \
    vaccines_model.h \
    schedule_model.h \
    proxysearchmodel.h \
    qdateedit_delegate.h \
    qlineedit_delegate.h

RESOURCES += \
    resources.qrc
