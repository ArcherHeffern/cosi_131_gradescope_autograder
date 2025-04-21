#!/usr/bin/env bash

set -euo pipefail

zip -r Archive.zip lib/ src/ run_autograder setup.sh
