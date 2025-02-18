#!/usr/bin/env python

import logging
import os
from os.path import dirname

from setuptools import setup

logger = logging.getLogger(__name__)

version = '0.5.0.dev0'

my_dir = dirname(__file__)

here = os.path.abspath(os.path.dirname(__file__))


def git_version(version_: str) -> str:
    """
    Return a version to identify the state of the underlying git repo. The version will
    indicate whether the head of the current git-backed working directory is tied to a
    release tag or not : it will indicate the former with a 'release:{version}' prefix
    and the latter with a 'dev0' prefix. Following the prefix will be a sha of the current
    branch head. Finally, a "dirty" suffix is appended to indicate that uncommitted
    changes are present.
    :param str version_: Semver version
    :return: Found Furiosa SDK version in Git repo
    :rtype: str
    """
    try:
        import git

        try:
            repo = git.Repo(os.path.join(*[my_dir, '..', '..', '.git']))
        except git.NoSuchPathError:
            logger.warning('.git directory not found: Cannot compute the git version')
            return ''
        except git.InvalidGitRepositoryError:
            logger.warning('Invalid .git directory not found: Cannot compute the git version')
            return ''
    except ImportError:
        logger.warning('gitpython not found: Cannot compute the git version.')
        return ''
    if repo:
        sha = repo.head.commit.hexsha
        if repo.is_dirty():
            return f'.dev0+{sha}.dirty'
        # commit is clean
        return f'.release:{version_}+{sha}'
    return 'no_git_version'


def write_version(filename: str = os.path.join(*[my_dir, "furiosa_sdk_model_validator", "git_version"])):
    """
    Write the Semver version + git hash to file, e.g. ".dev0+2f635dc265e78db6708f59f68e8009abb92c1e65".
    :param str filename: Destination file to write
    """
    text = f"{git_version(version)}"
    with open(filename, 'w') as file:
        file.write(text)


if __name__ == "__main__":
    setup_kwargs = {}

    write_version()

    setup(
        version=version,
        scripts=['bin/furiosa-validate'],
        **setup_kwargs,
    )
