#
#   Copyright (c) 2023 Jack Bennett
#   All rights reserved.
#
#   Please see the LICENCE file for more information.
#

# GetVersion.py
# ----
# Produce a dynamically-generated project version string from the latest Git
# tag and commit names, as well as the date of the last commit.
# Note! This MUST be executed from the root directory (thallium/)!!!
# Pass "cfg" as a CLI argument to also create a build/generated/dyn_version.h file.

# dependencies:
# GitPython (https://github.com/gitpython-developers/GitPython)

# if this is true, "dev_" + the latest commit SHA will be appended onto the version.
# this is hardcoded to true for now as all versions as of now are 'prereleases'
PRERELEASE = True

import os
import sys
from datetime import datetime, timezone
from git import Repo

workingdir = os.getcwd()
repo = Repo(workingdir)

# get latest commit

head = repo.head
sha = head.commit.hexsha[:7]

# get latest tag
# (v0.0.0 if there are none)

tags = repo.tags
current_tag = str(tags[-1]) if len(tags) > 0 else "v0.0.0"

# get date of latest commit

date_epoch = head.commit.committed_date
date_time = datetime.fromtimestamp(date_epoch, timezone.utc)

date = str(date_time.date())


# concatenate + conditional dev_ suffix

version_str = current_tag

if PRERELEASE:
    version_str += "dev_" + sha

version_str += " (" + date + ")"

if len(sys.argv) > 1 and sys.argv[1] == "cfg":
    header_path = "build/generated/"

    if not os.path.exists(header_path):
        os.makedirs(header_path)

    with open(os.path.join(header_path, "dyn_version.h"), "w") as file:
        file.write("""#pragma once
#ifndef _DYN_VERSION_H
#define _DYN_VERSION_H

#define THALLIUM_VERSION \"{0}\"

#endif // _DYN_VERSION_H
        """.format(version_str))
else:
    # output -> version_str
    print(version_str)
