#!/usr/bin/env bash

cd ../../

# install dependencies (Python PIP must be installed!)
pip install -r requirements.txt

# set Flask Environment Variable
export FLASK_APP=insecureapp.py
export FLASK_DIRECTORY=$PWD

# Turn on flask
flask run