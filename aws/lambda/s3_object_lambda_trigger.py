#steps
'''
1. Lambda function
2.Configure amazon S3 bucket as lambda event source
3.Trigger a lambda function by uploading an object to Amazon S3
4.Monitor AWS lambda S3 function through Amazon Cloud Watch

User uploads an object to the  source S3 bucket
S3 detects the object creation and publish the object creation event to lambda
by invoking it
AWS lambda executes the lambda function
From the event data lambda knows the source bucket name and object key name.
The lambda function reads the object and creates a thumbnail using graphics
then saves the thubmnail to the target bucket

step1: create a S3 bucket name "image-dsti-vipin-1"
step1: create a target S3 bucket name "image-dsti-vipin-1-resize"
step3: upload the image into the source target bucket (This can trigger lamda
        function) but here i triggered the lambda execution manually so step3 is
        fine.

step4: Create a lambda function

function name: Create-Thumbnail
runtime: Python 3.6
lambda execution role: lambda-execution-role
'''

import boto3
import os
import sys
import urllib
from PIL import Image
import PIL.Image

s3_client = boto3.client('s3')

def resize_image(image_path, resized_path):
    with Image.open(image_path) as image:
        image.thumbnail((128,128))
        image.save(resized_path)


def handler(event, context):
#Receive  an event which contains the name of incoming object
    for record in event['Records']:
        bucket = record['s3']['bucket']['name']
        key = record['s3']['object']['key']
        raw_key = urllib.parse.unquote_plus(key)
        download_path = '/tmp/{}'.format(key)
        upload_path = '/tmp/resized-{}'.format(key)
#Download the image to local storage
        s3_client.download_file(bucket, raw_key, download_path)
#Resize the image using Pillow library
        resize_image(download_path, upload_path)
#Upload the resized image to the resized bucket
        s3_client.upload_file(upload_path,
             '{}-resized'.format(bucket),
             'thumbnail-{}'.format(raw_key),
             ExtraArgs={'ContentType': 'image/jpeg'})


'''
Testing my lamda function:
Inside Lambda function top screen click test then
configure :
    Event template name: Amazon S3 Put
    Event name: Upload
Make necessary changes to example template like by replacing bucket name to ur
source S3 bucket and ARN
Replace key object name to your object name
Click create and test
Upon succefful execution image should be there in target resized bucket
'''

