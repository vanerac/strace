#!/usr/bin/env sh

wget https://raw.githubusercontent.com/strace/strace/master/src/sysent.h && mv sysent.h ./include/
wget https://raw.githubusercontent.com/strace/strace/master/src/sysent_shorthand_defs.h && mv sysent_shorthand_defs.h ./include/
wget https://raw.githubusercontent.com/strace/strace/master/src/linux/generic/syscallent-common.h && mv syscallent-common.h ./include/generic/
wget https://raw.githubusercontent.com/strace/strace/master/src/linux/generic/subcallent.h && mv subcallent.h ./include/generic/

wget https://raw.githubusercontent.com/strace/strace/master/src/linux/x86_64/syscallent.h && mv syscallent.h ./include/x86_64/

wget https://raw.githubusercontent.com/strace/strace/master/src/linux/32/subcallent.h && mv subcallent.h ./include/32/
wget https://raw.githubusercontent.com/strace/strace/master/src/linux/32/syscallent-common-32.h && mv syscallent-common-32.h ./include/32/
wget https://raw.githubusercontent.com/strace/strace/master/src/linux/i386/syscallent.h && mv syscallent.h ./include/i386/