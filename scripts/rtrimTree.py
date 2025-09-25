import os
import sys
import fileinput
import operator
import argparse
parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
description='''
Strip trailing whitespace from a tree of code files while making no
other formatting changes.  In particular, don't update line endings.
Optionally, additional tweaks can be applied, see --help.

WARNING: This script is destructive - it modifies files in place without warning.

''',
epilog='''
''')

parser.add_argument('--xd', type=str, nargs='+', default=['.git', 'external'],
    help='Exclude directories')
parser.add_argument('--filetypes', '-t', metavar='ext', type=str, nargs='+',
    help='File Types to process', default =  ["c", "h"])
parser.add_argument('--removeSoloComments',  default=False, action='store_true',
    help='Remove empty comment lines that have no adjacent comments.')
parser.add_argument('--dontTrim',  default=False, action='store_true',
    help='Perform only fixups, not including actual trimming.  e.g. Only remove empty comment lines.')
parser.add_argument('--verbose', '-v', default=False, action='store_true',
    help='Report parameters and all files processed, regardless of changes.')

args = parser.parse_args()

def applyFixups(lines):
    # true when inbetween /* and */
    inMultilineComment = False
    # true when previous line started with "//"
    # i.e. was not a line-end comment
    lastWasFullSingleLineComment = False
    # True when previous line was an empty comment that was removed
    lastWasRemovedSingleLineComment = False
    fixed = 0
    emptyComments = 0

    # note: lines is an array of byte arrays, not a strings
    for k, line in enumerate(lines):
        if len(line) != 0:
            # detect line ending so it can be preserved
            hasCR = (line[-1:] == b'\r')

            # first perform the primary purpose, strip right whitespace
            new_line = line.rstrip()

            # second perform solo comment tracking
            # this is very naive and handles only single-line C/C++ comments ("//")
            # if triming the line of white space results in exactly "//" then delete it unless
            # it follows another comment on the previous line or is part of a multi-line comment.
            # Track multi-line comments for starting and ending in a very
            # brute force way "/*" starts and "*/" ends if the line contains only one of them.
            # so single line annotations /*abc*/ won't change the comment tracking state
            # EVEN if the line ends in /*, such as foo(/*abc*/x, y); /* start long comment
            if args.removeSoloComments:
                tmp = new_line.lstrip()
                isFullLineComment = False
                if tmp[0:2] == b"//":
                    isFullLineComment = True
                else:
                    if operator.contains(line, b"/*"):
                        inMultilineComment = True
                    if  operator.contains(line, b"*/"):
                        inMultilineComment = False

                isEmptyComment = (tmp == b"//")

                # if this is a full-line, non-empty comment,
                # and the last line was a removed empty comment, restore previous line.
                # This solves patterns like:
                # //
                # // some comment
                # //

                if isFullLineComment and not isEmptyComment and lastWasRemovedSingleLineComment:
                    lines[k-1] = previousOriginalLine
                    emptyComments -= 1
                    fixed -= 1

                if isEmptyComment and not lastWasFullSingleLineComment and not inMultilineComment:
                    new_line = b''
                    # reset flag so this empty line doesn't
                    # protect a following empty line
                    isFullLineComment = False
                    lastWasRemovedSingleLineComment = True
                    emptyComments += 1
                else:
                    lastWasRemovedSingleLineComment = False

                lastWasFullSingleLineComment = isFullLineComment

            # put CR back if it was removed.
            # LF is handled in caller when line array is joined.
            if hasCR:
                new_line = new_line + b'\r'

            # save the current line in case we need to restore it
            # on the next line because it was an empty comment
            previousOriginalLine = lines[k]

            # if not doing trimming, save only if a special case
            if args.dontTrim and not lastWasRemovedSingleLineComment:
                pass
            elif len(line) != len(new_line):
                lines[k] = new_line
                fixed += 1

    return (fixed, emptyComments)

def rightTrimFileWhitespace(fname):
    if args.verbose:
        print("processing: %s" % (fname))
    # slurp file in binary to preserve line endings
    with open(fname, "rb") as infile:
        contents = infile.read()
    # break at LF so we can detect CRLF and LF differences
    lines = contents.split(b'\n')

    (linesFixed, emptyComments) = applyFixups(lines)

    # an assert by any other name
    if emptyComments != 0 and not args.removeSoloComments:
        raise Exception("Unexpected comment modifications occured")

    with open(fname, "wb") as fo:
        fo.write(b'\n'.join(lines))

    if linesFixed or args.verbose:
        if args.removeSoloComments:
            print("*** %s (%d/%d)" % (os.path.basename(fname), linesFixed, emptyComments), file=sys.stderr)
        else:
            print("*** %s (%d)" % (os.path.basename(fname), linesFixed), file=sys.stderr)


# skip these folders
excludeDirs = set(args.xd)
# turn extension list into actual file extensions
includeFileTypes = tuple(['.'  + t for t in args.filetypes])

if args.verbose:
    print("excludeDirs: %s" % (excludeDirs))
    print("  filetypes: %s" % (str(includeFileTypes)))
    print(" removeSolo: %s" % (args.removeSoloComments))
    print("    verbose: %s" % (args.verbose))

for dirpath, dirs, files in os.walk("./", topdown=True):
    dirs[:] = [d for d in dirs if d not in excludeDirs]
    for filename in files:
        if filename.endswith(includeFileTypes):
            fname = os.path.join(dirpath,filename)
            rightTrimFileWhitespace(fname)
