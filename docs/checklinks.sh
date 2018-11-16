#!/bin/sh

# This uses the official W3C link-checker tool:
#
#   https://github.com/w3c/link-checker
#
checklink \
	--summary \
	--broken \
	--location=http://127.0.0.1:8000/ \
	--exclude 'github.com|board.byuu.org|helmet.kafuka.org' \
	http://127.0.0.1:8000/
