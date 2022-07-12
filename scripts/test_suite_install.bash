#!/bin/bash

# Pulls GAUSSIANFILES.zip from public google drive and unzips
if [ -z "$1" ]; then
  DOWNLOAD_TO="."
elif [ -d "$1" ]; then
  DOWNLOAD_TO=$1
else
  echo "Directory does not exist "$1
  exit 1
fi

pip install --user --upgrade pip
pip install --user gdown
gdown https://drive.google.com/uc?id=1rCsj_jpMyE0S0cokFJDyBSA0aPNiIHNb

unzip "GAUSSIANFILES.zip"
#cleanup
rm GAUSSIANFILES.zip

if [ "$DOWNLOAD_TO" != "." ]; then
  mv GAUSSIANFILES $DOWNLOAD_TO
fi



