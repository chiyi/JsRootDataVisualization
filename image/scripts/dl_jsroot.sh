#!/bin/bash

echo "Download JsRoot772.tar.gz"
wget https://github.com/root-project/jsroot/releases/download/7.7.2/JsRoot772.tar.gz
mkdir /sv_dir/jsroot
tar xzvf JsRoot772.tar.gz -C jsroot
chown -R 1000:1000 jsroot

