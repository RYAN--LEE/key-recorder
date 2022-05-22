#pragma once
#include <QString>
#include <QStandardPaths> 

//const QString CONFIG_DIR = "./config/";
const QString HOME_DIR = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/sunlight";
const QString CONFIG_DIR = HOME_DIR + "/config";
const QString IMG_DIR = CONFIG_DIR + "/image/";
const QString CFG_KEY = "key_config.cfg";
const QString FILE_KEY = CONFIG_DIR + "/" + CFG_KEY;
const QString FILE_KEY_BASE = CONFIG_DIR + "/" + "key_config_base.cfg";
const QString FILE_KEY_PEOPLE = CONFIG_DIR + "/" + "key_config_people.cfg";
const QString FILE_KEY_DETECT = CONFIG_DIR + "/" + "key_config_detect.cfg";
const QString FILE_AREA = CONFIG_DIR + "/area_config.cfg";
const QString FILE_CFG = CONFIG_DIR + "/configure.cfg";
const QString FILE_ITEM_CFG = CONFIG_DIR + "/item.cfg";
const QString FILE_ORIDATA_CFG = CONFIG_DIR + "/oridata.cfg";
const QString FILE_WINDOW_NAME = CONFIG_DIR + "/window.cfg";

const QString API_CREATE_CARD = "createcard";
const QString API_GET_ROOM_NUM = "getroomnum";