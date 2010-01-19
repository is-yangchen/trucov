#!/bin/bash                                                                 
##############################################################################
#  Permission is hereby granted, free of charge, to any person               #
#  obtaining a copy of this software and associated documentation            #
#  files (the "Software"), to deal in the Software without                   #
#  restriction, including without limitation the rights to use,              #
#  copy, modify, merge, publish, distribute, sublicense, and#or sell         #
#  copies of the Software, and to permit persons to whom the                 #
#  Software is furnished to do so, subject to the following                  #
#  conditions:                                                               #
#                                                                            #
#  The above copyright notice and this permission notice shall be            #
#  included in all copies or substantial portions of the Software.           #
#                                                                            #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           #
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES           #
#  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                  #
#  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT               #
#  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,              #
#  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING              #
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR             #
#  OTHER DEALINGS IN THE SOFTWARE.                                           #
##############################################################################
#  COPYRIGHT (c) 2009 Schweitzer Engineering Laboratories, Pullman, WA       #
##############################################################################
# svn_revision_script.sh                                                     #
#                                                                            #
# Once started this program will run forever.                                #
# Input a filename under svn source control.                                 #
# Outputs the current working revision number.                               #
#                                                                            #
# The svn info command is expected to output the following pattern           #
#   .*\nRevision: [0-9]*\n.*                                                 #
##############################################################################

# Get svn info dump of the file
file_info=`svn info $1`
is_next=0
output=""

# We must parse through svn raw info dump to get the revision number.
for x in $file_info
do
    # If last value was 'Revision:' then current is the revision number.
    if [ $is_next -eq 1 ]
    then
        output=$x &> /dev/null
        break
    fi

    # If 'Revision:', then revision number is next.
    if [[ $x == Revision: ]]    
    then
        is_next=1
    fi
done

# Output 
if [[ $output == "" ]]
then
   echo "" 
else
   echo @@$output
fi



