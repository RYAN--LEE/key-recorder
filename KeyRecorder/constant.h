#pragma once
#include <QString>
#include <QStandardPaths> 

//const QString CONFIG_DIR = "./config/";
const QString CONFIG_DIR = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+ "/sunlight/config";
const QString IMG_DIR = CONFIG_DIR + "/image/";
const QString FILE_KEY = CONFIG_DIR + "/key_config.cfg";
const QString FILE_AREA = CONFIG_DIR + "/area_config.cfg";
const QString FILE_CFG = CONFIG_DIR + "/configure.cfg";
const QString FILE_WINDOW_NAME = CONFIG_DIR + "/window.cfg";

const QString API_CREATE_CARD = "createcard";
const QString API_GET_ROOM_NUM = "getroomnum";