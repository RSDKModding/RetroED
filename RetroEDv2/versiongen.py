from datetime import datetime
import pytz
import os
import sys
import re

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
n = time.strftime("%Y%m%d%H%M")

if os.path.exists("version.hpp"):
    for line in open("version.hpp", "r").readlines():
        match = re.match(r"#define\s+RE_VERSION\s+\(\"(.*)\"\)", line)
        if match and match.groups()[0] == s:
            print("version matches; nothing to be done")
            exit()

open("version.hpp", "w").write(
    "#pragma once\n\n"
    f"#define RE_VERSIONNUM ({n}ULL)\n"
    f"#define RE_VERSION    (\"{s}\")\n"
    f"#define RE_BUILD_TYPE ({buildtype})\n"
)
print(f"wrote version {s}")