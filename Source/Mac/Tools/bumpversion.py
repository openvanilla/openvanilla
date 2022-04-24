#!/usr/bin/env python3

import argparse
import plistlib
import re
import subprocess
import sys

VER_PATTERN = re.compile(r'(\d+(\.\d+)*)(.*)')


def upgrade(dry_run, revision_number, version_name, commit, plists):
    if revision_number:
        # value must be a string, lest an <integer> tag be used in plist
        target_rev = str(revision_number)
    else:
        # revs has an empty line, so len(rev) gives us what we want
        # (which is commit count + 1)
        s = subprocess.check_output(
            ["git", "rev-list", "HEAD"]).decode('utf-8')
        # value must be a string, lest an <integer> tag be used in plist
        target_rev = str(len(s.split("\n")))

    latest = None

    for path in plists:
        with open(path, 'rb') as f:
            plist = plistlib.load(f)

        bundle_version = int(plist["CFBundleVersion"])
        new_bundle_version = target_rev

        short_version_string = plist["CFBundleShortVersionString"]
        if version_name:
            new_short_version_string = version_name
        else:
            matches = VER_PATTERN.match(short_version_string)
            if matches:
                short_version_components = matches.group(1).split(".")
                short_version_components[-1] = str(
                    int(short_version_components[-1]) + 1)
                new_short_version_string = ".".join(
                    short_version_components) + matches.group(3)
            else:
                new_short_version_string = short_version_string

        msg = ('{path}: {old_name} ({old_rev}) -> '
               '{new_name}{name_info} ({new_rev}{rev_info}){dry_run}').format(
            path=path,
            old_name=short_version_string,
            old_rev=bundle_version,
            new_name=new_short_version_string,
            name_info=('' if version_name else ' [inferred]'),
            new_rev=new_bundle_version,
            rev_info=('' if revision_number else ' [inferred]'),
            dry_run=(' (dry run)' if dry_run else ''))
        print(msg)

        if not dry_run:
            plist["CFBundleVersion"] = new_bundle_version
            plist["CFBundleShortVersionString"] = new_short_version_string
            latest = plist["CFBundleShortVersionString"]
            with open(path, 'wb') as f:
                plistlib.dump(plist, f)

    if latest and commit:
        msg = "Bump to %s" % latest
        cmd = ["git", "commit", "-m", msg] + plists
        print('committing with command: %s' % cmd)
        subprocess.call(cmd)


def main():
    parser = argparse.ArgumentParser(description='bump Info.plist versions')
    parser.add_argument('plists', metavar='PATH', nargs='+', type=str)
    parser.add_argument('-n', '--dry-run', action='store_true',
                        help='dry run (preview the results')
    parser.add_argument(
        '--revision-number', type=int,
        help='default is inferred from number of commits plus one')
    parser.add_argument(
        '--version-name',
        help='default is the last current version component by one')
    parser.add_argument(
        '--commit', action='store_true',
        help='Commit the changes using the last version string as message')
    args = parser.parse_args()
    upgrade(args.dry_run, args.revision_number,
            args.version_name, args.commit, args.plists)

if __name__ == "__main__":
    main()
