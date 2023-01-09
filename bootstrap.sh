#!/usr/bin/env bash

set -e

which convert > /dev/null 2>&1
hasConvert=$?
which optipng > /dev/null 2>&1
hasOptiPng=$?

cd "$(dirname "$0")"

rm -rf ./resources/pokemon
curl https://veekun.com/static/pokedex/downloads/generation-2.tar.gz | tar -xvzC ./resources
rm -rf ./resources/pokemon/main-sprites/crystal ./resources/pokemon/main-sprites/silver

pokedex="./src/pkjs/pokedex.json"
echo '[""' > $pokedex

grep main-sprites package.json | xargs -l | cut -d" " -f2 | cut -d"," -f1 | while read -r file; do
  file="./resources/$file"
  fileColor="${file%.*}~color.png"
  fileBw="${file%.*}~bw.png"
  if [[ $hasConvert ]]; then
    convert "$file" -flatten -alpha off -type palette "$fileColor"
    convert "$file" -flatten -alpha off -monochrome -type palette "$fileBw"
  else
    cp "$file" "${file%.*}~color.png"
    cp "$file" "${file%.*}~bw.png"
  fi
  if [[ $hasOptiPng ]]; then
    optipng -strip all -o7 "$fileColor"
    optipng -strip all -o7 "$fileBw"
  fi
  if [[ "$file" != *"back"* ]]; then
    echo ',"'$(base64 -w0 "$fileColor")'"' >> $pokedex
  fi
  rm "$file"
done

echo ']' >> $pokedex
