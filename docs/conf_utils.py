import os
from datetime import datetime, timezone
from git import Repo

__GIT_ROOT = os.path.join(os.path.dirname(__file__), "..")
__REPO = Repo(__GIT_ROOT)

def get_version(dev: bool, wdate: bool) -> str:
    # get latest tag (v0.0.0 if there are none)
    tags = __REPO.tags
    current_tag = str(tags[-1]) if len(tags) > 0 else "v0.0.0"

    version_str = current_tag

    # dev appends dev_<commit SHA>...
    if dev:
        # get latest commit
        head = __REPO.head
        sha = head.commit.hexsha[:7]

        version_str += "dev_" + sha

    # wdate appends the date of latest commit...
    if wdate:
        # get date of latest commit
        date_epoch = head.commit.committed_date
        date_time = datetime.fromtimestamp(date_epoch, timezone.utc)
        date = str(date_time.date())

        version_str += " (" + date + ")"

    return version_str
