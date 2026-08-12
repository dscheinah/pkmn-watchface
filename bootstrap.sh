#!/usr/bin/env bash

which magick > /dev/null 2>&1
hasConvert=$?
which optipng > /dev/null 2>&1
hasOptiPng=$?

set -e

cd "$(dirname "$0")"

rm -rf ./app/resources/pokemon
curl https://veekun.com/static/pokedex/downloads/generation-2.tar.gz | tar -xvzC ./app/resources
rm -rf ./app/resources/pokemon/main-sprites/crystal ./app/resources/pokemon/main-sprites/silver

pokedex="./app/src/pkjs/pokedex.json"
echo '[""' > $pokedex

prefix=./app/resources/pokemon/main-sprites/gold

if [ $hasConvert -eq 0 ]; then
  magick "$prefix/back/2.png" -level 0%,100%,0.9 "$prefix/back/2.png"
  magick "$prefix/back/4.png" -level 0%,100%,0.7 "$prefix/back/4.png"
  magick "$prefix/back/5.png" -level 0%,100%,0.9 "$prefix/back/5.png"
  magick "$prefix/back/6.png" -level 0%,100%,0.8 "$prefix/back/6.png"
  magick "$prefix/back/7.png" -level 0%,100%,0.9 "$prefix/back/7.png"
  magick "$prefix/back/8.png" -level 0%,100%,0.9 "$prefix/back/8.png"
  magick "$prefix/back/142.png" -level 0%,100%,1.1 "$prefix/back/142.png"
  magick "$prefix/back/158.png" -level 0%,100%,0.8 "$prefix/back/158.png"
  magick "$prefix/back/159.png" -level 0%,100%,0.7 "$prefix/back/159.png"
  magick "$prefix/back/shiny/1.png" -level 0%,100%,2 "$prefix/back/shiny/1.png"
  magick "$prefix/back/shiny/5.png" -level 0%,100%,1.4 "$prefix/back/shiny/5.png"
  magick "$prefix/back/shiny/154.png" -level 0%,100%,1.1 "$prefix/back/shiny/154.png"
  magick "$prefix/back/shiny/160.png" -level 0%,100%,1.1 "$prefix/back/shiny/160.png"
  magick "$prefix/92.png" -level 0%,100%,0.7 "$prefix/92.png"
  magick "$prefix/93.png" -level 0%,100%,1.7 "$prefix/93.png"
  magick "$prefix/94.png" -level 0%,100%,1.5 "$prefix/94.png"
  magick "$prefix/200.png" -level 0%,100%,1.7 "$prefix/200.png"
  magick "$prefix/214.png" -level 0%,100%,1.2 "$prefix/214.png"
  magick "$prefix/216.png" -level 0%,100%,1.1 "$prefix/216.png"

  for i in 125 200 214 225 239 241; do
    magick "$prefix/$i.png" -sample 44x44 "$prefix/$i.png"
  done
  for i in 26 64 92 203 217 235; do
    magick "$prefix/$i.png" -sample 48x48 "$prefix/$i.png"
  done
  for i in 3 6 9 65 143 144 145 146 243 244 245 249 250; do
    magick "$prefix/$i.png" -sample 52x52 "$prefix/$i.png"
  done
fi

grep main-sprites app/package.json | xargs -l | cut -d" " -f2 | cut -d"," -f1 | while read -r file; do
  file="./app/resources/$file"
  fileColor="${file%.*}~color.png"
  fileBw="${file%.*}~bw.png"
  fileUp="${file%.*}~color~228h.png"
  if [ $hasConvert -eq 0 ]; then
    magick "$file" -flatten -alpha off +dither -remap ./pebble_colors_64.gif -trim -colors 4 -type palette "$fileColor"
    magick "$file" -flatten -alpha off -monochrome -trim -colors 2 -type palette "$fileBw"
    magick "$file" -sample 120% -flatten -alpha off +dither -remap ./pebble_colors_64.gif -trim -colors 4 -type palette "$fileUp"
  else
    cp "$file" "$fileColor"
    cp "$file" "$fileBw"
    cp "$file" "$fileUp"
  fi
  if [ $hasOptiPng -eq 0 ]; then
    optipng -strip all -o7 "$fileColor"
    optipng -strip all -o7 "$fileBw"
    optipng -strip all -o7 "$fileUp"
  fi
  if [[ "$file" != *"back"* ]]; then
    line=$(base64 -w0 "$fileColor")
    if [ $hasConvert -eq 0 ]; then
      gif=$(magick "$fileColor" -strip gif:- | base64 -w0)
      if [[ ${#gif} -lt ${#line} ]]; then
        line=${gif}
      fi
    fi
    echo ',"'${line}'"' >> $pokedex
  fi
done

if [ $hasConvert -eq 0 ]; then
  magick "$prefix/back/1.png" -flatten -fill white -opaque black -colorspace gray -threshold 85% -type bilevel "$prefix/back/1~bw.png"
  magick "$prefix/back/4.png" -flatten -level 0%,100%,0.8 -alpha off -monochrome -type palette "$prefix/back/4~bw.png"
  magick "$prefix/back/7.png" -flatten -level 0%,100%,0.8 -alpha off -monochrome -type palette "$prefix/back/7~bw.png"
  magick "$prefix/back/8.png" -flatten -level 0%,100%,0.8 -alpha off -monochrome -type palette "$prefix/back/8~bw.png"
  magick "$prefix/back/shiny/4.png" -flatten -level 0%,100%,0.5 -alpha off -monochrome -type palette "$prefix/back/shiny/4~bw.png"
  magick "$prefix/back/shiny/222.png" -flatten -level 0%,100%,0.3 -alpha off -monochrome -type palette "$prefix/back/shiny/222~bw.png"
  for i in 155 156 157 201-x 201-o; do
    magick "$prefix/back/shiny/$i.png" -flatten -fill white -opaque black -colorspace gray -threshold 99% -type bilevel "$prefix/back/shiny/$i~bw.png"
  done
fi
if [ $hasOptiPng -eq 0 ]; then
  optipng -strip all -o7 "$prefix/back/1~bw.png"
  optipng -strip all -o7 "$prefix/back/4~bw.png"
  optipng -strip all -o7 "$prefix/back/7~bw.png"
  optipng -strip all -o7 "$prefix/back/8~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/4~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/155~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/156~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/157~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/222~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/201-x~bw.png"
  optipng -strip all -o7 "$prefix/back/shiny/201-o~bw.png"
fi

find $prefix -type f -not -iname "*~*" -delete
find $prefix -type d -empty -delete

echo ']' >> $pokedex
