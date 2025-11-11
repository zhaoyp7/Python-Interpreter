#!/bin/bash

cd $(dirname $0)

antlr4 -no-listener -visitor -Dlanguage=Cpp Python3Lexer.g4 Python3Parser.g4 -o "../generated"

###
#
# If you do no want to do the bonus of
# "Supporting More Grammar of Python",
# You do no need to understand this file currently.
#
###
