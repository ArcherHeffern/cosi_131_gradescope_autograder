#!/usr/bin/env bash

set -euo pipefail

[ -e Archive.zip ] && rm Archive.zip
zip -r Archive.zip test.c run_autograder setup.sh
