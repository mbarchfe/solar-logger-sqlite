#!/bin/shell
ps -ef | grep "[s]olar-logger" | awk '{print $2}' | xargs kill

