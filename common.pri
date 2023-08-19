CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d
} else {
    BUILD_FLAG = release
}

OBJECTS_DIR = .build$${LIB_SUFFIX}/obj
MOC_DIR = .build$${LIB_SUFFIX}/moc
UI_DIR = .build$${LIB_SUFFIX}/ui

