#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG=run_daily.log
cd $DIR
nohup python3 ./cool.py &
