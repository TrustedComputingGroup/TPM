#  The copyright in this software is being made available under the BSD License,
#  included below. This software may be subject to other third party and
#  contributor rights, including patent rights, and no such rights are granted
#  under this license.
#
#  Copyright (c) Microsoft Corporation
#
#  All rights reserved.
#
#  BSD License
#
#  Redistribution and use in source and binary forms, with or without modification,
#  are permitted provided that the following conditions are met:
#
#  Redistributions of source code must retain the above copyright notice, this list
#  of conditions and the following disclaimer.
#
#  Redistributions in binary form must reproduce the above copyright notice, this
#  list of conditions and the following disclaimer in the documentation and/or other
#  materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ""AS IS""
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
import os
import sys
import fileinput
import operator
import argparse
parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
description='''
Detect instances of given EOL format. No changes are made.
If a file has the requested line ending, then the filename is output with
counts displayed as (LF/CRLF).

''',
epilog='''
''')

parser.add_argument('--xd', type=str, nargs='+', default=['.git', 'external'],
    help='Exclude directories')
parser.add_argument('--filetypes', '-t', metavar='ext', type=str, nargs='+',
    help='File Types to process', default =  ["c", "h"])
parser.add_argument('--verbose', '-v', default=False, action='store_true',
    help='Report parameters and all files processed, regardless of findings.')
parser.add_argument('--showerrors', '-e', default=False, action='store_true',
    help='Show lines that have the requested line endings (errors).')
parser.add_argument('--showcounts', '-c', default=False, action='store_true',
    help='Show line ending counts for all files.')
parser.add_argument('--crlf', default=False, action='store_true',
    help='Find/Count CRLF line endings.')
parser.add_argument('--lf', default=False, action='store_true',
    help='Find/Count LF-only line endings.')

totalLF = 0
totalCRLF = 0


args = parser.parse_args()
count = 0
if args.crlf:
    count = count+1
if args.lf:
    count = count+1

if (count != 1):
    raise Exception("exactly one --crlf, --lf option required.")

def countEndings(lines, endsWithLinebreak):
    countLF = 0
    countCRLF = 0

    # note: lines is an array of byte arrays, not a strings
    for k, line in enumerate(lines):

        # don't count last line if there is no line break at the end.
        if args.verbose and k == len(lines)-1 and not endsWithLinebreak:
            continue

        hasCRLF = False
        if len(line) != 0:
            # detect line ending
            hasCRLF = (line[-1:] == b'\r')

        if hasCRLF:
            countCRLF = countCRLF+1
        else:
            countLF = countLF+1

        # print lines in error if requested
        if args.showerrors and ((args.lf and not hasCRLF) or (args.crlf and hasCRLF)):
            print("*** %s (%d:%s)" % (os.path.basename(fname), k+1, line), file=sys.stdout)

    return (countLF, countCRLF)

def processFile(fname):

    # slurp file in binary to preserve line endings
    with open(fname, "rb") as infile:
        contents = infile.read()

    # check to see if line ends on a line break or not
    endsWithLinebreak = (contents[-1:] == b'\n')
    if args.verbose:
        if not endsWithLinebreak:
            print("processing: %s (missing final linebreak)" % (fname))
        else:
            print("processing: %s" % (fname))

    # break at LF so we can detect CRLF and LF differences
    lines = contents.split(b'\n')
    # if the file ends with a line break, then there will be one extra null line
    # after split, remove it.
    lines = lines[:len(lines)-1]
    (countLF, countCRLF) = countEndings(lines, endsWithLinebreak)

    global totalLF
    global totalCRLF
    totalLF += countLF
    totalCRLF += countCRLF

    if args.showcounts or (args.lf and countLF != 0) or (args.crlf and countCRLF != 0):
        print("*** %s (%d/%d)" % (os.path.basename(fname), countLF, countCRLF), file=sys.stdout)

# skip these folders
excludeDirs = set(args.xd)
# turn extension list into actual file extensions
includeFileTypes = tuple(['.'  + t for t in args.filetypes])

if args.verbose:
    print("excludeDirs: %s" % (excludeDirs))
    print("  filetypes: %s" % (str(includeFileTypes)))
    print("   findCRLF: %s" % (args.crlf))
    print("     findLF: %s" % (args.lf))
    print("    verbose: %s" % (args.verbose))

for dirpath, dirs, files in os.walk("./", topdown=True):
    dirs[:] = [d for d in dirs if d not in excludeDirs]
    for filename in files:
        if filename.endswith(includeFileTypes):
            fname = os.path.join(dirpath,filename)
            processFile(fname)
print("Total Lines ending with   LF: " + str(totalLF))
print("Total Lines ending with CRLF: " + str(totalCRLF))