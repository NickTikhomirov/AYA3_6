#!/bin/sh
set -e

files=`find . -name "*.cpp" | grep -v "./tools/*" | grep -v "./third-party/*"`
filter=-build/c++11,-runtime/references,-whitespace/braces,-whitespace/indent,-whitespace/line_length,-whitespace/comments,-build/include_order,-whitespace/tab,-whitespace/newline,-runtime/threadsafe_fn
echo $files | xargs cpplint --filter=$filter