VERSION = 1.1

QT += quick concurrent
CONFIG += c++1z c++17
CONFIG += release

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = src/Qml/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

RESOURCES += qml.qrc

INCLUDEPATH += src/

HEADERS += \

SOURCES += \
	src/main.cpp

release:DESTDIR = release
release:OBJECTS_DIR = release/obj
release:MOC_DIR = release/moc
release:RCC_DIR = release/rcc
release:UI_DIR = release/ui

unix {
	# If it is unix* OS -> define flag 'no-pie' to build executable (see: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
	QMAKE_LFLAGS += -no-pie
}

win32 {
	release:openssl.path = $$OUT_PWD/release
	openssl.files += $$PWD/lib/OpenSSL-Win32/*.dll
	INSTALLS += openssl
	system(chcp 65001 && echo "No" | xcopy $$shell_quote($$shell_path($$openssl.files)) \
			$$shell_quote($$shell_path($$openssl.path)) /V /F /R /-Y)
}