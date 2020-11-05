TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        core/httpmanager.cpp \
        core/request.cpp \
        core/responce.cpp \
		core/rest_data.cpp \
        core/tcp_socket.cpp \
        core/url.cpp \
        main.cpp

HEADERS += \
	core/httpmanager.h \
	core/request.h \
	core/responce.h \
	core/rest_data.h \
	core/tcp_socket.h \
	core/url.h
