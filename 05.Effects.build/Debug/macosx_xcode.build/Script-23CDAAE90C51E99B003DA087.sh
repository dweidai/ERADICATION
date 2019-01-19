#!/bin/sh
install_name_tool -change /usr/local/lib/libirrklang.dylib @executable_path/libirrklang.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME"
