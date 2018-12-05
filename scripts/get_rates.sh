#!/bin/bash

BASE=https://www.ecb.europa.eu/stats/eurofxref

HIST_ZIP=$BASE/eurofxref-hist.zip
LAST_ZIP=$BASE/eurofxref.zip

HIST_XML=$BASE/eurofxref-hist.xml
LAST_XML=$BASE/eurofxref-daily.xml

THIS_DIR=`dirname $0`
OUT_DIR=$THIS_DIR/data

XMLLINT=`which xmllint`

print_help()
{
    echo "Usage: $0 <hist|daily> <xml|zip>"
    echo " hist(orical) or daily data"
    echo " xml or zipped csv"
    echo "If xmllint is found - then it will be used to format xml"
    exit 1
}

TYPE=$1
FORMAT=$2

if [ x"$TYPE" != x"hist" -a x"$TYPE" != x"daily" ]
then
    print_help
fi

if [ x"$FORMAT" != x"xml" -a x"$FORMAT" != x"zip" ]
then
    print_help
fi

if [ ! -d $OUT_DIR ]
then
    mkdir -v $OUT_DIR
fi

if [ $FORMAT == "xml" ]
then
    data=$LAST_XML
    if [ $TYPE == "hist" ]
    then
        data=$HIST_XML
    fi

    if [ -z $XMLLINT ]
    then
        echo "Not using xmllint"
        curl -s $data?$RANDOM -o $OUT_DIR/$TYPE.$FORMAT
    else
        echo "Using xmllint for formatting"
        curl -s $data?$RANDOM | $XMLLINT --format -o $OUT_DIR/$TYPE.$FORMAT -
    fi
else
    data=$LAST_ZIP
    if [ $TYPE == "hist" ]
    then
        data=$HIST_ZIP
    fi
    curl -s $data?$RANDOM -o $OUT_DIR/$TYPE.$FORMAT
fi

ls -l $OUT_DIR
