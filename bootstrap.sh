#!/usr/bin/env bash

set -e

which convert > /dev/null 2>&1
hasConvert=$?
which optipng > /dev/null 2>&1
hasOptiPng=$?

cd "$(dirname "$0")/resources"

rm -rf ./pokemon
curl https://veekun.com/static/pokedex/downloads/generation-2.tar.gz | tar -xvz
rm -rf ./pokemon/main-sprites/crystal ./pokemon/main-sprites/silver

find ./pokemon -type f -name "*.png" | while read -r file; do
  if [[ $hasConvert ]]; then
    convert "$file" -flatten -alpha off -type palette "${file%.*}~color.png"
    convert "$file" -flatten -alpha off -monochrome -type palette "${file%.*}~bw.png"
  else
    cp "$file" "${file%.*}~color.png"
    cp "$file" "${file%.*}~bw.png"
  fi
  if [[ $hasOptiPng ]]; then
    optipng -strip all -o7 "${file%.*}~color.png"
    optipng -strip all -o7 "${file%.*}~bw.png"
  fi
  rm $file;
done
