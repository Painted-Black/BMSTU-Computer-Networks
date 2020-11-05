TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        core/connection_handler.cpp \
        core/request.cpp \
        core/responce.cpp \
        core/rest_data.cpp \
        core/rest_handler.cpp \
        core/rest_route.cpp \
        core/rest_server.cpp \
		core/server.cpp \
	handler/handler_statistic.cpp \
	handler/html_file_handler.cpp \
        main.cpp \
        statistic/statistic.cpp \
        statistic/statistic_server.cpp \
        thread_pool.cpp

HEADERS += \
    core/connection_handler.h \
    core/request.h \
    core/responce.h \
    core/rest_data.h \
    core/rest_handler.h \
    core/rest_route.h \
    core/rest_server.h \
	core/server.h \
	handler/handler_statistic.h \
	handler/html_file_handler.h \
    statistic/statistic.h \
    statistic/statistic_server.h \
    thread_pool.h

LIBS += -pthread
