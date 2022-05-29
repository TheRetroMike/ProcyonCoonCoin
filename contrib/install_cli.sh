 #!/usr/bin/env bash

 # Execute this file to install the procyon cli tools into your path on OS X

 CURRENT_LOC="$( cd "$(dirname "$0")" ; pwd -P )"
 LOCATION=${CURRENT_LOC%Procyon-Qt.app*}

 # Ensure that the directory to symlink to exists
 sudo mkdir -p /usr/local/bin

 # Create symlinks to the cli tools
 sudo ln -s ${LOCATION}/Procyon-Qt.app/Contents/MacOS/procyond /usr/local/bin/procyond
 sudo ln -s ${LOCATION}/Procyon-Qt.app/Contents/MacOS/procyon-cli /usr/local/bin/procyon-cli
