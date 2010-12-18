#############################################################################
# Copyright (C) 2010 Timothy Lovorn
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#############################################################################

import sys

from RunInterface import RunInterface

if len(sys.argv) < 2:
    print "usage: python test_RunInterface.py path"
    sys.exit(1)
path = sys.argv[1]

testRunInterface = RunInterface(path, "test_cfg")
testConfigs = testRunInterface.oneDimRun("test_xrun", "x", 0.04, 0.18, 0.04)
testRunInterface.doRun(testConfigs, maxProcesses=4)