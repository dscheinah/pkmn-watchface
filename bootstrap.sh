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

prefix=./resources/pokemon/main-sprites/gold

if [[ $hasConvert ]]; then
  convert "$prefix/back/2.png" -level 0%,100%,0.9 "$prefix/back/2.png"
  convert "$prefix/back/4.png" -level 0%,100%,0.7 "$prefix/back/4.png"
  convert "$prefix/back/5.png" -level 0%,100%,0.9 "$prefix/back/5.png"
  convert "$prefix/back/6.png" -level 0%,100%,0.8 "$prefix/back/6.png"
  convert "$prefix/back/7.png" -level 0%,100%,0.9 "$prefix/back/7.png"
  convert "$prefix/back/8.png" -level 0%,100%,0.9 "$prefix/back/8.png"
  convert "$prefix/back/142.png" -level 0%,100%,1.1 "$prefix/back/142.png"
  convert "$prefix/back/158.png" -level 0%,100%,0.8 "$prefix/back/158.png"
  convert "$prefix/back/159.png" -level 0%,100%,0.7 "$prefix/back/159.png"
  convert "$prefix/back/shiny/1.png" -level 0%,100%,2 "$prefix/back/shiny/1.png"
  convert "$prefix/back/shiny/5.png" -level 0%,100%,1.4 "$prefix/back/shiny/5.png"
  convert "$prefix/back/shiny/154.png" -level 0%,100%,1.1 "$prefix/back/shiny/154.png"
  convert "$prefix/back/shiny/160.png" -level 0%,100%,1.1 "$prefix/back/shiny/160.png"
  convert "$prefix/92.png" -level 0%,100%,0.7 "$prefix/92.png"
  convert "$prefix/93.png" -level 0%,100%,1.7 "$prefix/93.png"
  convert "$prefix/94.png" -level 0%,100%,1.5 "$prefix/94.png"
  convert "$prefix/214.png" -level 0%,100%,1.2 "$prefix/214.png"

  for i in 125 200 214 225 241; do
    convert "$prefix/$i.png" -sample 44x44 "$prefix/$i.png"
  done
  for i in 26 92 203 217 235; do
    convert "$prefix/$i.png" -sample 48x48 "$prefix/$i.png"
  done
  for i in 143 144 145 146 243 244 245 249 250; do
    convert "$prefix/$i.png" -sample 52x52 "$prefix/$i.png"
  done
fi

grep main-sprites package.json | xargs -l | cut -d" " -f2 | cut -d"," -f1 | while read -r file; do
  file="./resources/$file"
  fileColor="${file%.*}~color.png"
  fileBw="${file%.*}~bw.png"
  if [[ $hasConvert ]]; then
    convert "$file" -flatten -alpha off +dither -remap ./pebble_colors_64.gif -trim -colors 4 -type palette "$fileColor"
    convert "$file" -flatten -alpha off -monochrome -trim -colors 2 -type palette "$fileBw"
  else
    cp "$file" "${file%.*}~color.png"
    cp "$file" "${file%.*}~bw.png"
  fi
  if [[ $hasOptiPng ]]; then
    optipng -strip all -o7 "$fileColor"
    optipng -strip all -o7 "$fileBw"
  fi
  if [[ "$file" != *"back"* ]]; then
    line=$(base64 -w0 "$fileColor")
    if [[ $hasConvert ]]; then
      gif=$(convert "$fileColor" -strip gif:- | base64 -w0)
      if [[ ${#gif} -lt ${#line} ]]; then
        line=${gif}
      fi
    fi
    echo ',"'${line}'"' >> $pokedex
  fi
done

if [[ $hasConvert ]]; then
  convert "$prefix/back/1.png" -flatten -alpha off -colorspace gray -threshold 85% -type bilevel "$prefix/back/1.base.png"
  convert "$prefix/back/1.png" -flatten -channel rgba -transparent black -fill black -opaque white -fill white -opaque none -alpha off -colorspace gray -threshold 85% -type bilevel "$prefix/back/1.mask.png"
  convert "$prefix/back/1.base.png" "$prefix/back/1.mask.png" -compose add -composite "$prefix/back/1~bw.png"
  convert "$prefix/back/4.png" -flatten -level 0%,100%,0.8 -alpha off -monochrome -type palette "$prefix/back/4~bw.png"
  convert "$prefix/back/7.png" -flatten -level 0%,100%,0.8 -alpha off -monochrome -type palette "$prefix/back/7~bw.png"
  convert "$prefix/back/8.png" -flatten -level 0%,100%,0.8 -alpha off -monochrome -type palette "$prefix/back/8~bw.png"
  convert "$prefix/back/shiny/4.png" -flatten -level 0%,100%,0.5 -alpha off -monochrome -type palette "$prefix/back/shiny/4~bw.png"
  convert "$prefix/back/shiny/222.png" -flatten -level 0%,100%,0.3 -alpha off -monochrome -type palette "$prefix/back/shiny/222~bw.png"
  for i in 155 156 157; do
    convert "$prefix/back/shiny/$i.png" -flatten -alpha off -colorspace gray -threshold 99% -type bilevel "$prefix/back/shiny/$i.base.png"
    convert "$prefix/back/shiny/$i.png" -flatten -channel rgba -transparent black -fill black -opaque white -fill white -opaque none -alpha off -colorspace gray -threshold 99% -type bilevel "$prefix/back/shiny/$i.mask.png"
    convert "$prefix/back/shiny/$i.base.png" "$prefix/back/shiny/$i.mask.png" -compose add -composite "$prefix/back/shiny/$i~bw.png"
  done
fi
if [[ $hasOptiPng ]]; then
  optipng -strip all -o7 "$prefix/back/1~bw.png"
  optipng -strip all -o7 "$prefix/back/4~bw.png"
  optipng -strip all -o7 "$prefix/back/7~bw.png"
  optipng -strip all -o7 "$prefix/back/8~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/4~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/155~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/156~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/157~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/222~bw.png"
fi

find $prefix -type f -not -iname "*~*" -delete
find $prefix -type d -empty -delete

echo ']' >> $pokedex
