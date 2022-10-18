from datetime import datetime
import pytz
import sys

buildadd = "dev"
buildtype = 0

try:
    buildadd = sys.argv[1]
except:
    pass

if len(buildadd) >= 7:  # autobuild commit hash
    buildtype = 2
    buildadd = "-" + buildadd[:7]
elif buildadd != "dev":
    buildtype = 1
else:
    buildadd = "-dev"


time = datetime.now(pytz.timezone("US/Eastern"))
s = time.strftime("v%Y.%m.%d") + buildadd

LINES = [
    "#pragma once\n\n"
    f"#define RE_VERSION    (\"{s}\")\n",
    f"#define RE_BUILD_TYPE ({buildtype})\n",
]

open("version.hpp", "w").writelines(LINES)
